#pragma once

/* Dreamcast VMU memory card images (128 KB, 256 blocks of 512 bytes).
 *
 * RingPad's port A card is one such image. Saves found online usually come
 * as one file: a .VMS with the file's bytes plus a 108-byte .VMI that names it
 * (Sonic Adventure's is SONICADV_INT, 10 blocks). This writes such a file into
 * an existing card, replacing a file with the same name and leaving every
 * other file alone. Pure C++ so it is host-tested. */

#include <cstdint>
#include <string>
#include <vector>

namespace ringpad {

constexpr size_t kVMUImageBytes = 128 * 1024;

struct VMUFile {
    std::string name;          /* up to 12 characters, e.g. "SONICADV_INT" */
    bool game = false;         /* a VMU mini-game (0xCC) rather than data (0x33) */
    bool copy_protected = false;
    std::vector<uint8_t> data; /* whole blocks; padded to 512 bytes when written */
};

/* Empty when the image is a formatted card, otherwise why it is not. */
std::string vmu_image_problem(const std::vector<uint8_t>& image);

/* Reads name and flags from a .VMI; empty name when it is not one. */
VMUFile vmu_file_from_vmi(const std::vector<uint8_t>& vmi);

/* Names of the files on a card. */
std::vector<std::string> vmu_list_files(const std::vector<uint8_t>& image);

/* Writes the file into the card, replacing one with the same name. Returns an
 * empty string on success, otherwise a reason (the card is then unchanged).
 * timestamp is 8 BCD bytes: century, year, month, day, hour, minute, second, weekday. */
std::string vmu_write_file(std::vector<uint8_t>& image, const VMUFile& file, const uint8_t timestamp[8]);

} // namespace ringpad
