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

add_executable(
    WFC

    src/main.cpp
    src/App.cpp
    src/Grid.cpp
    src/WFC.cpp
    src/Renderer.cpp
    src/UI.cpp

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
)

add_custom_command(TARGET WFC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:WFC>
)