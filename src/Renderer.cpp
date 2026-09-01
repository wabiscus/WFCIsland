#include "Renderer.hpp"
#include "Grid.hpp"

Renderer::Renderer(SDL_Renderer* renderer)
    : m_renderer(renderer)
{
}

void Renderer::render(const Grid& grid, int offsetX, int offsetY)
{
    const int cellSize = 10;

    for (int y = 0; y < grid.getHeight(); ++y)
    {
        for (int x = 0; x < grid.getWidth(); ++x)
        {
            SDL_FRect rect{
                static_cast<float>(offsetX + x * cellSize),
                static_cast<float>(offsetY + y * cellSize),
                static_cast<float>(cellSize),
                static_cast<float>(cellSize)
            };

            switch (grid.get(x, y))
            {
                case Tile::Water:
                    SDL_SetRenderDrawColor(m_renderer, 40, 100, 180, 255);
                    break;

                case Tile::Sand:
                    SDL_SetRenderDrawColor(m_renderer, 220, 190, 120, 255);
                    break;

                case Tile::Grass:
                    SDL_SetRenderDrawColor(m_renderer, 70, 160, 70, 255);
                    break;
            }

            SDL_RenderFillRect(m_renderer, &rect);
        }
    }
}
