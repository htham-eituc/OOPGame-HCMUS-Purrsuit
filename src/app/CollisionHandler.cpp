#include "CollisionHandler.h"
#include "Map.h"
#include "Character.h"
#include "Item.h"

bool CollisionHandler::checkMapCollision(const SDL_Rect& box, const Map& map) {
    const int tileSize = 32;

    int leftTile   = box.x / tileSize;
    int rightTile  = (box.x + box.w - 1) / tileSize;
    int topTile    = box.y / tileSize;
    int bottomTile = (box.y + box.h - 1) / tileSize;

    for (int y = topTile; y <= bottomTile; ++y) {
        for (int x = leftTile; x <= rightTile; ++x) {
            if (map.isCollidable(x, y)) return true;
        }
    }
    return false;
}

bool CollisionHandler::checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_HasIntersection(&a, &b);
}