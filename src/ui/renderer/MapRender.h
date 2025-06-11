#pragma once
#include <SDL.h>
#include "Map.h"

class MapRender : public Map {
public:
    MapRender(SDL_Renderer* renderer);
    ~MapRender();

    void render();
    void renderAboveLayer();

private:
    SDL_Renderer* renderer;

    void drawLayer(const TileLayer& layer);
};