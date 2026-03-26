cmake_minimum_required(VERSION 3.20)
project(RollForge VERSION 0.1.0 LANGUAGES CXX)

# Set the standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find vcpkg dependencies
find_package(nlohmann_json CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# 1. Define the RollForge core static library
add_library(rollforge_core STATIC
    src/action_engine.cpp
    src/entity.cpp
    src/mechanics.cpp
    src/session_state.cpp
)

target_include_directories(rollforge_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Link JSON and the experimental library for std::println support
target_link_libraries(rollforge_core PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(rollforge_core PRIVATE stdc++exp)

# 2. Define the Python bindings module
pybind11_add_module(rollforge bindings/bindings.cpp)

# Link the C++ core library and the experimental library to the Python module
target_link_libraries(rollforge PRIVATE rollforge_core stdc++exp)

# 3. Enable testing
enable_testing()
add_subdirectory(tests)