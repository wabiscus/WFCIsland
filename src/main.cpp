#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL3/SDL.h>
#include <iostream>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "Grid.hpp"
#include "Renderer.hpp"
#include "UI.hpp"
#include "WFC.hpp"
#include "ShapeGenerator.hpp"
#include "App.hpp"

SDL_Window* window = nullptr;
SDL_Renderer* sdlRenderer = nullptr;

Renderer* renderer = nullptr;
Ruleset* ruleset = nullptr;
Grid* grid = nullptr;
WFC* wfc = nullptr;
UI* UILeft = nullptr;
UI* UIRight = nullptr;
ShapeGenerator* shapegen = nullptr;
App* app = nullptr;

bool running = true;

void mainLoop()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);

        if (event.type == SDL_EVENT_QUIT)
            running = false;
    }

    if (!running)
    {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    SDL_SetRenderDrawColor(sdlRenderer, 20, 20, 20, 255);
    SDL_RenderClear(sdlRenderer);

    app->update();

    app->render(
        280,
        0,
        UILeft->isGridVisible(),
        UILeft->arePossibilitiesVisible()
    );

    UILeft->render(*app);
    UIRight->render(*app);

    ImGui::Render();

    ImGui_ImplSDLRenderer3_RenderDrawData(
        ImGui::GetDrawData(),
        sdlRenderer
    );

    SDL_RenderPresent(sdlRenderer);
}

int main()
{
    std::cerr << "=== MAIN START ===" << std::endl;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    std::cerr << "=== SDL INIT OK ===" << std::endl;

    if (!SDL_CreateWindowAndRenderer(
            "WFC",
            1280,
            720,
            0,
            &window,
            &sdlRenderer))
    {
        SDL_Log(
            "SDL_CreateWindowAndRenderer failed: %s",
            SDL_GetError()
        );

        SDL_Quit();
        return 1;
    }

    std::cerr << "=== WINDOW OK ===" << std::endl;

    renderer = new Renderer(sdlRenderer);
    ruleset = new Ruleset(RulesetType::Tropical);
    grid = new Grid(72, 72);

    wfc = new WFC(*grid, *ruleset);

    std::cerr << "=== BEFORE WFC INIT ===" << std::endl;

    wfc->initialize();

    UILeft = new UI(UIPanel::Left);
    UIRight = new UI(UIPanel::Right);

    shapegen = new ShapeGenerator(*grid, *ruleset);

    std::cerr << "=== BEFORE APP ===" << std::endl;

    app = new App(
        *grid,
        *ruleset,
        *shapegen,
        *wfc,
        *renderer
    );

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(
        window,
        sdlRenderer
    );

    ImGui_ImplSDLRenderer3_Init(sdlRenderer);

    std::cerr << "=== INIT COMPLETE ===" << std::endl;

#ifdef __EMSCRIPTEN__

    emscripten_set_main_loop(
        mainLoop,
        0,
        true
    );

#else

    while (running)
    {
        mainLoop();
    }

#endif

    return 0;
}