#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <SDL.h>

class Inventory {
public:
    void addItem(const std::string& itemName);
    bool hasItem(const std::string& itemName) const;
    void render(); // For now, prints to console

private:
    std::unordered_set<std::string> items;
};

