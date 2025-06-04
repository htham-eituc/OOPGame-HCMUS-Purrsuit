#include "MapFactory.h"
#include "TiledMapLoader.h"
#include "Map.h"

Map* MapFactory::create(SDL_Renderer* renderer, const std::string& path) {
    Map *map = new Map(renderer);
    MapData mapData = TiledMapLoader::loadMap(path, renderer);
    map->loadFromData(mapData);
    return map;
}