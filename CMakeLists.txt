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

# SonicPad hooks into Flycast are applied to configured copies of individual
# source files, so the pinned checkout stays untouched. Each replacement must
# match exactly once, or configuration fails and the hook needs updating.
function(sonicpad_patch_flycast_source rel old new prefix)
	set(src ${SONICPAD_FLYCAST_DIR}/${rel})
	file(READ ${src} text)
	string(FIND "${text}" "${old}" first)
	string(FIND "${text}" "${old}" last REVERSE)
	if(first EQUAL -1 OR NOT first EQUAL last)
		message(FATAL_ERROR "SonicPad hook for ${rel} must match exactly once; update it for this Flycast pin")
	endif()
	string(REPLACE "${old}" "${new}" patched "${text}")
	get_filename_component(name ${rel} NAME)
	set(out ${CMAKE_BINARY_DIR}/sonicpad/${name})
	file(WRITE ${out}.tmp "${prefix}${patched}")
	configure_file(${out}.tmp ${out} COPYONLY)
	set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${src})
	get_target_property(sources flycast SOURCES)
	list(REMOVE_ITEM sources ${rel} ${src})
	set_property(TARGET flycast PROPERTY SOURCES ${sources})
	get_source_file_property(options ${src} COMPILE_OPTIONS)
	if(options)
		set_source_files_properties(${out} PROPERTIES COMPILE_OPTIONS "${options}")
	endif()
	target_sources(flycast PRIVATE ${out})
endfunction()

# Use the SonicPad AOT executor in place of Flycast's SH-4 interpreter.
sonicpad_patch_flycast_source(core/emulator.cpp
	"interpreter = Get_Sh4Interpreter();"
	"interpreter = SonicPad_CreateSh4Executor();"
	"class Sh4Executor;\nSh4Executor *SonicPad_CreateSh4Executor();\n")

if(IOS)
	# The iOS shell ignores launch arguments; boot the game named by SONICPAD_GAME.
	sonicpad_patch_flycast_source(shell/apple/emulator-ios/emulator/FlycastViewController.mm
		"flycast_init(0, nullptr);"
		"flycast_init(0, nullptr);\n\tif (const char *sonicpadGame = getenv(\"SONICPAD_GAME\"))\n\t\tsettings.content.path = sonicpadGame;"
		"")
	target_include_directories(flycast PRIVATE ${SONICPAD_FLYCAST_DIR}/shell/apple/emulator-ios/emulator)
endif()

file(GLOB _sp_generated CONFIGURE_DEPENDS ${SONICPAD_RECOMP_DIR}/*.cpp)
if(NOT _sp_generated)
	set(_sp_generated ${CMAKE_CURRENT_SOURCE_DIR}/port/recomp/sp_aot_index_empty.cpp)
endif()
list(LENGTH _sp_generated _sp_generated_count)
message(STATUS "SonicPad AOT sources: ${_sp_generated_count}")

target_sources(flycast PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/port/recomp/sp_aot_executor.cpp ${_sp_generated})
target_include_directories(flycast PRIVATE ${SONICPAD_FLYCAST_DIR}/core ${CMAKE_CURRENT_SOURCE_DIR}/port/recomp)
target_compile_definitions(flycast PRIVATE TARGET_NO_REC)
set_source_files_properties(${_sp_generated} PROPERTIES COMPILE_OPTIONS "-O2;-Wno-unused-function")
