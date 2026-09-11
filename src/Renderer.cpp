#include "Renderer.hpp"
#include "Grid.hpp"

Renderer::Renderer(SDL_Renderer *renderer)
    : m_renderer(renderer)
{
}

void Renderer::render(const Grid &grid, int offsetX, int offsetY, bool showGrid, bool showPossibilities)
{
    const int cellSize = 10;
    const Scope &scope = grid.getScope();

    for (int y = 0; y < grid.getHeight(); ++y)
    {
        for (int x = 0; x < grid.getWidth(); ++x)
        {
            SDL_FRect rect{
                static_cast<float>(offsetX + x * cellSize),
                static_cast<float>(offsetY + y * cellSize),
                static_cast<float>(cellSize),
                static_cast<float>(cellSize)};

            // Couleur de la cellule
            switch (grid.get(x, y).tile)
            {
            case Tile::Unknown:
                SDL_SetRenderDrawColor(m_renderer, 40, 40, 40, 255);
                break;

            case Tile::Water:
                SDL_SetRenderDrawColor(m_renderer, 40, 100, 180, 255);
                break;

            case Tile::Sand:
                SDL_SetRenderDrawColor(m_renderer, 220, 190, 120, 255);
                break;

            case Tile::Grass:
                SDL_SetRenderDrawColor(m_renderer, 70, 160, 70, 255);
                break;

            case Tile::Forest:
                SDL_SetRenderDrawColor(m_renderer, 30, 110, 50, 255);
                break;

            case Tile::Rock:
                SDL_SetRenderDrawColor(m_renderer, 100, 100, 100, 255);
                break;

            case Tile::Snow:
                SDL_SetRenderDrawColor(m_renderer, 230, 230, 240, 255);
                break;

            case Tile::Lava:
                SDL_SetRenderDrawColor(m_renderer, 220, 70, 30, 255);
                break;

            case Tile::Contradiction:
                SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
                break;

            case Tile::Boundary:
                SDL_SetRenderDrawColor(m_renderer, 80, 127, 128, 255);
                break;
            }

            SDL_RenderFillRect(m_renderer, &rect);

            // Grille uniquement dans le scope
            if (showGrid &&
                x >= scope.x &&
                x < scope.x + scope.width &&
                y >= scope.y &&
                y < scope.y + scope.height)
            {
                SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
                SDL_RenderRect(m_renderer, &rect);
            }
        }
    }
}
