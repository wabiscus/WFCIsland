#pragma once

#include <SDL3/SDL.h>

class Grid;

class Renderer
{
public:
    explicit Renderer(SDL_Renderer* renderer);

    void render(const Grid& grid, int offsetX, int offsetY);

private:
    SDL_Renderer* m_renderer;
};
