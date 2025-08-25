#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "DataStruct.h"
#include "Item.h"
#include "TransitionZone.h"

struct Tile {
    int gid = 0;          // the actual tile ID (masked, no flip flags)
    bool flipH = false;   // horizontal flip
    bool flipV = false;   // vertical flip
    bool flipD = false;   // diagonal/rotation flip
};

struct TileLayer {
    std::string name;
    std::vector<Tile> tiles;
    int width, height;
};

struct Tileset {
    int firstgid;
    int tilecount;
    int tileWidth, tileHeight;
    int columns;
    int rows;
    SDL_Texture* texture = nullptr;
    std::string imagePath;
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
    
    std::vector<Tileset> tilesets;
    std::vector<TileLayer> layers;
    TileLayer aboveObject;
    TileLayer aboveLayer;
    std::vector<std::vector<bool>> collisionMap;

    SpawnPoints spawnPoints;
    std::vector<TransitionZone> transitionZones;

    std::vector<Item> items;
};