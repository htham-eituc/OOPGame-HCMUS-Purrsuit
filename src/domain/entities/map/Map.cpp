#include <SDL_image.h>
#include <iostream>
#include "Map.h"
#include "MemoryUtils.h"
#include "Item.h"

Map::Map(SDL_Renderer* renderer) : renderer(renderer) {}

Map::~Map() {
    for (auto& ts : mapData.tilesets) 
        safeDestroyTexture(ts.texture);
}

void Map::loadFromData(const MapData &data) {
    mapData = data;
}

void Map::render()
{
    for (const auto& layer : mapData.layers)
        drawLayer(layer);
        
    for (auto& item : mapData.items) 
        item.render(renderer, mapData.tilesets);
}

void Map::renderAboveLayer() {
    if (!mapData.aboveLayer.data.empty())
        drawLayer(mapData.aboveLayer);
}

void Map::drawLayer(const TileLayer& layer) {
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

            SDL_RenderCopy(renderer, tileset->texture, &srcRect, &destRect);
        }
    }
}

bool Map::checkCollision(const SDL_Rect& box) const {
    int tileSize = TILE_SIZE;

    int leftTile   = box.x / tileSize;
    int rightTile  = (box.x + box.w - 1) / tileSize;
    int topTile    = box.y / tileSize;
    int bottomTile = (box.y + box.h - 1) / tileSize;

    for (int y = topTile; y <= bottomTile; ++y) {
        for (int x = leftTile; x <= rightTile; ++x) {
            if (isCollidable(x, y)) return true;
        }
    }

    return false;
}

bool Map::isCollidable(int x, int y) const {
    if (y < 0 || y >= mapData.mapHeight || x < 0 || x >= mapData.mapWidth)
        return true; // Out of bounds
    return mapData.collisionMap[y][x];
}

std::vector<Item>& Map::getItems() { 
    return mapData.items; 
}