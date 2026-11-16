# RingPad: Sonic Adventure (PAL v1.003) static recompilation for Apple platforms.
#
# Game code runs as ahead-of-time compiled C++ generated from the owner's disc
# (port/recomp + generated/recomp). Flycast, pinned under ref/flycast, supplies
# the Dreamcast hardware layer (PowerVR2, AICA, GD-ROM, Maple) and the SH-4
# interpreter used as the fallback/oracle. No host JIT is built.
cmake_minimum_required(VERSION 3.24)

set(CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "RingPad builds arm64 only")
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
	set(CMAKE_OSX_DEPLOYMENT_TARGET "16.0" CACHE STRING "Minimum iOS version")
endif()
project(ringpad C CXX OBJC OBJCXX)

# Xcode schemes let xcodebuild run the iOS UI tests.
set(CMAKE_XCODE_GENERATE_SCHEME ON)

# Flycast forces Vulkan off for iOS in its cache; this normal variable takes
# precedence (policy CMP0126) so iOS can render through MoltenVK.
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
	set(USE_VULKAN ON)
endif()

set(RINGPAD_FLYCAST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ref/flycast CACHE PATH "Pinned Flycast checkout")
set(RINGPAD_RECOMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated/recomp CACHE PATH "Generated AOT block sources")
if(NOT EXISTS ${RINGPAD_FLYCAST_DIR}/core/emulator.cpp)
	message(FATAL_ERROR "Pinned Flycast checkout missing at ${RINGPAD_FLYCAST_DIR}; see docs/BUILDING.md")
endif()

# Flycast options for RingPad: OpenGL renderer, no optional services.
set(USE_VULKAN OFF CACHE BOOL "" FORCE)
set(USE_BREAKPAD OFF CACHE BOOL "" FORCE)
set(USE_LUA OFF CACHE BOOL "" FORCE)
set(USE_OPENMP OFF CACHE BOOL "" FORCE)
if(APPLE AND NOT CMAKE_GENERATOR STREQUAL "Xcode" AND NOT ZLIB_LIBRARY)
	find_library(RINGPAD_SYSTEM_ZLIB z REQUIRED)
	set(ZLIB_LIBRARY ${RINGPAD_SYSTEM_ZLIB} CACHE STRING "System zlib" FORCE)
endif()

add_subdirectory(${RINGPAD_FLYCAST_DIR} flycast)

# RingPad hooks into Flycast are applied to configured copies of individual
# source files, so the pinned checkout stays untouched. Each replacement must
# match exactly once, or configuration fails and the hook needs updating.
function(ringpad_patch_flycast_source rel old new prefix)
	set(src ${RINGPAD_FLYCAST_DIR}/${rel})
	file(READ ${src} text)
	string(FIND "${text}" "${old}" first)
	string(FIND "${text}" "${old}" last REVERSE)
	if(first EQUAL -1 OR NOT first EQUAL last)
		message(FATAL_ERROR "RingPad hook for ${rel} must match exactly once; update it for this Flycast pin")
	endif()
	string(REPLACE "${old}" "${new}" patched "${text}")
	get_filename_component(name ${rel} NAME)
	set(out ${CMAKE_BINARY_DIR}/ringpad/${name})
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
	get_filename_component(dir ${src} DIRECTORY)
	target_include_directories(flycast PRIVATE ${dir})
	target_sources(flycast PRIVATE ${out})
endfunction()

# Use the RingPad AOT executor in place of Flycast's SH-4 interpreter.
ringpad_patch_flycast_source(core/emulator.cpp
	"interpreter = Get_Sh4Interpreter();"
	"interpreter = RingPad_CreateSh4Executor();"
	"class Sh4Executor;\nSh4Executor *RingPad_CreateSh4Executor();\n")

if(IOS)
	# RingPad's own iOS shell replaces Flycast's GLKit shell: OpenGL ES is
	# deprecated and unavailable in the Apple Silicon Simulator, so iOS renders
	# with Flycast's Vulkan renderer on MoltenVK (Metal).
	get_target_property(_sp_sources flycast SOURCES)
	# Flycast's Images.xcassets goes too: its AppIcon would replace RingPad's.
	list(FILTER _sp_sources EXCLUDE REGEX "emulator-ios/emulator/(AppDelegate|FlycastViewController|PadViewController|EditPadViewController|EmulatorView|main\\.m|iCade-iOS/|Images\\.xcassets|.*\\.storyboard|.*\\.xib)")
	set_property(TARGET flycast PROPERTY SOURCES ${_sp_sources})
	set(_sp_ios_shell
		${CMAKE_CURRENT_SOURCE_DIR}/port/apple/ios/RingPadApp.mm
		${CMAKE_CURRENT_SOURCE_DIR}/port/apple/ios/SPDiagnostics.mm
		${CMAKE_CURRENT_SOURCE_DIR}/port/apple/ios/SPGameLibrary.mm
		${CMAKE_CURRENT_SOURCE_DIR}/port/apple/ios/SPPortMenu.mm)
	# The touch kit is its own library so other Dreamcast ports can reuse it.
	add_subdirectory(port/dcpad)
	target_link_libraries(flycast PRIVATE dcpad)
	target_sources(flycast PRIVATE ${_sp_ios_shell}
		${CMAKE_CURRENT_SOURCE_DIR}/port/apple/ios/Assets.xcassets
		${CMAKE_CURRENT_SOURCE_DIR}/port/input/FlycastPadBridge.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/port/apple/ios/SPFrameCapture.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/port/game/GameImport.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/port/game/VMUImage.cpp)
	target_include_directories(flycast PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/port/apple/ios
		${CMAKE_CURRENT_SOURCE_DIR}/port/input ${CMAKE_CURRENT_SOURCE_DIR}/port/game)
	set_source_files_properties(${_sp_ios_shell} PROPERTIES COMPILE_OPTIONS "-fobjc-arc")
	# The flycast target lives in Flycast's directory, so the property must be
	# set there for Xcode to compile the catalog into the app.
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/port/apple/ios/Assets.xcassets TARGET_DIRECTORY flycast PROPERTIES
		MACOSX_PACKAGE_LOCATION "Resources")
	# Every build is identifiable: version, commit count as the build number,
	# and the commit (with -dirty for uncommitted changes) in About and the log.
	set(RINGPAD_VERSION "0.1.0")
	execute_process(COMMAND git -C ${CMAKE_CURRENT_SOURCE_DIR} rev-list --count HEAD
		OUTPUT_VARIABLE _sp_build OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
	execute_process(COMMAND git -C ${CMAKE_CURRENT_SOURCE_DIR} describe --always --dirty --abbrev=7
		OUTPUT_VARIABLE _sp_commit OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
	if(NOT _sp_build)
		set(_sp_build 0)
		set(_sp_commit "unknown")
	endif()
	message(STATUS "RingPad ${RINGPAD_VERSION} build ${_sp_build} (${_sp_commit})")
	set_target_properties(flycast PROPERTIES
		OUTPUT_NAME "RingPad"
		MACOSX_BUNDLE_EXECUTABLE_NAME "RingPad"
		MACOSX_BUNDLE_GUI_IDENTIFIER "com.chrissotraidis.ringpad"
		MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/port/apple/ios/Info.plist.in
		MACOSX_BUNDLE_SHORT_VERSION_STRING "${RINGPAD_VERSION}"
		MACOSX_BUNDLE_BUNDLE_VERSION "${_sp_build}"
		MACOSX_BUNDLE_LONG_VERSION_STRING "${_sp_commit}"
		XCODE_ATTRIBUTE_MARKETING_VERSION "${RINGPAD_VERSION}"
		XCODE_ATTRIBUTE_CURRENT_PROJECT_VERSION "${_sp_build}"
		XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.chrissotraidis.ringpad"
		XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
		XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
		# Xcode 27's Swift compiler rejects Flycast's "5.0" language version.
		Swift_LANGUAGE_VERSION 5
		XCODE_ATTRIBUTE_SWIFT_VERSION 5)
	set_target_properties(AltKit PROPERTIES Swift_LANGUAGE_VERSION 5 XCODE_ATTRIBUTE_SWIFT_VERSION 5)

	# Flycast loads Vulkan with dlopen; RingPad links MoltenVK statically and
	# hands Flycast its entry point instead.
	ringpad_patch_flycast_source(core/rend/vulkan/vulkan_context.cpp
		"static vk::detail::DynamicLoader dl;"
		"static RingPadVkLoader dl;"
		"extern \"C\" void *RingPad_VkGetInstanceProcAddr();\nstruct RingPadVkLoader { template <typename T> T getProcAddress(const char *) const { return reinterpret_cast<T>(RingPad_VkGetInstanceProcAddr()); } };\n")
	set(RINGPAD_MOLTENVK_XCFRAMEWORK ${CMAKE_CURRENT_SOURCE_DIR}/ref/moltenvk-all/MoltenVK/MoltenVK/static/MoltenVK.xcframework
		CACHE PATH "MoltenVK v1.4.2 static xcframework")
	if(NOT EXISTS ${RINGPAD_MOLTENVK_XCFRAMEWORK})
		message(FATAL_ERROR "MoltenVK xcframework missing at ${RINGPAD_MOLTENVK_XCFRAMEWORK}; see docs/BUILDING.md")
	endif()
	target_link_libraries(flycast PRIVATE ${RINGPAD_MOLTENVK_XCFRAMEWORK}
		# Flycast's OpenGL renderer stays compiled in; RingPad renders with Vulkan.
		"-framework Metal" "-framework QuartzCore" "-framework IOSurface" "-framework CoreGraphics" "-framework OpenGLES"
		# Flycast's controller rumble uses Core Haptics, which this SDK does not auto-link.
		"-framework CoreHaptics" "-framework UniformTypeIdentifiers" "-framework GameController")
	# UI tests: real UIKit touches against the shell in the Simulator.
	find_package(XCTest REQUIRED)
	xctest_add_bundle(RingPadUITests flycast ${CMAKE_CURRENT_SOURCE_DIR}/tests/ios/RingPadUITests.m
		${CMAKE_CURRENT_SOURCE_DIR}/tests/ios/DCPadCaptureTests.m
		${CMAKE_CURRENT_SOURCE_DIR}/tests/ios/DCPadPlayTests.mm)
	set_target_properties(RingPadUITests PROPERTIES
		XCODE_PRODUCT_TYPE com.apple.product-type.bundle.ui-testing
		XCODE_ATTRIBUTE_TEST_TARGET_NAME flycast
		XCODE_ATTRIBUTE_TEST_HOST ""
		XCODE_ATTRIBUTE_BUNDLE_LOADER ""
		XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.chrissotraidis.ringpad.uitests"
		XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
		XCODE_ATTRIBUTE_GENERATE_INFOPLIST_FILE YES)
	target_link_libraries(RingPadUITests PRIVATE "-framework CoreGraphics" "-framework UIKit")
endif()

file(GLOB _sp_generated CONFIGURE_DEPENDS ${RINGPAD_RECOMP_DIR}/*.cpp)
if(NOT _sp_generated)
	set(_sp_generated ${CMAKE_CURRENT_SOURCE_DIR}/port/recomp/sp_aot_index_empty.cpp)
endif()
list(LENGTH _sp_generated _sp_generated_count)
message(STATUS "RingPad AOT sources: ${_sp_generated_count}")

target_sources(flycast PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/port/recomp/sp_aot_executor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/port/game/GameCheats.cpp ${_sp_generated})
target_include_directories(flycast PRIVATE ${RINGPAD_FLYCAST_DIR}/core ${CMAKE_CURRENT_SOURCE_DIR}/port/recomp
	${CMAKE_CURRENT_SOURCE_DIR}/port/game)
target_compile_definitions(flycast PRIVATE TARGET_NO_REC)
set_source_files_properties(${_sp_generated} PROPERTIES COMPILE_OPTIONS "-O2;-Wno-unused-function")
