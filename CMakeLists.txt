cmake_minimum_required(VERSION 3.20)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE PATH "vcpkg toolchain")
endif()

project(RollForge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(nlohmann_json CONFIG REQUIRED)
add_library(rollforge_core STATIC
    src/action_engine.cpp
    src/entity.cpp
    src/mechanics.cpp
    src/session_state.cpp
)

target_include_directories(rollforge_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(rollforge_core PUBLIC nlohmann_json::nlohmann_json)

option(BUILD_PYTHON_BINDINGS "Build the Python bindings module" OFF)

if(SKBUILD OR BUILD_PYTHON_BINDINGS)
    find_package(pybind11 CONFIG QUIET)
    if(NOT pybind11_FOUND)
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

    install(TARGETS rollforge DESTINATION .)
endif()

enable_testing()
add_subdirectory(tests)