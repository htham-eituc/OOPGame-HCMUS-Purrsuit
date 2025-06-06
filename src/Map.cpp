#include "Map.h"
#include "tileson.hpp"
#include <SDL_image.h>
#include <iostream>

Map::Map(SDL_Renderer* renderer) : renderer(renderer) {
    loadMap("assets/maps/map2.json");
}

Map::~Map() {
    for (auto& ts : tilesets) {
        if (ts.texture) SDL_DestroyTexture(ts.texture);
    }
}

SDL_Texture* Map::loadTexture(const std::string& path) {
    SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
    if (!tex) {
        std::cerr << "Failed to load texture: " << path << " - " << IMG_GetError() << std::endl;
    }
    return tex;
}

void Map::loadMap(const std::string& path) {
    tson::Tileson parser;
    std::unique_ptr<tson::Map> map = parser.parse(fs::path(path));

    if (map->getStatus() != tson::ParseStatus::OK) {
        std::cerr << "Failed to parse map: " << path << std::endl;
        return;
    }

    mapWidth = map->getSize().x;
    mapHeight = map->getSize().y;
    tileWidth = map->getTileSize().x;
    tileHeight = map->getTileSize().y;

    // Load tilesets
    for (const auto& ts : map->getTilesets()) {
        Tileset tileset;
        tileset.firstgid = ts.getFirstgid();
        tileset.tileWidth = ts.getTileSize().x;
        tileset.tileHeight = ts.getTileSize().y;
        tileset.columns = ts.getColumns();
        tileset.tilecount = ts.getTileCount();

        std::string imgPath = "assets/tiles/" + ts.getImagePath().u8string();
        tileset.texture = loadTexture(imgPath);

        tilesets.push_back(tileset);
    }

    // Load tile layers
    for (auto& layer : map->getLayers()) {
        if (layer.getType() != tson::LayerType::TileLayer || !layer.isVisible()) continue;

        TileLayer tileLayer;
        tileLayer.name = layer.getName();
        tileLayer.width = layer.getSize().x;
        tileLayer.height = layer.getSize().y;

        auto& data = layer.getData();
        tileLayer.data.assign(data.begin(), data.end());
        if (tileLayer.name == "Collision") {
            collisionMap.resize(mapHeight, std::vector<bool>(mapWidth, false));

            for (int y = 0; y < mapHeight; ++y) {
                for (int x = 0; x < mapWidth; ++x) {
                    int index = y * tileLayer.width + x;
                    int gid = tileLayer.data[index];  
                    if (gid != 0) {
                        collisionMap[y][x] = true;  
                    }
                }
            }
        }
        if (tileLayer.name == "Above")
            aboveLayer = tileLayer;
        else 
            layers.push_back(tileLayer);
    }

    items = LoadItemsFromMap(path);
}

std::vector<Item> Map::LoadItemsFromMap(const std::string& path) {
    tson::Tileson parser;
    std::unique_ptr<tson::Map> map = parser.parse(fs::path(path));

    std::vector<Item> items;

    if (map->getStatus() != tson::ParseStatus::OK) {
        std::cerr << "Failed to load map: " << path << "\n";
        return items;
    }

    tson::Layer* objectLayer = map->getLayer("Object");

    if (objectLayer && objectLayer->getType() == tson::LayerType::ObjectGroup) {
        for (auto& obj : objectLayer->getObjects()) {
            std::string name = obj.getName();
            auto pos = obj.getPosition(); 
            auto size = obj.getSize();   
            int gid = obj.getGid();
            int SCALE = TILE_SIZE/size.x;
            
            SDL_Rect rect = {
                static_cast<int>(pos.x * SCALE),
                static_cast<int>(pos.y * SCALE),
                static_cast<int>(size.x * SCALE),
                static_cast<int>(size.y * SCALE)
            };

            items.emplace_back(name, rect, gid);
        }
    }

    return items;
}

void Map::render() {
    for (const auto& layer : layers)
        drawLayer(layer);
        
    for (auto& item : items) 
        item.render(renderer, tilesets);
}

void Map::renderAboveLayer() {
    if (!aboveLayer.data.empty())
        drawLayer(aboveLayer);
}

void Map::drawLayer(const TileLayer& layer) {
    for (int y = 0; y < layer.height; ++y) {
        for (int x = 0; x < layer.width; ++x) {
            int index = y * layer.width + x;
            int tileID = layer.data[index];

            if (tileID == 0) continue;

            const Tileset* tileset = nullptr;
            int localID = 0;

            for (int i = static_cast<int>(tilesets.size()) - 1; i >= 0; --i) {
                if (tileID >= tilesets[i].firstgid) {
                    tileset = &tilesets[i];
                    localID = tileID - tilesets[i].firstgid;
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
    if (y < 0 || y >= mapHeight || x < 0 || x >= mapWidth)
        return true; // Out of bounds is collidable
    return collisionMap[y][x];
}

std::vector<Item>& Map::getItems() { 
    return items; 
}