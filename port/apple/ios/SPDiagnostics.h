// SonicPad's persistent runtime log, kept on the device so a problem seen in
// play can be read afterwards and shared from the ••• menu.
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/* Opens Library/Application Support/SonicPad/Logs/sonicpad.log, keeps the
 * previous session as sonicpad.previous.log and copies everything written to
 * stderr (Flycast's log and the SONICPAD_* lines) into it with timestamps.
 * Also records memory warnings, thermal and Low Power Mode changes. */
FOUNDATION_EXPORT void SPDiagnosticsStart(void);

/* One timestamped breadcrumb in the log and the unified device log.
 * For lifecycle events, not per-frame tracing. */
FOUNDATION_EXPORT void SPLog(NSString *format, ...) NS_FORMAT_FUNCTION(1, 2);

/* File descriptor of the open log, for the crash handler (-1 when closed). */
FOUNDATION_EXPORT int SPDiagnosticsFD(void);

/* Copies the previous and current session logs into one shareable file. */
FOUNDATION_EXPORT NSURL *_Nullable SPDiagnosticsReportURL(void);

NS_ASSUME_NONNULL_END
