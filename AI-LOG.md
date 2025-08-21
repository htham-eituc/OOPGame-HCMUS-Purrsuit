# 🧠 AI Collaboration Log —  

- **Project Name**: Purrsuit: A Whisker - A Whisper 😸⚔️
- **Team Members**:  
    1. **Ngo Huynh Tham** [GitHub](https://github.com/htham-eituc) | [Email](mailto:nhtham2413@clc.fitus.edu.vn)  
    2. **Nguyen Cao Thong** [GitHub](https://github.com/merdeput) | [Email](mailto:ncthong@clc.fitus.edu.vn)  
    3. **Tran Minh Tien** [GitHub](https://github.com/tien140406) | [Email](mailto:tmtien2418@clc.fitus.edu.vn)
    4. **Huynh Thai Hoang** [GitHub](https://github.com/tohuu123) | [Email](mailto:tmtien2418@clc.fitus.edu.vn)
- **AI Tools Used**: ChatGPT, Claude  
- **Period of Use**: 1/6/2025 – 24/8/2025  

---

## 📌 Table of Contents

- [Phase 1: Conceptualization & Architectural Design](#phase-1-conceptualization--architectural-design)
  - [Game Idea & Mechanics](#game-idea--mechanics)
  - [Core Class Design](#core-class-design)
  - [UML & Design Artifacts](#uml--design-artifacts)
  - [Design Patterns & Challenges](#design-patterns--challenges)
- [Phase 2: Implementation & Iteration](#phase-2-implementation--iteration)
  - [Boilerplate & Code Generation](#boilerplate--code-generation)
  - [Debugging Assistance](#debugging-assistance)
  - [Refactoring & Optimization](#refactoring--optimization)
- [Summary & Reflections](#summary--reflections)

---

## 🧱 Phase 1: Conceptualization & Architectural Design

### 🎮 Game Idea & Mechanics

**Prompt Used:**  

**AI Response Summary:**  
- [Summarize key ideas suggested by the AI]

**Decision & Application:**  
- [Explain what you used, adapted, or rejected, and why]

---
### 🎮 End game plot

**Prompt Used:**  
  My cat game now need end game plot, i'm planning for my cat to collect item to creats cure for zombie cat to become normal cat again. In the last map, i don't know how my cat can spread/use the cure for all the zombie without much coding. We can use cut scene for this. Give at least 5 reasonable plot to handle this (using the game mechanism again, not heavy coding).  

**AI Response Summary:**  
```🔥 Plot Idea 1: Broadcasting the Cure via Sound
Mechanic Used: SoundEventManager

Plot: The player cat builds a "sonic transmitter" at the center of the map using collected items (e.g., wires, amplifier, battery). Once activated, a loud "meow-frequency" spreads the cure through sound.

Gameplay Trigger:

- Player places all cure parts at a specific tile ("machine base").
- Play a sound event: core::soundEvent->emitSound(...)
- All zombies path toward the sound → start shaking → change to normal cats via a cutscene.

Cutscene:
- Zoom out camera, show cats gathering.
- Use a white flash or screen fade, then replace all zombie textures with normal cat ones.

✅ Minimal code, reuses sound + A*.

🌸 Plot Idea 2: Magic Tree Disperses Cure
Mechanic Used: Item placement + tilemap

Plot: The cure must be planted under a magical tree on the final map. The tree absorbs it and releases glowing pollen to cure all zombies.

Gameplay Trigger:
- Final item is placed at the "tree root" tile.
- Start a cutscene: Show sparkles rising, camera pans across the map.
- All zombies slowly sit down and transform back.

✅ Simple cutscene, just needs a special tile.

🌊 Plot Idea 3: Cure in the Water Supply
Mechanic Used: Collision with tile (e.g., a well)

Plot: The player pours the cure into a village well or fountain. It gets into the environment and cures zombies naturally.

Gameplay Trigger:
- Use if (player collides with tile X && has cure) → trigger animation.
- Show zombie cats drinking or reacting as the water sparkles.

✅ Easy to implement using getTile(x, y) and flags.

🧪 Plot Idea 4: Cat Alchemy Bomb
Mechanic Used: Item throw + sound attraction

Plot: The player throws a "cure bomb" made from collected items. It explodes and sends out healing gas + sound. Zombies run toward it and transform.

Gameplay Trigger:

- When bomb is used: emit large sound radius.
- All zombies path toward it using A*, when close → change to cured state.

Cutscene:
- Freeze player movement.
- Let zombies run toward and fade/transform.

✅ Uses emitSound() + pathfinding only.

🌞 Plot Idea 5: Cure with the Power of Meow
Mechanic Used: Player action + NPC trigger

Plot: After collecting all ingredients, your cat performs a "Powerful Meow" at a high place. It echoes across the land and cures all zombies with love.

Gameplay Trigger:
- Step on a "meow spot" tile.
- Trigger animation + cutscene with particle or screen flash.

Zombies stop, look around, and become friendly.

✅ Fully achievable with just one trigger tile and a global flag switch.
Optional Enhancements (No Heavy Code):    
- Camera pan to zombies transforming.  
- Sprite swap from zombie to normal cat.  
- Text dialogue box for narration (just reuse existing render text function).  
- Freeze input during cutscene to avoid bugs.  
```
**Decision & Application:**  
Selected the mystical tree root concept (idea #2). It fits the natural and magical theme of the game and avoids per-zombie scripting. The cat places the final item at the root, triggering a cutscene where glowing vines or particles spread through the ground—symbolizing the cure reaching all zombies at once. Simple to implement with a one-time animation and sound event, no new systems needed.

---

### 🧩 Core Class Design

**Prompt Used:**  

**AI Response Summary:**  
- [List class suggestions, attributes, relationships]

**Decision & Application:**  
- [How you integrated or modified AI suggestions]

---

### 🗂 UML & Design Artifacts

**Prompt Used:**  

**AI Response Summary:**  
- [Patterns suggested, pros and cons]

**Decision & Application:**  
- [Which patterns you chose and how they helped your architecture]

---

## 🔁 Phase 2: Implementation & Iteration

### 🏗 Boilerplate & Code Generation

**Prompt Used:**  

**AI Response Summary:**  
- [How AI explained or helped fix the issue]

**Decision & Application:**  
- [Was the fix correct? Did it help identify the root cause?]

---
### Zombie AI Cat  
**Prompt Used:**  
"Help me create AI for zombies in my game. Zombie cat AI behavior: 
1. Zombies are blind but have strong hearing.
2. They hear player footsteps only when the player is moving nearby.
3. If footsteps are heard nearby, zombies will chase the player until they catch them or the player escapes their hearing zone.
4. Loud sound events attract zombies to the source using A* pathfinding, then they wander around that area.
5. When undisturbed, zombies idle or wander randomly.

**AI Response Summary:**  
- **State Machine Design**: AI proposed a comprehensive state-based system with `ZombieState` enum (Idle, Patrolling, Chasing, Attacking, Stunned) to manage different zombie behaviors
- **Pathfinding Integration**: Implemented A* pathfinding algorithm with `PathfindingUtils` class to handle obstacle avoidance and intelligent navigation around walls and barriers
- **Audio-Based Detection**: Created hearing system where zombies can detect player movement sounds, with `canBeHeard()` checks and audio range calculations
- **Smart Navigation**: 
  - Line-of-sight detection for direct player visibility
  - Fallback to pathfinding when direct path is blocked
  - Dynamic path recalculation when target moves
  - Collision avoidance with map boundaries
- **Behavioral Logic**:
  - Patrol patterns with waypoint systems
  - Smooth transitions between states based on distance and conditions
  - Attack cooldowns and range-based combat
  - Memory system to continue searching after losing sight of player
- **Performance Optimizations**: Suggested limiting pathfinding frequency and using distance checks before expensive calculations

**Decision & Application:**  
- **Architecture**: The state machine approach provided clean, maintainable code that's easy to debug and extend. Each zombie state has clear entry/exit conditions and specific behaviors.
- **Pathfinding Success**: The A* implementation with grid-based collision detection effectively solved wall navigation issues, making zombies appear intelligent rather than getting stuck on obstacles.

### Spawning and Zone Transition in tilemap
**Prompt Used:**  
*(For new feature)* Implement a TiledMapLoader::LoadSpawnPointsFromMap function that reads from an object layer named ‘Spawning’ and returns a SpawnPoints struct with player, princess, and zombie spawn points using tileson.hpp. This is the example of the tilemap.json. Ask me question until you can do properly.  
<<tilemap.json file>>  

**AI Response Summary:**  

1. Suggested Data Structure
We’ll define a simple struct to store spawn info:

```cpp
struct SpawnPoints {
    Vector2 playerSpawn;
    Vector2 princessSpawn;
    std::vector<Vector2> zombieSpawns;
};
```

2. Implement LoadSpawnPointsFromMap
```cpp
SpawnPoints TiledMapLoader::LoadSpawnPointsFromMap(const std::string& path) {
    SpawnPoints spawnPoints;

    tson::Tileson parser;
    std::unique_ptr<tson::Map> map = parser.parse(fs::path(path));
    if (map->getStatus() != tson::ParseStatus::OK) {
        std::cerr << "Failed to parse map for spawn points: " << path << std::endl;
        return spawnPoints;
    }

    tson::Layer* objectLayer = map->getLayer("Spawning");
    if (!objectLayer || objectLayer->getType() != tson::LayerType::ObjectGroup) {
        std::cerr << "No valid 'Spawning' layer found in map: " << path << std::endl;
        return spawnPoints;
    }

    for (const auto& obj : objectLayer->getObjects()) {
        std::string name = obj.getName();  // Expected: "player", "princess", "zombie"
        tson::Vector2f pos = obj.getPosition();
        Vector2 position{ pos.x, pos.y };

        if (name == "player") {
            spawnPoints.playerSpawn = position;
        } else if (name == "princess") {
            spawnPoints.princessSpawn = position;
        } else if (name == "zombie") {
            spawnPoints.zombieSpawns.push_back(position);
        }
    }

    return spawnPoints;
}
```

Then we'll parse that Spawning object layer into this structure.

**Decision & Application:**  
Used this almost exactly as suggested, with minor adjustments for the Vector2 type. The AI incorrectly tried to assign tson::Vector2i to Vector2f, but I corrected it. The function now dynamically loads spawn points for all entities using just map data—zero hardcoding.

---
### Item and Inventory 
**Prompt Used:**  
*(For Debug)* The logic and mechanism run well on seperate stage. But the problem is, the item is not in the same tileset on map, so the gid might be different right. I'm using Tiled for parsing. So if i manage an inventory through out the game, i couldn't, how do i fix this?

**AI Response Summary:**  
Solution Options:
Option 1: Store SDL_Texture* (Simplest)

```cpp
struct InventoryItem {
    std::string name;
    SDL_Texture* texture;  // Store the actual texture
    SDL_Rect srcRect;      // Store the source rectangle
};
```
Option 2: Item ID System (Most flexible)
```cpp
enum class ItemType {
    SWORD = 1,
    POTION = 2,
    KEY = 3,
    // etc.
};

struct InventoryItem {
    std::string name;
    ItemType itemType;     // Universal item identifier
};
```
Option 3: Dedicated Items Tileset
Create one tileset just for inventory items that's consistent across all maps.

**Decision & Application:**  
Choose option 1, due to:  
- Solves GID problem: Items look the same in inventory regardless of which map you're on
- Faster rendering: No tileset lookup needed during inventory render
- Memory efficient: Only stores texture pointers (textures are managed elsewhere)
- Simple integration: Just call getTextureInfo() when collecting items
---
**Prompt Used:**  
"Help me create a visual inventory system for my game. I have items with different GIDs across maps using Tiled, and need a horizontal inventory bar with 6 slots that shows actual item sprites. Items should persist visually when changing maps, and I want to enhance it with custom UI sprites (slot frames, banner background, special item cursors)."

**AI Response Summary:**  
- **Initial Problem Identification**: AI recognized that storing GIDs directly would cause issues when changing maps because tilesets get destroyed/recreated, making texture pointers invalid
- **Solution Architecture**: Proposed creating a separate `InventoryTextureManager` that loads and caches item textures independently from map tilesets, ensuring persistence across map changes
- **Two-Phase Implementation**: 
  1. First solved the core texture persistence issue by storing `SDL_Texture*` and `SDL_Rect` when items are collected
  2. Then enhanced with UI sprites (banner background, slot frames, special item highlighting system)
- **Key Technical Solutions**:
  - Texture caching system to prevent redundant loading
  - Separation of map tilesets from inventory textures
  - Proper sprite scaling and positioning for UI elements
  - Special item management system with cursor overlays
- **Debugging Support**: Provided diagnostic code to identify missing `imagePath` field in tileset loading, which was the root cause of texture loading failures

**Decision & Application:**  
- **Highly Effective**: The solution correctly identified and solved the fundamental issue of texture persistence across map changes. The two-phase approach (fix core logic first, then enhance visuals) was logical and practical.
- **Root Cause Analysis**: AI successfully diagnosed that the missing `tileset.imagePath = imgPath;` assignment in the map loader was causing texture loading failures.
- **Scalable Architecture**: The `InventoryTextureManager` design provides a clean separation of concerns and is easily extensible for future inventory features.
- **Visual Enhancement**: The UI sprite integration (banner, frames, cursors) significantly improved the visual polish while maintaining code organization.
- **Final Result**: Achieved a professional-looking inventory system that persists across map changes with proper sprite rendering and special item highlighting capabilities.
---

### 🧹 Refactoring & Optimization

**Prompt Used:**  


**AI Response Summary:**  
- [Summarize refactoring suggestions]

**Decision & Application:**  
- [Did you accept changes? What improvements were made?]

---

## 🧠 Summary & Reflections

- **Overall Benefit:**  
  [Describe how AI improved your workflow or learning]

- **Limitations Encountered:**  
  [Mention times AI was misleading, wrong, or unhelpful]

- **Final Thoughts:**  
  [Would you use AI in future projects? What would you do differently?]

---

