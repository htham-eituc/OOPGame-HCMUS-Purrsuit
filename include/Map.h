#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>

const int TILE_SIZE = 32;
const int MAP_WIDTH = 25;
const int MAP_HEIGHT = 20;

class Map {
public:
    Map(SDL_Renderer* renderer);
    ~Map();

    void render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* tileset = nullptr;
    int tileSize = TILE_SIZE;
    int tilesetCols = 0;
    int tileMap[MAP_HEIGHT][MAP_WIDTH];  // Changed from vector to 2D array
    
    SDL_Texture* loadTexture(const char* path);  // Changed parameter type
};