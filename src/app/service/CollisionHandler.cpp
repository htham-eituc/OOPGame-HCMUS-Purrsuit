#include "CollisionHandler.h"
#include "Map.h"
#include "Character.h"
#include "Item.h"

bool CollisionHandler::checkMapCollision(const SDL_Rect& box, const Map& map) {
    int leftTile   = (box.x < 0) ? -1 : box.x / TILE_SIZE;
    int rightTile  = (box.x + box.w - 1) / TILE_SIZE;
    int topTile    = (box.y < 0) ? -1 : box.y /TILE_SIZE;
    int bottomTile = (box.y + box.h - 1) / TILE_SIZE;

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