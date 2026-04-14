cmake_minimum_required(VERSION 3.20)

project(GW2_GolemRespawn_Addon)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS ON)

include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.80
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(nlohmann_json)

add_library(third_party_libs STATIC)
target_include_directories(third_party_libs PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
file(GLOB IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/*.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
)
target_sources(third_party_libs PRIVATE ${IMGUI_SOURCES})

add_library(nexus INTERFACE
    "nexus/Nexus.h"
)
add_library(mumble INTERFACE
    "mumble/Mumble.h"
)
add_library(rtapi INTERFACE
    "rtapi/RTAPI.hpp"
)
add_library(arcdps INTERFACE
    "arcdps/ArcDPS.h"
)

target_link_libraries(third_party_libs PRIVATE
    d3d11
    d3dcompiler
    comdlg32
    nexus
    mumble
    rtapi
    arcdps
    nlohmann_json::nlohmann_json
)
target_include_directories(third_party_libs PUBLIC
    "."
)

add_library(first_party_libs STATIC
    "src/Shared.cpp"
    "src/Settings.cpp"
    "src/Render.cpp"
    "src/Textures.cpp"
    "src/MumbleUtils.cpp"
    "src/Render.cpp"
    "src/TypesUtils.cpp"
)
target_include_directories(first_party_libs PUBLIC
    "src"
    "resources"
)
target_link_libraries(first_party_libs PRIVATE
    third_party_libs
)

add_library(GW2_GolemRespawn SHARED
    "src/entry.cpp"
)
target_link_libraries(GW2_GolemRespawn PRIVATE
    third_party_libs
    first_party_libs
)
set_target_properties(GW2_GolemRespawn PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release"
)
