// See SPDiagnostics.h.
#import "SPDiagnostics.h"

#import <os/log.h>

#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <sys/time.h>
#include <unistd.h>

#include <atomic>
#include <cstdio>
#include <ctime>
#include <string>

static constexpr off_t kRotateBytes = 2 * 1024 * 1024;
static std::atomic<int> gLogFD{-1};
static int gConsoleFD = -1;   // the original stderr, so Xcode and devicectl still see output
static NSString *gLogDir;

static NSString *CurrentPath(void) { return [gLogDir stringByAppendingPathComponent:@"sonicpad.log"]; }
static NSString *PreviousPath(void) { return [gLogDir stringByAppendingPathComponent:@"sonicpad.previous.log"]; }

static int OpenLog(void)
{
	return open(CurrentPath().fileSystemRepresentation, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644);
}

static void Rotate(void)
{
	rename(CurrentPath().fileSystemRepresentation, PreviousPath().fileSystemRepresentation);
}

static size_t Timestamp(char *buf, size_t size)
{
	timeval tv;
	gettimeofday(&tv, nullptr);
	tm local;
	localtime_r(&tv.tv_sec, &local);
	const size_t n = strftime(buf, size, "%H:%M:%S", &local);
	return n + (size_t)snprintf(buf + n, size - n, ".%03d ", (int)(tv.tv_usec / 1000));
}

// Reads everything written to stderr, echoes it to the original stderr and
// appends it to the log one timestamped line at a time. This thread is the
// only writer apart from the crash handler, so lines never interleave.
static void *TeeThread(void *arg)
{
	const int in = (int)(intptr_t)arg;
	std::string pending;
	char buf[4096];
	off_t written = 0;
	struct stat st;
	if (fstat(gLogFD, &st) == 0)
		written = st.st_size;
	for (;;) {
		const ssize_t n = read(in, buf, sizeof buf);
		if (n <= 0)
			break;
		if (gConsoleFD >= 0)
			write(gConsoleFD, buf, (size_t)n);
		pending.append(buf, (size_t)n);
		size_t start = 0, nl;
		while ((nl = pending.find('\n', start)) != std::string::npos) {
			char stamp[32];
			const size_t sl = Timestamp(stamp, sizeof stamp);
			const int fd = gLogFD;
			if (fd >= 0) {
				write(fd, stamp, sl);
				write(fd, pending.data() + start, nl - start + 1);
				written += (off_t)(sl + nl - start + 1);
			}
			start = nl + 1;
		}
		pending.erase(0, start);
		if (written > kRotateBytes) {
			const int old = gLogFD.exchange(-1);
			close(old);
			Rotate();
			gLogFD = OpenLog();
			written = 0;
		}
	}
	return nullptr;
}

static NSString *Machine(void)
{
	char name[64] = {};
	size_t size = sizeof name - 1;
	sysctlbyname("hw.machine", name, &size, nullptr, 0);
	return [NSString stringWithUTF8String:name];
}

static NSString *ThermalName(NSProcessInfoThermalState s)
{
	switch (s) {
	case NSProcessInfoThermalStateNominal: return @"nominal";
	case NSProcessInfoThermalStateFair: return @"fair";
	case NSProcessInfoThermalStateSerious: return @"serious";
	case NSProcessInfoThermalStateCritical: return @"critical";
	}
	return @"unknown";
}

void SPDiagnosticsStart(void)
{
	if (gLogDir != nil)
		return;
	NSString *support = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES).firstObject;
	gLogDir = [support stringByAppendingPathComponent:@"SonicPad/Logs"];
	[NSFileManager.defaultManager createDirectoryAtPath:gLogDir withIntermediateDirectories:YES attributes:nil error:nil];
	Rotate();   // each launch starts a fresh file; the last session stays readable
	gLogFD = OpenLog();

	int fds[2];
	if (gLogFD >= 0 && pipe(fds) == 0) {
		gConsoleFD = dup(STDERR_FILENO);
		dup2(fds[1], STDERR_FILENO);
		close(fds[1]);
		setvbuf(stderr, nullptr, _IOLBF, 0);
		pthread_t thread;
		pthread_create(&thread, nullptr, TeeThread, (void *)(intptr_t)fds[0]);
		pthread_detach(thread);
	}

	NSBundle *bundle = NSBundle.mainBundle;
	NSProcessInfo *info = NSProcessInfo.processInfo;
	UIDevice *device = UIDevice.currentDevice;
	NSDictionary *fs = [NSFileManager.defaultManager attributesOfFileSystemForPath:NSHomeDirectory() error:nil];
	SPLog(@"session start: SonicPad %@ (%@) on %@ %@ %@, %.1f GB RAM, %.1f GB free, thermal %@, low power %d",
			[bundle objectForInfoDictionaryKey:@"CFBundleShortVersionString"], [bundle objectForInfoDictionaryKey:@"CFBundleVersion"],
			Machine(), device.systemName, device.systemVersion, info.physicalMemory / 1073741824.0,
			[fs[NSFileSystemFreeSize] doubleValue] / 1e9, ThermalName(info.thermalState), info.lowPowerModeEnabled ? 1 : 0);

	NSNotificationCenter *center = NSNotificationCenter.defaultCenter;
	[center addObserverForName:UIApplicationDidReceiveMemoryWarningNotification object:nil queue:nil
			usingBlock:^(NSNotification *) { SPLog(@"memory warning"); }];
	[center addObserverForName:NSProcessInfoThermalStateDidChangeNotification object:nil queue:nil
			usingBlock:^(NSNotification *) { SPLog(@"thermal %@", ThermalName(NSProcessInfo.processInfo.thermalState)); }];
	[center addObserverForName:NSProcessInfoPowerStateDidChangeNotification object:nil queue:nil
			usingBlock:^(NSNotification *) { SPLog(@"low power %d", NSProcessInfo.processInfo.lowPowerModeEnabled ? 1 : 0); }];
}

void SPLog(NSString *format, ...)
{
	va_list args;
	va_start(args, format);
	NSString *line = [[NSString alloc] initWithFormat:format arguments:args];
	va_end(args);
	os_log(OS_LOG_DEFAULT, "SonicPad: %{public}@", line);
	fprintf(stderr, "SONICPAD_APP %s\n", line.UTF8String);
}

int SPDiagnosticsFD(void)
{
	return gLogFD;
}

NSURL *SPDiagnosticsReportURL(void)
{
	if (gLogDir == nil)
		return nil;
	fflush(stderr);
	usleep(100000);   // let the tee thread write what is in the pipe
	NSMutableData *report = [NSMutableData data];
	for (NSString *path in @[PreviousPath(), CurrentPath()]) {
		NSData *data = [NSData dataWithContentsOfFile:path];
		if (data == nil)
			continue;
		NSString *title = [NSString stringWithFormat:@"===== %@ =====\n", path.lastPathComponent];
		[report appendData:[title dataUsingEncoding:NSUTF8StringEncoding]];
		[report appendData:data];
	}
	NSDateFormatter *f = [[NSDateFormatter alloc] init];
	f.dateFormat = @"yyyyMMdd-HHmmss";
	NSString *name = [NSString stringWithFormat:@"SonicPad-Diagnostics-%@.txt", [f stringFromDate:NSDate.date]];
	NSURL *url = [NSURL fileURLWithPath:[NSTemporaryDirectory() stringByAppendingPathComponent:name]];
	return [report writeToURL:url atomically:YES] ? url : nil;
}
