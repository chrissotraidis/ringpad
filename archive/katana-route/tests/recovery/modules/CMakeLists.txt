cmake_minimum_required(VERSION 3.20)
project(RingPadModuleOracle LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
add_executable(recovery_oracle ../module-execution.cpp
    "${FIXTURE_DIR}/a/dc_runtime.cpp" "${FIXTURE_DIR}/a/dc_arm7.cpp"
    "${FIXTURE_DIR}/a/dc_image.cpp" "${FIXTURE_DIR}/a/dc_native_overrides.cpp")
target_include_directories(recovery_oracle PRIVATE "${FIXTURE_DIR}" "${FIXTURE_DIR}/a")
include("${FIXTURE_DIR}/module-sources.cmake")
