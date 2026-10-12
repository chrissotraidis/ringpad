# DCPad: reusable Dreamcast touch controls for Apple ports.
#
#   add_subdirectory(path/to/dcpad)
#   target_link_libraries(my_app PRIVATE dcpad)
#
# The core (layout and touch engine) builds everywhere and is host-tested; the
# UIKit overlay, settings store and game-controller input are added on iOS.
# DCPad publishes one Dreamcast controller state through the input mixer in
# DCPAD_INPUT_DIR; a port adapts that state to its own runtime.
set(DCPAD_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../input CACHE PATH "Dreamcast input state, mixer and mapping sources")

add_library(dcpad STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/core/DCPad.cpp
	${DCPAD_INPUT_DIR}/DreamcastInputAdapter.cpp
	${DCPAD_INPUT_DIR}/DreamcastInputMixer.cpp
	${DCPAD_INPUT_DIR}/DreamcastControllerMapping.cpp)
target_include_directories(dcpad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core ${DCPAD_INPUT_DIR})
target_compile_features(dcpad PUBLIC cxx_std_17)

if(IOS)
	set(_dcpad_apple
		${CMAKE_CURRENT_SOURCE_DIR}/apple/DCPadOverlayView.mm
		${CMAKE_CURRENT_SOURCE_DIR}/apple/DCPadSettingsStore.mm
		${CMAKE_CURRENT_SOURCE_DIR}/apple/DCPadControllerInput.mm)
	target_sources(dcpad PRIVATE ${_dcpad_apple})
	set_source_files_properties(${_dcpad_apple} PROPERTIES COMPILE_OPTIONS "-fobjc-arc")
	target_include_directories(dcpad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/apple)
	target_link_libraries(dcpad PUBLIC "-framework UIKit" "-framework GameController")
endif()
