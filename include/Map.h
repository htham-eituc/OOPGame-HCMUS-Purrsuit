#pragma once
#include <SDL.h>
#include <string>
#include <vector>

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

class Map {
public:
    Map(SDL_Renderer* renderer);
    ~Map();

    void render();             // Regular layers
    void renderAboveLayer();  // "Above" layer, after characters
    bool checkCollision(const SDL_Rect& box) const;
    bool isCollidable(int x, int y) const;
    const int TILE_SIZE = 32;

private:
    void loadMap(const std::string& path);
    SDL_Texture* loadTexture(const std::string& path);
    void drawLayer(const TileLayer& layer);


    SDL_Renderer* renderer;

    int mapWidth = 0, mapHeight = 0;
    int tileWidth = 0, tileHeight = 0;

    std::vector<Tileset> tilesets;
    std::vector<TileLayer> layers;
    std::vector<std::vector<bool>> collisionMap;
    TileLayer aboveLayer;

};
