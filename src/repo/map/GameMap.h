#pragma once
#include <SDL.h>
#include "Map.h"

class GameMap : public Map {
public:
    GameMap(SDL_Renderer* renderer);
    ~GameMap();

    void render();
    void renderAboveLayer();

private:
    SDL_Renderer* renderer;

    void drawLayer(const TileLayer& layer);
};