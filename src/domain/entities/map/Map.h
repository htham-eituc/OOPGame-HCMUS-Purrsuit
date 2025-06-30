#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "MapTypes.h"
#include "Constants.h"

class Item;

class Map {
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

    int getMapPixedHeight() const {
        return mapData.mapHeight * mapData.tileHeight;
    }

protected:
    MapData mapData;
};