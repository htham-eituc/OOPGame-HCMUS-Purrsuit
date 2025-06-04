#pragma once
#include <string>

class Map;
struct SDL_Renderer;

class MapFactory {
public:
    static Map* create(SDL_Renderer* renderer, const std::string& path);
};
