#include <SDL3/SDL.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "Grid.hpp"
#include "Renderer.hpp"
#include "UI.hpp"
#include "WFC.hpp"

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = nullptr;
    SDL_Renderer *sdlRenderer = nullptr;

    SDL_CreateWindowAndRenderer(
        "WFC",
        1280,
        720,
        0,
        &window,
        &sdlRenderer);

    Renderer renderer(sdlRenderer);
    Grid grid(103, 72);
    WFC wfc(grid);
    UI ui;

    if (!window)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // --------------------------------------------------
    // ImGui
    // --------------------------------------------------

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(window, sdlRenderer);
    ImGui_ImplSDLRenderer3_Init(sdlRenderer);

    // --------------------------------------------------
    // Boucle principale
    // --------------------------------------------------

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        // Nouvelle frame ImGui
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // --------------------------------------------------
        // Interface
        // --------------------------------------------------
        SDL_SetRenderDrawColor(sdlRenderer, 20, 20, 20, 255);
        SDL_RenderClear(sdlRenderer);

        renderer.render(grid, 250, 0);

        // ImGui ici si nécessaire
        ui.render(grid, wfc);

        // --------------------------------------------------
        // Rendu
        // --------------------------------------------------

        ImGui::Render();

        ImGui_ImplSDLRenderer3_RenderDrawData(
            ImGui::GetDrawData(),
            sdlRenderer);

        SDL_RenderPresent(sdlRenderer);
    }

    // --------------------------------------------------
    // Nettoyage
    // --------------------------------------------------

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();

    ImGui::DestroyContext();

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}