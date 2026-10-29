// See VMUImage.h. Layout: block 255 is the root (16 bytes of 0x55, then the
// FAT and directory locations); the FAT holds one 16-bit entry per block
// (0xFFFC free, 0xFFFA end of a file); each directory block holds sixteen
// 32-byte entries.
#include "VMUImage.h"

#include <algorithm>
#include <cstring>

namespace sonicpad {
namespace {

constexpr size_t kBlock = 512;
constexpr uint16_t kFree = 0xFFFC, kEnd = 0xFFFA;

uint16_t rd16(const std::vector<uint8_t>& b, size_t o) { return uint16_t(b[o] | (b[o + 1] << 8)); }
void wr16(std::vector<uint8_t>& b, size_t o, uint16_t v) { b[o] = uint8_t(v); b[o + 1] = uint8_t(v >> 8); }

struct Layout {
    unsigned fat, dir, dir_blocks, user_blocks;
};

Layout layout(const std::vector<uint8_t>& b)
{
    const size_t root = 255 * kBlock;
    return {rd16(b, root + 0x46), rd16(b, root + 0x4A), rd16(b, root + 0x4C), rd16(b, root + 0x50)};
}

std::string entry_name(const std::vector<uint8_t>& b, size_t entry)
{
    std::string name(reinterpret_cast<const char *>(&b[entry + 4]), 12);
    name.erase(name.find_last_not_of(std::string(" \0", 2)) + 1);
    return name;
}

template <typename F> void each_entry(const Layout& l, F f)
{
    for (unsigned i = 0; i < l.dir_blocks; i++)
        for (unsigned e = 0; e < 16; e++)
            f(size_t(l.dir - i) * kBlock + e * 32);
}

} // namespace

std::string vmu_image_problem(const std::vector<uint8_t>& b)
{
    if (b.size() != kVMUImageBytes)
        return "a memory card image is exactly 131,072 bytes";
    for (size_t i = 0; i < 16; i++)
        if (b[255 * kBlock + i] != 0x55)
            return "not a formatted VMU memory card";
    const Layout l = layout(b);
    if (l.fat >= 256 || l.dir >= 256 || l.dir_blocks == 0 || l.dir_blocks > l.dir + 1 || l.user_blocks == 0 || l.user_blocks > 256)
        return "the memory card's layout is damaged";
    return {};
}

VMUFile vmu_file_from_vmi(const std::vector<uint8_t>& vmi)
{
    VMUFile f;
    if (vmi.size() != 108)
        return f;
    std::string name(reinterpret_cast<const char *>(&vmi[0x58]), 12);
    name.erase(name.find_last_not_of(std::string(" \0", 2)) + 1);
    f.name = name;
    const uint16_t mode = rd16(vmi, 0x64);
    f.game = mode & 2;
    f.copy_protected = mode & 1;
    return f;
}

std::vector<std::string> vmu_list_files(const std::vector<uint8_t>& b)
{
    std::vector<std::string> names;
    if (!vmu_image_problem(b).empty())
        return names;
    each_entry(layout(b), [&](size_t e) {
        if (b[e] == 0x33 || b[e] == 0xCC)
            names.push_back(entry_name(b, e));
    });
    return names;
}

std::string vmu_write_file(std::vector<uint8_t>& image, const VMUFile& file, const uint8_t timestamp[8])
{
    if (std::string why = vmu_image_problem(image); !why.empty())
        return why;
    if (file.name.empty() || file.name.size() > 12)
        return "the save has no valid file name";
    if (file.data.empty())
        return "the save is empty";
    if (file.game)
        return "VMU mini-games are not supported, only saves";
    std::vector<uint8_t> b = image;   // work on a copy; the card changes only on success
    const Layout l = layout(b);
    const size_t fat = size_t(l.fat) * kBlock;
    const unsigned need = unsigned((file.data.size() + kBlock - 1) / kBlock);

    // Remove a file with the same name, freeing its blocks.
    size_t slot = 0;
    bool have_slot = false;
    each_entry(l, [&](size_t e) {
        if ((b[e] == 0x33 || b[e] == 0xCC) && entry_name(b, e) == file.name) {
            for (unsigned blk = rd16(b, e + 2), n = 0; blk < 256 && n < 256; n++) {
                const unsigned next = rd16(b, fat + blk * 2);
                wr16(b, fat + blk * 2, kFree);
                blk = next == kEnd ? 256 : next;
            }
            std::memset(&b[e], 0, 32);
        }
        if (!have_slot && b[e] == 0) {
            slot = e;
            have_slot = true;
        }
    });
    if (!have_slot)
        return "the memory card's directory is full";

    // Data files take free user blocks from the top down, as the BIOS does.
    std::vector<unsigned> blocks;
    for (int blk = int(l.user_blocks) - 1; blk >= 0 && blocks.size() < need; blk--)
        if (rd16(b, fat + blk * 2) == kFree)
            blocks.push_back(unsigned(blk));
    if (blocks.size() < need)
        return "the memory card does not have " + std::to_string(need) + " free blocks";
    for (size_t i = 0; i < blocks.size(); i++) {
        wr16(b, fat + blocks[i] * 2, i + 1 < blocks.size() ? uint16_t(blocks[i + 1]) : kEnd);
        uint8_t *dst = &b[blocks[i] * kBlock];
        std::memset(dst, 0, kBlock);
        const size_t from = i * kBlock;
        std::memcpy(dst, &file.data[from], std::min(kBlock, file.data.size() - from));
    }

    b[slot] = 0x33;
    b[slot + 1] = file.copy_protected ? 0xFF : 0x00;
    wr16(b, slot + 2, uint16_t(blocks[0]));
    std::memset(&b[slot + 4], ' ', 12);
    std::memcpy(&b[slot + 4], file.name.data(), file.name.size());
    std::memcpy(&b[slot + 0x10], timestamp, 8);
    wr16(b, slot + 0x18, uint16_t(need));
    wr16(b, slot + 0x1A, 0);
    image.swap(b);
    return {};
}

} // namespace sonicpad
