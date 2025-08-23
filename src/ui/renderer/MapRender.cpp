#include "MapRender.h"
#include "MemoryUtils.h"
#include "Constants.h"
#include "Camera.h"

MapRender::MapRender(SDL_Renderer* renderer)
    : renderer(renderer) {}

MapRender::~MapRender() {
    for (auto& ts : mapData.tilesets) {
        safeDestroyTexture(ts.texture);
    }
}

void MapRender::render() {
    for (const auto& layer : mapData.layers)
        drawLayer(layer);

    for (auto& item : mapData.items)
        item.render(renderer, mapData.tilesets);
        
    if (!mapData.aboveObject.tiles.empty())
        drawLayer(mapData.aboveObject);
}

void MapRender::renderAboveLayer() {
    if (!mapData.aboveLayer.tiles.empty())
        drawLayer(mapData.aboveLayer);
}

void MapRender::drawLayer(const TileLayer& layer) {
    for (int y = 0; y < layer.height; ++y) {
        for (int x = 0; x < layer.width; ++x) {
            int index = y * layer.width + x;
            const Tile& tile = layer.tiles[index];

            if (tile.gid == 0) continue; // empty

            const Tileset* tileset = nullptr;
            int localID = 0;

            for (int i = static_cast<int>(mapData.tilesets.size()) - 1; i >= 0; --i) {
                if (tile.gid >= mapData.tilesets[i].firstgid) {
                    tileset = &mapData.tilesets[i];
                    localID = tile.gid - mapData.tilesets[i].firstgid;
                    break;
                }
            }

            if (!tileset || !tileset->texture) continue;

            SDL_Rect srcRect = {
                (localID % tileset->columns) * tileset->tileWidth,
                (localID / tileset->columns) * tileset->tileHeight,
                tileset->tileWidth,
                tileset->tileHeight
            };

            SDL_Rect destRect = {
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };

            SDL_Rect camDes = Camera::ToCamView(destRect);

            // 🚀 Handle flipping
            SDL_RendererFlip flip = SDL_FLIP_NONE;
            if (tile.flipH) flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
            if (tile.flipV) flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);

            // ⚠ SDL2 cannot do diagonal flip directly, you’ll need rotation
            double angle = tile.flipD ? 90.0 : 0.0;

            SDL_RenderCopyEx(renderer, tileset->texture, &srcRect, &camDes, angle, nullptr, flip);
        }
    }
}