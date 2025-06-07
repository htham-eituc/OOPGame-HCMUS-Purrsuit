#pragma once
#include <string>

class MapRender;
struct SDL_Renderer;

class MapFactory {
public:
    static MapRender* create(SDL_Renderer* renderer, const std::string& path);
};
