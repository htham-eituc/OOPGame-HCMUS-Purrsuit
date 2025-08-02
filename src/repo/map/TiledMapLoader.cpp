#include <iostream>
#include <SDL_image.h>
#include "TiledMapLoader.h"
#include "Map.h"
#include "Item.h"
#include "Services.h"
#include "tileson.hpp"

SDL_Texture* TiledMapLoader::loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
    if (!tex) {
        std::cerr << "Failed to load texture: " << path << " - " << IMG_GetError() << std::endl;
    }
    return tex;
}

MapData TiledMapLoader::loadMap(const std::string& path, SDL_Renderer* renderer) {
    MapData mapData;
    tson::Tileson parser;
    std::unique_ptr<tson::Map> map = parser.parse(fs::path(path));

    if (map->getStatus() != tson::ParseStatus::OK) {
        std::cerr << "Failed to parse map: " << path << std::endl;
        return mapData;
    }

    mapData.mapWidth = map->getSize().x;
    mapData.mapHeight = map->getSize().y;
    mapData.tileWidth = map->getTileSize().x;
    mapData.tileHeight = map->getTileSize().y;

    // Load tileset
    for (const auto& ts : map->getTilesets()) {
        Tileset tileset;
        tileset.firstgid = ts.getFirstgid();
        tileset.tileWidth = ts.getTileSize().x;
        tileset.tileHeight = ts.getTileSize().y;
        tileset.columns = ts.getColumns();
        tileset.tilecount = ts.getTileCount();

        std::string imgPath = "assets/tiles/" + ts.getImagePath().u8string();
        std::string imageId = ts.getImagePath().filename().string();
        core::textures->loadTexture(imageId, imgPath);
        tileset.texture = core::textures->getTexture(imageId);

        mapData.tilesets.push_back(tileset);
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
            mapData.collisionMap.resize(mapData.mapHeight, std::vector<bool>(mapData.mapWidth, false));

            for (int y = 0; y < mapData.mapHeight; ++y) {
                for (int x = 0; x < mapData.mapWidth; ++x) {
                    int index = y * tileLayer.width + x;
                    int gid = tileLayer.data[index];  
                    if (gid != 0) {
                        mapData.collisionMap[y][x] = true;  
                    }
                }
            }
        }
        if (tileLayer.name == "Above")
            mapData.aboveLayer = tileLayer;
        else if (tileLayer.name == "Above Object")
            mapData.aboveObject = tileLayer;
        else 
            mapData.layers.push_back(tileLayer);
    }

    mapData.items = LoadItemsFromMap(path);
    mapData.spawnPoints = LoadSpawnPointsFromMap(path);
    mapData.transitionZones = LoadTransitionZonesFromMap(path);
    return mapData;
}

std::vector<Item> TiledMapLoader::LoadItemsFromMap(const std::string& path) {
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

SpawnPoints TiledMapLoader::LoadSpawnPointsFromMap(const std::string& path) {
    SpawnPoints spawnPoints;

    tson::Tileson parser;
    std::unique_ptr<tson::Map> map = parser.parse(fs::path(path));
    if (map->getStatus() != tson::ParseStatus::OK) {
        std::cerr << "Failed to parse map for spawn points: " << path << std::endl;
        return spawnPoints;
    }

    tson::Layer* objectLayer = map->getLayer("Spawning");
    if (!objectLayer || objectLayer->getType() != tson::LayerType::ObjectGroup) {
        std::cerr << "No valid 'Spawning' layer found in map: " << path << std::endl;
        return spawnPoints;
    }

    for (const auto& obj : objectLayer->getObjects()) {
        std::string name = obj.getName();  // Expected: "player", "princess", "zombie"
        tson::Vector2i pos = obj.getPosition();
        Vector2 position{ static_cast<float>(pos.x), static_cast<float>(pos.y) };

        if (name == "player") {
            spawnPoints.playerSpawn = position;
        } else if (name == "princess") {
            spawnPoints.princessSpawn = position;
        } else if (name == "zombie") {
            spawnPoints.zombieSpawns.push_back(position);
        }
    }

    return spawnPoints;
}

std::vector<TransitionZone> TiledMapLoader::LoadTransitionZonesFromMap(const std::string& path) {
    std::vector<TransitionZone> zones;

    tson::Tileson parser;
    std::unique_ptr<tson::Map> map = parser.parse(fs::path(path));

    if (auto* layer = map->getLayer("Transitions"); layer && layer->getType() == tson::LayerType::ObjectGroup) {
        for (auto& obj : layer->getObjects()) {
            if (obj.getObjectType() == tson::ObjectType::Rectangle) {
                SDL_Rect rect = {
                    static_cast<int>(obj.getPosition().x),
                    static_cast<int>(obj.getPosition().y),
                    static_cast<int>(obj.getSize().x),
                    static_cast<int>(obj.getSize().y)
                };

                std::string zoneName = obj.getName();

                std::string requiredItem;
                std::string instruction;
                std::string entering;

                auto* itemProp = obj.getProp("Item");
                if (itemProp) {
                    requiredItem = itemProp->getValue<std::string>();
                }
                auto* textProp = obj.getProp("Instruction");
                if (textProp) {
                    instruction = textProp->getValue<std::string>();
                }
                auto* enterProp = obj.getProp("Entering");
                if (enterProp) {
                    entering = enterProp->getValue<std::string>();
                }

                zones.push_back({rect, zoneName, instruction, entering, requiredItem});
            }
        }
    }

    return zones;
}


