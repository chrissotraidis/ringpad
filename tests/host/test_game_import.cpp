/* Host tests for the RingPad game import verifier.
 *
 * Uses small synthetic tracks with a test identity, so the checks run without
 * game data. Set RINGPAD_REAL_GDI to a real GDI folder to also verify the
 * owner's disc against the supported identity. */

#include "GameImport.h"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

using namespace ringpad;
namespace fs = std::filesystem;

static int g_failures = 0;
static int g_checks = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        ++g_checks;                                                            \
        if (!(cond)) {                                                         \
            ++g_failures;                                                      \
            std::printf("FAIL: %s (%s:%d)\n", (msg), __FILE__, __LINE__);     \
        }                                                                      \
    } while (0)

static void write(const fs::path& p, const std::string& s) {
    std::ofstream(p, std::ios::binary) << s;
}

/* SHA-256 of "abc", "de" and "fgh". */
static const char *kAbc = "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad";
static const char *kDe = "6b4e1e5f6d6a5b7b3a8a3d3b2f0e0c5f4b0b2a0b8f6b8a1d2c8a2e9d5b0e8f7a";

static GameIdentity test_identity(const std::string& de_sha, const std::string& fgh_sha) {
    static std::string de, fgh;
    de = de_sha;
    fgh = fgh_sha;
    return GameIdentity{"Test disc", "TEST", "", {{
        {1, 0, 4, 2352, 3, kAbc},
        {2, 600, 0, 2352, 2, de.c_str()},
        {3, 45000, 4, 2352, 3, fgh.c_str()},
    }}};
}

static void test_parse() {
    std::vector<GdiTrack> t;
    std::string err;
    CHECK(parse_gdi("3\n1 0 4 2352 track01.bin 0\n2 600 0 2352 \"track 02.raw\" 0\n3 45000 4 2352 track03.bin 0\n", t, err),
          "quoted names parse");
    CHECK(t.size() == 3 && t[1].filename == "track 02.raw", "quoted filename kept");
    CHECK(!parse_gdi("2\n1 0 4 2352 a.bin 0\n", t, err), "count mismatch rejected");
    CHECK(!parse_gdi("1\n1 0 4 2352 ../a.bin 0\n", t, err), "traversal rejected");
    CHECK(!parse_gdi("1\n1 0 4 2352 /tmp/a.bin 0\n", t, err), "absolute path rejected");
    CHECK(!parse_gdi("x\n", t, err), "missing count rejected");
    CHECK(parse_gdi("1\r\n1 0 4 2352 a.bin 0\r\n\r\n", t, err), "CRLF and blank lines accepted");
}

static void test_verify(const fs::path& dir) {
    fs::create_directories(dir);
    write(dir / "track01.bin", "abc");
    write(dir / "track02.raw", "de");
    write(dir / "track03.bin", "fgh");
    write(dir / "disc.gdi", "3\n1 0 4 2352 track01.bin 0\n2 600 0 2352 track02.raw 0\n3 45000 4 2352 track03.bin 0\n");
    const std::string de = sha256_file((dir / "track02.raw").string());
    const std::string fgh = sha256_file((dir / "track03.bin").string());
    CHECK(sha256_file((dir / "track01.bin").string()) == kAbc, "known SHA-256 of abc");

    uint64_t last = 0;
    GameCheck ok = verify_game(dir.string(), test_identity(de, fgh), [&](uint64_t done, uint64_t) { last = done; });
    CHECK(ok.ok, "matching folder verifies");
    CHECK(ok.track_paths.size() == 3 && last == 8, "progress covers every track byte");
    CHECK(verify_game((dir / "disc.gdi").string(), test_identity(de, fgh)).ok, "a .gdi path also verifies");

    CHECK(!verify_game(dir.string(), test_identity(kDe, fgh)).ok, "wrong track hash rejected");
    write(dir / "track03.bin", "fghi");
    CHECK(!verify_game(dir.string(), test_identity(de, fgh)).ok, "wrong track size rejected");
    write(dir / "track03.bin", "fgh");
    write(dir / "disc.gdi", "3\n1 0 4 2352 track01.bin 0\n2 601 0 2352 track02.raw 0\n3 45000 4 2352 track03.bin 0\n");
    CHECK(!verify_game(dir.string(), test_identity(de, fgh)).ok, "wrong layout rejected");
    write(dir / "disc.gdi", "3\n1 0 4 2352 track01.bin 0\n2 600 0 2352 missing.raw 0\n3 45000 4 2352 track03.bin 0\n");
    CHECK(!verify_game(dir.string(), test_identity(de, fgh)).ok, "missing track rejected");
    write(dir / "other.gdi", "1\n1 0 4 2352 track01.bin 0\n");
    CHECK(!verify_game(dir.string(), test_identity(de, fgh)).ok, "two .gdi files rejected");
    fs::remove_all(dir);
    fs::create_directories(dir);
    CHECK(!verify_game(dir.string(), test_identity(de, fgh)).ok, "empty folder rejected");
    fs::remove_all(dir);
}

int main() {
    test_parse();
    test_verify(fs::temp_directory_path() / "ringpad-import-test");
    if (const char *real = std::getenv("RINGPAD_REAL_GDI")) {
        GameCheck c = verify_game(real, supported_game());
        CHECK(c.ok, "owner's disc matches the supported identity");
        CHECK(c.gdi_sha256 == supported_game().gdi_sha256, "owner's descriptor hash matches");
        std::printf("real disc: %s\n", c.ok ? "verified" : c.error.c_str());
    }
    std::printf("\n%d/%d checks passed\n", g_checks - g_failures, g_checks);
    if (g_failures) {
        std::printf("RESULT: FAIL (%d)\n", g_failures);
        return 1;
    }
    std::printf("RESULT: PASS\n");
    return 0;
}
