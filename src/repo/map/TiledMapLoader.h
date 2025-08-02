#pragma once
#include <string> 
#include "MapTypes.h"

class Map;
class Item;

class TiledMapLoader {
private:
    static SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);

public:
    static MapData loadMap(const std::string& path, SDL_Renderer* renderer);
    static std::vector<Item> LoadItemsFromMap(const std::string& path);
    static SpawnPoints LoadSpawnPointsFromMap(const std::string& path);
    static std::vector<TransitionZone> LoadTransitionZonesFromMap(const std::string& path);
};
