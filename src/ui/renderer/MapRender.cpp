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
        
    if (!mapData.aboveObject.data.empty())
        drawLayer(mapData.aboveObject);
}

void MapRender::renderAboveLayer() {
    if (!mapData.aboveLayer.data.empty())
        drawLayer(mapData.aboveLayer);
}

void MapRender::drawLayer(const TileLayer& layer) {
    for (int y = 0; y < layer.height; ++y) {
        for (int x = 0; x < layer.width; ++x) {
            int index = y * layer.width + x;
            int tileID = layer.data[index];

            if (tileID == 0) continue;

            const Tileset* tileset = nullptr;
            int localID = 0;

            for (int i = static_cast<int>(mapData.tilesets.size()) - 1; i >= 0; --i) {
                if (tileID >= mapData.tilesets[i].firstgid) {
                    tileset = &mapData.tilesets[i];
                    localID = tileID - mapData.tilesets[i].firstgid;
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
            SDL_RenderCopy(renderer, tileset->texture, &srcRect, &camDes);
        }
    }
}