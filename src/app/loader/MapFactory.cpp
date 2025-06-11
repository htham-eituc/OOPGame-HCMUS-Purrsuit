#include "MapFactory.h"
#include "TiledMapLoader.h"
#include "MapRender.h"
#include "Map.h"

MapRender* MapFactory::create(SDL_Renderer* renderer, const std::string& path) {
    MapRender *map = new MapRender(renderer);
    MapData mapData = TiledMapLoader::loadMap(path, renderer);
    map->loadFromData(mapData);
    return map;
}