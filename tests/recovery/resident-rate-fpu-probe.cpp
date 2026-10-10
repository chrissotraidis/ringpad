// Diagnostic for the exact extracted runtime helpers, not a guest execution test.
#include "dc_runtime.hpp"
#include <bit>
#include <cfenv>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#pragma STDC FENV_ACCESS ON

namespace dcrecomp_generated {
[[noreturn]] void dc_unimplemented(std::uint32_t, const char* operation) {
    throw std::runtime_error(operation);
}
}
using namespace dcrecomp_generated;

static std::uint64_t fpcr() {
    std::uint64_t result;
    asm volatile("mrs %0, fpcr" : "=r"(result));
    return result;
}
static void restore_fpcr(std::uint64_t value) {
    asm volatile("msr fpcr, %0\n\tisb" :: "r"(value) : "memory");
}
struct Results { std::uint32_t product, root, ratio; };
static Results calculate() {
    volatile float literal = std::bit_cast<float>(0x40c90fdau);
    volatile float rate = 500.0f, denominator = 44100.0f, two = 2.0f;
    volatile float product = literal * rate;
    volatile float root = std::sqrt(two);
    volatile float ratio = product / denominator;
    return {std::bit_cast<std::uint32_t>(float(product)),
            std::bit_cast<std::uint32_t>(float(root)),
            std::bit_cast<std::uint32_t>(float(ratio))};
}
int main() {
    const auto original = fpcr();
    struct Restore { std::uint64_t value; ~Restore() { restore_fpcr(value); } } restore{original};
    // Establish a known host mode before asking the real helper to change it.
    restore_fpcr(original & ~((3ull << 22) | (1ull << 24)));
    if (std::fesetround(FE_TONEAREST)) return 2;
    SH4Context ctx{};
    dc_write_fpscr(ctx, 0);
    const auto nearest = calculate();
    dc_write_fpscr(ctx, 0x40001);
    dc_require_fpu_round_nearest(ctx, 0x8c65eb26);
    const auto after = fpcr();
    const auto requested = calculate();
    if (std::fesetround(FE_TOWARDZERO)) return 2;
    const auto hardware_zero = calculate();
    const bool controls = nearest.product == 0x4544597b && nearest.root == 0x3fb504f3 &&
        nearest.ratio == 0x3d91e531 && hardware_zero.product == 0x4544597a &&
        hardware_zero.root == 0x3fb504f3 && hardware_zero.ratio == 0x3d91e52f;
    const bool synchronized = (after & ((3ull << 22) | (1ull << 24))) ==
        ((3ull << 22) | (1ull << 24));
    const bool arithmetic_matches_zero = requested.product == hardware_zero.product &&
        requested.root == hardware_zero.root && requested.ratio == hardware_zero.ratio;
    std::printf("{\"controls_passed\":%s,\"runtime_mode_synchronized\":%s,"
        "\"arithmetic_matches_zero\":%s,\"fpcr_after_request\":\"%llx\","
        "\"product\":\"%08x\",\"sqrt2\":\"%08x\",\"ratio\":\"%08x\"}\n",
        controls ? "true" : "false", synchronized ? "true" : "false",
        arithmetic_matches_zero ? "true" : "false", (unsigned long long)after,
        requested.product, requested.root, requested.ratio);
    return !controls ? 2 : synchronized && arithmetic_matches_zero ? 0 : 1;
}
