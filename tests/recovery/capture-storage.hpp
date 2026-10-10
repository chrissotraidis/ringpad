#pragma once
#include <algorithm>
#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#ifdef __APPLE__
#include <sys/clonefile.h>
#endif

// Sharing is permitted only after comparing the live capture, never as an
// alternative source of guest state. Verify the resulting file as well.
inline bool capture_bytes_equal(const std::filesystem::path& path,
                                const void* data, std::size_t size) {
    std::ifstream input(path, std::ios::binary | std::ios::ate);
    if (!input || input.tellg() != static_cast<std::streamoff>(size)) return false;
    input.seekg(0);
    std::array<char, 65536> buffer;
    auto* bytes = static_cast<const char*>(data);
    for (std::size_t offset = 0; offset < size;) {
        const auto count = std::min(buffer.size(), size - offset);
        input.read(buffer.data(), count);
        if (input.gcount() != static_cast<std::streamsize>(count) ||
            std::memcmp(buffer.data(), bytes + offset, count)) return false;
        offset += count;
    }
    return true;
}

inline bool write_observed_capture(const std::filesystem::path& output,
                                   const std::filesystem::path& reference,
                                   const void* data, std::size_t size) {
    if (std::filesystem::exists(output))
        throw std::runtime_error("Capture output already exists: " + output.string());
    bool shared = false;
#ifdef __APPLE__
    if (!reference.empty() && capture_bytes_equal(reference, data, size))
        shared = clonefile(reference.c_str(), output.c_str(), 0) == 0;
#endif
    if (!shared || !capture_bytes_equal(output, data, size)) {
        shared = false;
        std::ofstream stream(output, std::ios::binary | std::ios::trunc);
        stream.write(static_cast<const char*>(data), size);
        stream.close();
        if (!stream) throw std::runtime_error("Cannot write capture: " + output.string());
    }
    if (!capture_bytes_equal(output, data, size))
        throw std::runtime_error("Capture differs from live bytes: " + output.string());
    return shared;
}
