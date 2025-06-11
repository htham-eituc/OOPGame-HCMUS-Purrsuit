#pragma once
#include <SDL_rect.h>
#include <vector>

class Map;
class Character;
class Item;

class CollisionHandler {
public:
    // Tile-based map collision
    static bool checkMapCollision(const SDL_Rect& box, const Map& map);

    // Generic AABB check
    static bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
    
};