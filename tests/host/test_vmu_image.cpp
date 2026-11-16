// Host tests for port/game/VMUImage: writing a single save into a card.
#include "VMUImage.h"

#include <cstdio>
#include <cstring>

using namespace ringpad;

static int failures = 0, checks = 0;
#define CHECK(cond, what) do { checks++; if (!(cond)) { failures++; std::printf("FAIL: %s\n", what); } } while (0)

// A freshly formatted card, laid out as Flycast and the BIOS do: FAT in block
// 254, 13 directory blocks from 253 down, 200 user blocks.
static std::vector<uint8_t> blank_card()
{
    std::vector<uint8_t> b(kVMUImageBytes, 0);
    const size_t root = 255 * 512, fat = 254 * 512;
    std::memset(&b[root], 0x55, 16);
    auto w16 = [&](size_t o, uint16_t v) { b[o] = uint8_t(v); b[o + 1] = uint8_t(v >> 8); };
    w16(root + 0x46, 254); w16(root + 0x48, 1); w16(root + 0x4A, 253); w16(root + 0x4C, 13); w16(root + 0x50, 200);
    for (int i = 0; i < 256; i++) w16(fat + i * 2, 0xFFFC);
    w16(fat + 255 * 2, 0xFFFA); w16(fat + 254 * 2, 0xFFFA);
    for (int d = 253; d > 241; d--) w16(fat + d * 2, uint16_t(d - 1));
    w16(fat + 241 * 2, 0xFFFA);
    return b;
}

static VMUFile save(const char *name, size_t bytes, uint8_t fill)
{
    VMUFile f;
    f.name = name;
    f.data.assign(bytes, fill);
    return f;
}

int main()
{
    const uint8_t when[8] = {0x20, 0x26, 0x10, 0x22, 0x14, 0x00, 0x00, 0x03};
    std::vector<uint8_t> card = blank_card();
    CHECK(vmu_image_problem(card).empty(), "a formatted card is accepted");
    CHECK(!vmu_image_problem(std::vector<uint8_t>(1000)).empty(), "a short file is rejected");

    CHECK(vmu_write_file(card, save("SONICADV_INT", 5120, 0xA1), when).empty(), "a 10-block save is written");
    CHECK(vmu_list_files(card).size() == 1 && vmu_list_files(card)[0] == "SONICADV_INT", "the save is listed");
    CHECK(card[199 * 512] == 0xA1 && card[190 * 512] == 0xA1 && card[189 * 512] == 0, "it takes blocks 199 down to 190");

    CHECK(vmu_write_file(card, save("OTHERGAME", 1024, 0xB2), when).empty(), "a second file is written");
    CHECK(vmu_write_file(card, save("SONICADV_INT", 5120, 0xC3), when).empty(), "the save is replaced");
    const auto names = vmu_list_files(card);
    CHECK(names.size() == 2, "replacing keeps one copy and the other file");
    CHECK(card[188 * 512] == 0xB2, "the other file is untouched");
    int c3 = 0;
    for (int blk = 0; blk < 200; blk++) c3 += card[blk * 512] == 0xC3;
    CHECK(c3 == 10, "the replacement occupies exactly 10 blocks");

    std::vector<uint8_t> before = card;
    CHECK(!vmu_write_file(card, save("HUGE", 200 * 512, 0xD4), when).empty(), "a file too large is refused");
    CHECK(card == before, "a refused write leaves the card unchanged");

    std::vector<uint8_t> vmi(108, 0);
    std::memcpy(&vmi[0x58], "SONICADV_INT", 12);
    CHECK(vmu_file_from_vmi(vmi).name == "SONICADV_INT", "the .VMI names the file");
    CHECK(vmu_file_from_vmi(std::vector<uint8_t>(50)).name.empty(), "a bad .VMI gives no name");

    std::printf("%d/%d checks passed\nRESULT: %s\n", checks - failures, checks, failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
