#include "PathFinder.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>

struct AStarNode {
    int x, y;
    float gCost, hCost;
    AStarNode* parent;

    float fCost() const { return gCost + hCost; }

    bool operator>(const AStarNode& other) const {
        return fCost() > other.fCost();
    }
};

inline float heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

PathFinder::PathFinder(const std::vector<std::vector<bool>>& collisionMap, int width, int height)
    : map(collisionMap), mapWidth(width), mapHeight(height) {}

std::vector<Vector2> PathFinder::findPath(const Vector2& startWorld, const Vector2& endWorld, int tileSize) {
    int sx = static_cast<int>(startWorld.x / tileSize);
    int sy = static_cast<int>(startWorld.y / tileSize);
    int ex = static_cast<int>(endWorld.x / tileSize);
    int ey = static_cast<int>(endWorld.y / tileSize);

    struct NodeCmp {
        bool operator()(const AStarNode* a, const AStarNode* b) const {
            return a->fCost() > b->fCost();
        }
    };

    std::priority_queue<AStarNode*, std::vector<AStarNode*>, NodeCmp> openSet;
    std::unordered_map<int, AStarNode*> allNodes;

    auto hash = [this](int x, int y) {
        return y * mapWidth + x;
    };

    auto getNode = [&](int x, int y) -> AStarNode* {
        int id = hash(x, y);
        if (allNodes.count(id)) return allNodes[id];
        AStarNode* node = new AStarNode{x, y, INFINITY, INFINITY, nullptr};
        allNodes[id] = node;
        return node;
    };

    AStarNode* startNode = getNode(sx, sy);
    AStarNode* endNode = getNode(ex, ey);

    startNode->gCost = 0.0f;
    startNode->hCost = heuristic(sx, sy, ex, ey);
    openSet.push(startNode);

    std::vector<std::pair<int, int>> directions = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    while (!openSet.empty()) {
        AStarNode* current = openSet.top();
        openSet.pop();

        if (current->x == ex && current->y == ey) {
            std::vector<Vector2> path;
            for (AStarNode* node = current; node != nullptr; node = node->parent) {
                path.emplace_back(node->x * tileSize + tileSize / 2, node->y * tileSize + tileSize / 2);
            }
            std::reverse(path.begin(), path.end());

            for (auto& p : allNodes) delete p.second;
            return path;
        }

        for (auto& [dx, dy] : directions) {
            int nx = current->x + dx;
            int ny = current->y + dy;

            if (nx < 0 || ny < 0 || nx >= mapWidth || ny >= mapHeight || map[ny][nx])
                continue;

            AStarNode* neighbor = getNode(nx, ny);
            float tentativeG = current->gCost + 1.0f;

            if (tentativeG < neighbor->gCost) {
                neighbor->gCost = tentativeG;
                neighbor->hCost = heuristic(nx, ny, ex, ey);
                neighbor->parent = current;
                openSet.push(neighbor);
            }
        }
    }

    for (auto& p : allNodes) delete p.second;
    return {};
}
