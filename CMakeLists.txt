# SonicPad: Sonic Adventure (PAL v1.003) static recompilation for Apple platforms.
#
# Game code runs as ahead-of-time compiled C++ generated from the owner's disc
# (port/recomp + generated/recomp). Flycast, pinned under ref/flycast, supplies
# the Dreamcast hardware layer (PowerVR2, AICA, GD-ROM, Maple) and the SH-4
# interpreter used as the fallback/oracle. No host JIT is built.
cmake_minimum_required(VERSION 3.24)

set(CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "SonicPad builds arm64 only")
project(sonicpad C CXX OBJC OBJCXX)

set(SONICPAD_FLYCAST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ref/flycast CACHE PATH "Pinned Flycast checkout")
set(SONICPAD_RECOMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated/recomp CACHE PATH "Generated AOT block sources")
if(NOT EXISTS ${SONICPAD_FLYCAST_DIR}/core/emulator.cpp)
	message(FATAL_ERROR "Pinned Flycast checkout missing at ${SONICPAD_FLYCAST_DIR}; see docs/BUILDING.md")
endif()

# Flycast options for SonicPad: OpenGL renderer, no optional services.
set(USE_VULKAN OFF CACHE BOOL "" FORCE)
set(USE_BREAKPAD OFF CACHE BOOL "" FORCE)
set(USE_LUA OFF CACHE BOOL "" FORCE)
set(USE_OPENMP OFF CACHE BOOL "" FORCE)
if(APPLE AND NOT ZLIB_LIBRARY)
	find_library(SONICPAD_SYSTEM_ZLIB z REQUIRED)
	set(ZLIB_LIBRARY ${SONICPAD_SYSTEM_ZLIB} CACHE STRING "System zlib" FORCE)
endif()

add_subdirectory(${SONICPAD_FLYCAST_DIR} flycast)

# Replace Flycast's SH-4 executor with the SonicPad AOT executor. The pinned
# checkout stays untouched; a configured copy of emulator.cpp is compiled instead.
set(_sp_emu_src ${SONICPAD_FLYCAST_DIR}/core/emulator.cpp)
file(READ ${_sp_emu_src} _sp_emu)
string(REPLACE "interpreter = Get_Sh4Interpreter();" "interpreter = SonicPad_CreateSh4Executor();" _sp_emu_patched "${_sp_emu}")
if(_sp_emu_patched STREQUAL _sp_emu)
	message(FATAL_ERROR "Flycast executor hook not found in ${_sp_emu_src}; update the SonicPad hook for this pin")
endif()
set(_sp_emu_out ${CMAKE_BINARY_DIR}/sonicpad/emulator.cpp)
file(WRITE ${_sp_emu_out}.tmp "class Sh4Executor;\nSh4Executor *SonicPad_CreateSh4Executor();\n${_sp_emu_patched}")
configure_file(${_sp_emu_out}.tmp ${_sp_emu_out} COPYONLY)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${_sp_emu_src})

get_target_property(_sp_sources flycast SOURCES)
list(REMOVE_ITEM _sp_sources core/emulator.cpp ${_sp_emu_src})
set_property(TARGET flycast PROPERTY SOURCES ${_sp_sources})

file(GLOB _sp_generated CONFIGURE_DEPENDS ${SONICPAD_RECOMP_DIR}/*.cpp)
if(NOT _sp_generated)
	set(_sp_generated ${CMAKE_CURRENT_SOURCE_DIR}/port/recomp/sp_aot_index_empty.cpp)
endif()
list(LENGTH _sp_generated _sp_generated_count)
message(STATUS "SonicPad AOT sources: ${_sp_generated_count}")

target_sources(flycast PRIVATE ${_sp_emu_out} ${CMAKE_CURRENT_SOURCE_DIR}/port/recomp/sp_aot_executor.cpp ${_sp_generated})
target_include_directories(flycast PRIVATE ${SONICPAD_FLYCAST_DIR}/core ${CMAKE_CURRENT_SOURCE_DIR}/port/recomp)
target_compile_definitions(flycast PRIVATE TARGET_NO_REC)
set_source_files_properties(${_sp_generated} PROPERTIES COMPILE_OPTIONS "-O2;-Wno-unused-function")
