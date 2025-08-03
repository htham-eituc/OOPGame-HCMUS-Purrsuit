#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "MapTypes.h"
#include "Constants.h"

class Item;

class Map {
protected:
    MapData mapData;

public:
    Map() = default;
    virtual ~Map() = default;

    void loadFromData(const MapData& data) { mapData = data; }

    const std::vector<Item>& getItems() const { return mapData.items; }
    std::vector<Item>& getItems() { return mapData.items; }

    bool isCollidable(int x, int y) const {
        if (y < 0 || y >= mapData.mapHeight || x < 0 || x >= mapData.mapWidth)
            return true; 
        return mapData.collisionMap[y][x];
    }

    int getMapPixelWidth() const { 
        return mapData.mapWidth * mapData.tileWidth; 
    }

    int getMapPixelHeight() const {
        return mapData.mapHeight * mapData.tileHeight;
    }

    int getWidth() const {
        return mapData.mapWidth;
    }

    int getHeight() const {
        return mapData.mapHeight;
    }  

    std::vector< std::vector<bool> > getCollisionMap() const {
        return mapData.collisionMap;
    }

    const SpawnPoints& getSpawnPoints() const { 
        return mapData.spawnPoints; 
    }

    const std::vector<TransitionZone>& getTransitionZones() const {
        return mapData.transitionZones;
    }

    const std::vector<Tileset>& getTileSets() const {
        return mapData.tilesets;
    }
};