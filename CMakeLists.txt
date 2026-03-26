cmake_minimum_required(VERSION 3.20)
project(RollForge VERSION 0.1.0 LANGUAGES CXX)

# Set the standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find vcpkg dependencies
find_package(nlohmann_json CONFIG REQUIRED)

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

# Link JSON library
target_link_libraries(rollforge_core PUBLIC nlohmann_json::nlohmann_json)

# 2. Python bindings module (optional — requires pybind11)
option(BUILD_PYTHON_BINDINGS "Build the Python bindings module" OFF)

if(BUILD_PYTHON_BINDINGS)
    find_package(pybind11 CONFIG QUIET)
    if(NOT pybind11_FOUND)
        # Fallback: fetch pybind11 via FetchContent
        include(FetchContent)
        FetchContent_Declare(
            pybind11
            GIT_REPOSITORY https://github.com/pybind/pybind11.git
            GIT_TAG        v2.13.6
        )
        FetchContent_MakeAvailable(pybind11)
    endif()

    pybind11_add_module(rollforge bindings/bindings.cpp)
    target_link_libraries(rollforge PRIVATE rollforge_core)
endif()

# 3. Enable testing
enable_testing()
add_subdirectory(tests)