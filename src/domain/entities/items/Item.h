#pragma once
#include <string>
#include <SDL.h>
#include <vector>

struct Tileset;

class Item {
public:
    std::string name;
    SDL_Rect rect;
    bool collected = false;
    int gid = 0;

    Item(const std::string& name, const SDL_Rect& rect, int gid);
    void render(SDL_Renderer* renderer, const std::vector<Tileset>& tilesets);
    SDL_Rect getBounds() const;
};