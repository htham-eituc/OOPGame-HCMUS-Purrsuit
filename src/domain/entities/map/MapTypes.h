#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "DataStruct.h"
#include "Item.h"

struct TileLayer {
    std::string name;
    std::vector<int> data;
    int width, height;
};

struct Tileset {
    int firstgid;
    int tilecount;
    int tileWidth, tileHeight;
    int columns;
    SDL_Texture* texture = nullptr;
};

struct SpawnPoints {
    Vector2 playerSpawn{};
    Vector2 princessSpawn{};
    std::vector<Vector2> zombieSpawns;
};

struct MapData {
    int mapWidth = 0;
    int mapHeight = 0;
    int tileWidth = 0;
    int tileHeight = 0;

    SpawnPoints spawnPoints;
    
    std::vector<Tileset> tilesets;
    std::vector<TileLayer> layers;
    TileLayer aboveLayer;
    std::vector<std::vector<bool>> collisionMap;

    std::vector<Item> items;
};