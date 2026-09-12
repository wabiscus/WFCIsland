cmake_minimum_required(VERSION 3.25)

project(WFC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.x
)

FetchContent_MakeAvailable(SDL3)

set(SDLIMAGE_PNG ON CACHE BOOL "Enable PNG support" FORCE)

set(SDLIMAGE_AVIF OFF CACHE BOOL "Disable AVIF support" FORCE)
set(SDLIMAGE_JXL OFF CACHE BOOL "Disable JPEG-XL support" FORCE)
set(SDLIMAGE_WEBP OFF CACHE BOOL "Disable WebP support" FORCE)
set(SDLIMAGE_TIF OFF CACHE BOOL "Disable TIFF support" FORCE)
set(SDLIMAGE_JPG OFF CACHE BOOL "Disable JPEG support" FORCE)

FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.2.0
)

FetchContent_MakeAvailable(SDL3_image)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

add_executable(
    WFC

    src/main.cpp
    src/Grid.cpp
    src/WFC.cpp
    src/Renderer.cpp
    src/UI.cpp
    src/ShapeGenerator.cpp
    src/Ruleset.cpp
    src/App.cpp
    src/Utils.cpp

    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_sdl3.cpp
    external/imgui/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(
    WFC PRIVATE

    include

    external/imgui
    external/imgui/backends
)

target_link_libraries(
    WFC PRIVATE

    SDL3::SDL3
    SDL3_image::SDL3_image
    nlohmann_json::nlohmann_json
)

add_custom_command(TARGET WFC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:WFC>
)

add_custom_command(TARGET WFC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3_image::SDL3_image>
        $<TARGET_FILE_DIR:WFC>
)

add_custom_command(TARGET WFC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/rulesets
        $<TARGET_FILE_DIR:WFC>/rulesets
)