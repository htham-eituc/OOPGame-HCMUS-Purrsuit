#include "MapFactory.h"
#include "TiledMapLoader.h"
#include "GameMap.h"
#include "Map.h"

GameMap* MapFactory::create(SDL_Renderer* renderer, const std::string& path) {
    GameMap *map = new GameMap(renderer);
    MapData mapData = TiledMapLoader::loadMap(path, renderer);
    map->loadFromData(mapData);
    return map;
}