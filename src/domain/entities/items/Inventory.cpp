#include <iostream>
#include "Inventory.h"

void Inventory::addItem(const std::string& itemName) {
    items.insert(itemName);
    std::cout << "Collected: " << itemName << "\n";
}

bool Inventory::hasItem(const std::string& itemName) const {
    return items.find(itemName) != items.end();
}

void Inventory::render() {
    std::cout << "Inventory: ";
    for (const auto& item : items)
        std::cout << item << " ";
    std::cout << "\n";
}

const std::unordered_set<std::string>& Inventory::getItemNames() const {
    return items; 
}

