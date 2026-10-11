#include "dc_runtime.hpp"
#include "system-flash-write-check.hpp"
int main(){try{
    check_system_flash_services();check_system_flash_writes();
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
