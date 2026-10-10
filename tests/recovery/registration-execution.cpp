// Actual SBOARD registration with declared task/heap state, not scheduler boot.
#include "dc_image.hpp"
#include "generated_program.hpp"
#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
using namespace dcrecomp_generated;
#include "registration-check.hpp"
static void check(unsigned size, bool head, bool success) {
    auto runtime = std::make_unique<DCRuntime>();
    load_embedded_elf_image(*runtime); register_recompiled_program(*runtime);
    check_registration(*runtime,size,head,success);
}
int main() {
    try {
        check(256,true,true); check(32,true,true);
        check(16,true,false); check(256,false,false);
    } catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;}
}
