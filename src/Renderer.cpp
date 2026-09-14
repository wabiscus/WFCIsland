#include "Renderer.hpp"
#include "Grid.hpp"
#include "Utils.hpp"
#include <SDL3_image/SDL_image.h>

Renderer::Renderer(SDL_Renderer *renderer)
    : m_renderer(renderer)
{
}

static SDL_Color tileToColorExport(Tile tile)
{
    switch (tile)
    {
    case Tile::Water:
        return {50, 120, 200, 255};

    case Tile::Sand:
        return {220, 190, 100, 255};

    case Tile::Grass:
        return {80, 160, 70, 255};

    case Tile::Forest:
        return {40, 100, 50, 255};

    case Tile::Rock:
        return {120, 120, 120, 255};

    case Tile::Snow:
        return {240, 240, 240, 255};

    case Tile::Lava:
        return {220, 60, 30, 255};

    case Tile::Boundary:
        return {0, 0, 0, 255};

    case Tile::Unknown:
        return {0, 0, 0, 255};

    case Tile::Contradiction:
        return {255, 0, 255, 255};
    }

    return {0, 0, 0, 255};
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

std::vector<Uint8> Renderer::exportImage(
    const Grid &grid,
    int scale)
{
    const int width = grid.getWidth() * scale;
    const int height = grid.getHeight() * scale;

    SDL_Surface *surface = SDL_CreateSurface(
        width,
        height,
        SDL_PIXELFORMAT_RGBA32);

    if (surface == nullptr)
        return {};

    for (int y = 0; y < grid.getHeight(); ++y)
    {
        for (int x = 0; x < grid.getWidth(); ++x)
        {
            const Tile tile = grid.get(x, y).tile;
            const ImU32 color = tileToColor(tile);

            const Uint8 r = (color >> 0) & 0xFF;
            const Uint8 g = (color >> 8) & 0xFF;
            const Uint8 b = (color >> 16) & 0xFF;
            const Uint8 a = (color >> 24) & 0xFF;

            for (int pixelY = 0; pixelY < scale; ++pixelY)
            {
                for (int pixelX = 0; pixelX < scale; ++pixelX)
                {
                    const int imageX = x * scale + pixelX;
                    const int imageY = y * scale + pixelY;

                    if (!SDL_WriteSurfacePixel(
                            surface,
                            imageX,
                            imageY,
                            r,
                            g,
                            b,
                            a))
                    {
                        SDL_DestroySurface(surface);
                        return {};
                    }
                }
            }
        }
    }

    SDL_IOStream *io = SDL_IOFromDynamicMem();

    if (io == nullptr)
    {
        SDL_DestroySurface(surface);
        return {};
    }

    if (!IMG_SavePNG_IO(surface, io, false))
    {
        SDL_CloseIO(io);
        SDL_DestroySurface(surface);
        return {};
    }

    const Sint64 size = SDL_GetIOSize(io);

    if (size <= 0)
    {
        SDL_CloseIO(io);
        SDL_DestroySurface(surface);
        return {};
    }

    SDL_SeekIO(io, 0, SDL_IO_SEEK_SET);

    std::vector<Uint8> data(static_cast<std::size_t>(size));

    const std::size_t bytesRead = SDL_ReadIO(
        io,
        data.data(),
        data.size());

    SDL_CloseIO(io);
    SDL_DestroySurface(surface);

    if (bytesRead != data.size())
        return {};

    return data;
}