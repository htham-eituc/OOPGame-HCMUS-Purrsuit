#pragma once
#include <vector>
#include "DataStruct.h"

class PathFinder {
public:
    PathFinder(const std::vector<std::vector<bool>>& collisionMap, int width, int height);

    std::vector<Vector2> findPath(const Vector2& startWorld, const Vector2& endWorld, int tileSize);

private:
    const std::vector<std::vector<bool>>& map;
    int mapWidth;
    int mapHeight;
};
