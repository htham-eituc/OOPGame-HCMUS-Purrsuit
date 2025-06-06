#pragma once
#include <string>

class GameMap;
struct SDL_Renderer;

class MapFactory {
public:
    static GameMap* create(SDL_Renderer* renderer, const std::string& path);
};
