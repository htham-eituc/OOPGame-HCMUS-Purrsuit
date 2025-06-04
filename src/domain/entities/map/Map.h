#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "MapTypes.h"
#include "Constants.h"

class Item;

class Map {
public:
    Map(SDL_Renderer* renderer); 
    ~Map();

    void loadFromData(const MapData& data);
    void render();             // Regular layers
    void renderAboveLayer();  // "Above" layer, after characters

    bool checkCollision(const SDL_Rect& box) const;
    bool isCollidable(int x, int y) const;

    std::vector<Item>& getItems();
    const MapData& getMapData() const; // For Minimap or AI

private:
    void drawLayer(const TileLayer& layer);

    SDL_Renderer* renderer;

    MapData mapData;
};
