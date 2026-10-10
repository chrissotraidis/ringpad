// Executes only the isolated owner-image PRS routine, not the game/loader.
// Inputs must first pass the bounded independent decoder in the Python runner.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace dcrecomp_generated;
static void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
static std::vector<std::uint8_t> read(const char* path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open input");
    return {std::istreambuf_iterator<char>(file), {}};
}
int main(int argc, char** argv) {
    try {
        require(argc == 4, "usage: prs-oracle ENCODED EXPECTED DESTINATION");
        const auto encoded = read(argv[1]), expected = read(argv[2]);
        const auto destination = static_cast<std::uint32_t>(std::stoul(argv[3], nullptr, 0));
        constexpr std::uint32_t input = 0x0CD00000, entry = 0x8C04E260;
        require(!encoded.empty() && encoded.size() < 0x2F0000, "Invalid encoded size");
        require(destination >= 0x0C900000 && destination < input &&
                expected.size() + 32 < input - destination, "Output overlaps input/outside test arena");
        auto runtime = std::make_unique<DCRuntime>();
        load_embedded_elf_image(*runtime);
        register_recompiled_program(*runtime);
        for (std::size_t n = 0; n < encoded.size(); ++n) dc_write8(*runtime, input + n, encoded[n]);
        for (std::size_t n = 0; n < expected.size() + 32; ++n)
            dc_write8(*runtime, destination - 16 + n, 0xA5);
        SH4Context ctx{};
        for (unsigned n = 0; n < 15; ++n) ctx.r[n] = 0x12340000 + n;
        ctx.pc = entry; ctx.pr = 0xFFFFFFFF; ctx.r[15] = 0x8CFFF000;
        ctx.r[4] = input; ctx.r[5] = destination;
        const auto before = ctx;
        call_recompiled(ctx, *runtime, entry);
        require(ctx.pc == before.pr && ctx.pr == before.pr, "Return PC/PR changed");
        require(ctx.r[15] == before.r[15], "Stack not restored");
        for (unsigned n = 8; n < 15; ++n) require(ctx.r[n] == before.r[n], "Preserved GPR changed");
        require(ctx.r[0] == expected.size(), "Wrong decoded length");
        require(ctx.r[4] == input + encoded.size(), "Input consumption mismatch");
        require(ctx.r[5] == destination + expected.size(), "Output pointer mismatch");
        for (std::size_t n = 0; n < expected.size(); ++n) {
            if (dc_read8(*runtime, destination + n) != expected[n])
                throw std::runtime_error("Decoded byte mismatch at " + std::to_string(n));
        }
        for (unsigned n = 0; n < 16; ++n) {
            require(dc_read8(*runtime, destination - 16 + n) == 0xA5, "Leading guard changed");
            require(dc_read8(*runtime, destination + expected.size() + n) == 0xA5, "Trailing guard changed");
        }
        for (std::size_t n = 0; n < encoded.size(); ++n)
            require(dc_read8(*runtime, input + n) == encoded[n], "Encoded input changed");
        bool rejected = false;
        try {
            call_recompiled(ctx, *runtime, 0x8C700000u);
        } catch (const std::runtime_error& error) {
            rejected = std::string(error.what()).find("No recompiled/native target registered") != std::string::npos;
        }
        require(rejected, "Missing target did not fail explicitly");
        std::cout << "PASS bytes=" << expected.size()
                  << " input_consumed=" << encoded.size()
                  << " guards/input/GPRs/stack/return preserved; missing target rejected\n";
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}
