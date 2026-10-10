#include "capture-storage.hpp"
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    if (argc != 2) return 2;
    const std::filesystem::path dir = argv[1];
    std::filesystem::create_directory(dir);
    std::vector<unsigned char> bytes(1048579);
    for (std::size_t i = 0; i < bytes.size(); ++i) bytes[i] = (i * 17) % 251;
    auto check = [](bool value) { if (!value) throw std::runtime_error("Storage control failed"); };
    check(!write_observed_capture(dir/"reference", {}, bytes.data(), bytes.size()));
    check(write_observed_capture(dir/"equal", dir/"reference", bytes.data(), bytes.size()));
    bytes[65536] ^= 0x5a;
    check(!write_observed_capture(dir/"different", dir/"reference", bytes.data(), bytes.size()));
    check(!capture_bytes_equal(dir/"reference", bytes.data(), bytes.size()));
    check(!write_observed_capture(dir/"shorter", dir/"reference", bytes.data(), bytes.size()-1));
    check(!write_observed_capture(dir/"missing", dir/"absent", bytes.data(), bytes.size()));
    check(!write_observed_capture(dir/"empty", {}, bytes.data(), 0));
    check(write_observed_capture(dir/"empty-equal", dir/"empty", bytes.data(), 0));
    bool rejected = false;
    try { write_observed_capture(dir/"equal", {}, bytes.data(), bytes.size()); }
    catch (const std::runtime_error&) { rejected = true; }
    check(rejected);
    rejected = false;
    try { write_observed_capture(dir/"nonexistent"/"output", {}, bytes.data(), bytes.size()); }
    catch (const std::runtime_error&) { rejected = true; }
    check(rejected);
    bytes[65536] ^= 0x5a;
    check(capture_bytes_equal(dir/"reference", bytes.data(), bytes.size()));
    check(capture_bytes_equal(dir/"equal", bytes.data(), bytes.size()));
    std::cout << "PASS capture storage: identical, changed, size, missing, empty, existing, write failure; source preserved\n";
}
