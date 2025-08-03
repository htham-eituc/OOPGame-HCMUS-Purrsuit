#pragma once
#include <string>
#include <SDL.h>
#include <vector>

struct Tileset;

class Item {
private: 
    std::string name;
    SDL_Rect rect;
    bool collected = false;
    int gid = 0;
public:
    Item(const std::string& name, const SDL_Rect& rect, int gid);
    void render(SDL_Renderer* renderer, const std::vector<Tileset>& tilesets);
    
    std::string getName() const;
    bool isCollected() const;
    SDL_Rect getBounds() const;
    int getGid() const;

    void setCollected(bool state); 
};