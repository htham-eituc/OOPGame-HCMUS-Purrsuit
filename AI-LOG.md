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
### 📄 Game Roadmap 

**Prompt Used:**  *(For suggestion)*  
We are doing an OOP project about a game, the story of the game is: you are a cat and you have to rescue your princess, in each map, you have to avoid obstacles and NPCs that are chasing you (it could contain from one to two NPCs to make the hard level). This is just a simple project, we can make a pixel game or else, make me a road map to implement this, please include as much as possible, such as: how to make the graphics, which programming languages and tools should be use, how to add the AI into your game, what i have to learn to make the game, and so on. You can give me multiple examples.

**AI Response Summary:**  
Game Summary
Title (Example): Cat Quest: Rescue the Princess   
Genre: 2D Top-down pixel adventure game   
Player: A cat navigating levels to rescue a princess  
Enemies: 1–2 NPCs per level that chase the player using A*  
Obstacles: Static or moving barriers  
Goal: Reach the exit or key point while avoiding being caught

🔧 Tech Stack & Tools (Pick Based on Your Preferences)  
🔹 Recommended Programming Languages:  
| Option                        | Pros                                                |
| ----------------------------- | --------------------------------------------------- |
| **C++ with SDL2**             | Fast, OOP-friendly, more control                    |
| **Python with Pygame**        | Simpler syntax, great for beginners                 |
| **C# with Unity**             | Powerful, many built-in tools, drag-and-drop editor |
| **JavaScript with Phaser.js** | Web-based games, runs in browser                    |

🛣️ (Shorten) Roadmap:  
📌 PHASE 1 – Plan & Learn  
📌 PHASE 2 – Game Engine & Graphics  
📌 PHASE 3 – Core Gameplay  
📌 PHASE 4 – Level Design  
📌 PHASE 5 – Add Polish  
📌 PHASE 6 – Test & Submit  

🧠 AI Techniques to Implement
AI Technique	Use Case  
A* Pathfinding	NPCs chase the cat player intelligently  
Finite State Machine (FSM)  
NPC behavior: patrol, chase, search, return  
(Bonus) Dynamic difficulty scaling	Increase NPC speed/vision on later maps  

✅ What You Should Learn:
| Area      | Topics                            |
| --------- | --------------------------------- |
| C++ or C# | Classes, pointers, OOP principles |
| Game Dev  | Loops, states, asset loading      |
| AI        | FSM, A\*, simple heuristics       |
| Tools     | SDL2, Tiled, Aseprite, Git        |


**Decision & Application:**  
- For C++ OOP-focused school projects, we choose C++ with SDL2 which is familar and to have more knowledge about OOP.
- C# and Unity is not suitable as some teammate's laptop could not handle the "heavy" of the IDE. 
- We have used the application that AI provide:
  - Aseprite: to design characters, game UI. 
  - Tiled: to design map.
  - Git: to cooperate code with others.
- The road map that AI give is good for our project, doesn't need much adjust for our project plan.
---


### 🎮 Game plot

**Prompt Used:**  
  My cat game now need end game plot, i'm planning for my cat to collect item to creats cure for zombie cat to become normal cat again. In the last map, i don't know how my cat can spread/use the cure for all the zombie without much coding. We can use cut scene for this. Give at least 5 reasonable plot to handle this (using the game mechanism again, not heavy coding).  

**AI Response Summary:**  
🔥 Plot Idea 1: Broadcasting the Cure via Sound
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
### 🧩 Core Class Design

**Prompt Used:**  
What is Domain-Driven Design? Should we apply it for my cat game. Suggest design for my game to use it.

**AI Response Summary:**  
*1. Core Domain Concepts (The "Game World")*
Start by identifying the core game concepts — your domain. These are the objects and behaviors central to your game.

| Concept     | Responsibility                                         |
| ----------- | ------------------------------------------------------ |
| `Character` | Base for all entities that move (e.g. player, enemies) |
| `Player`    | Controls the cat, handles input, item interaction      |
| `NPC`       | Follows FSM, AI behavior                               |
| `Map`       | Holds tile data and collision information              |
| `Item`      | Collectible entities with effects                      |
| `Inventory` | Holds items for the player                             |
| `GameState` | Tracks current level, rescued princess, etc            |

These become your entities and value objects in DDD.

*2. Bounded Contexts (Subsystems)*  
Separate parts of the game logically into contexts:

Example Bounded Contexts:
- WorldContext  
Map, Tiles, Collisions, Navigation Grid  

- CharacterContext  
Player, NPCs, Movement, FSM, Health

- InteractionContext  
Items, Doors, Switches, Triggers

- GameStateContext  
Win/Lose conditions, saved progress, level transitions

Each context owns its own logic and communicates through defined interfaces.

*3. Folder structure* (shorten, more our in report)  
```
src/
├── Core/                  ← Shared types like Vector2, Direction, Timer
├── Domain/
│   ├── World/             ← Map, Tile, Layer, Collision logic
│   ├── Characters/        ← Character, Player, NPCs, FSM, Movement
│   ├── Items/             ← Item, Inventory, Collect logic
│   ├── GameState/         ← Game status, level, flags
├── Application/           ← Orchestrates game flow, scenes, level logic
├── Infrastructure/        ← SDL2 wrappers, rendering, input, audio
├── UI/                    ← Menus, HUD, item UI
```

*4. Services and Use Cases*  
Example Domain Services:
CollisionService – checks tile + character collisions  
MovementService – applies velocity, handles sliding  
ItemPickupService – handles when a player overlaps with an item  
These are stateless services that coordinate domain objects.

**Decision & Application:**  
Decide to use DDD for our game, due to:
- Clear structure as the game grows (multiple levels, more NPC types, quests). 
- Easier testing, since logic is separated from rendering.  
- Maintainable as features evolve (e.g. adding UI, levels, dialogue).  
- Great for our teams: each dev can focus on a bounded context.

---

### 🗂 UML & Design Artifacts

**Prompt Used:**  

**AI Response Summary:**  
- [Patterns suggested, pros and cons]

**Decision & Application:**  
- [Which patterns you chose and how they helped your architecture]

---
### 🗂 Define AI state

**Prompt Used:**  *(For design artifacts)*  
I want to implement the pathfinding algorithm (AStar) and FSM, generate a visual FSM diagram so i could understand it.

**AI Response Summary:**  
NPC Behavior Finite State Machine

       +----------------+
       |     Patrol     |
       +----------------+
               |
         [sees player]
               v
       +----------------+
       |     Chase      |
       +----------------+
               |
        [loses sight]
               v
       +----------------+
       |    Search      |
       +----------------+
               |
        [timeout over]
               v
       +----------------+
       |     Return     |
       +----------------+
               |
       [back at path]
               v
         back to Patrol

**Decision & Application:**  
- Understanding and use plant UML to draw out FSM. 

**Prompt Used:**  *(For learning how to do)*  
I'm doing the map design part for my cat game. It is a 32x32 pixel game. Recommend assets that i can have idea from. After that, step by step introduce how to use the Map that is parsing from Tiled.

**AI Response Summary:**  
*Game assets*
1. Kenney Game Assets All-in-1
Overview: A comprehensive collection of over 60,000 assets, including 2D sprites, 3D models, UI elements, and more.
Assets are in PNG format, making them easy to use with SDL2.
Link: Kenney Game Assets All-in-1

2. Pixel Art Top Down - Basic by Cainos
Overview: A set of 32x32 pixel art sprites and tilesets designed for top-down games.
Link: Pixel Art Top Down - Basic

3. Modern Interiors - RPG Tileset [16x16] by LimeZu
Overview: A detailed 16x16 tileset perfect for creating modern indoor environments. 
Link: Modern Interiors - RPG Tileset

4. Tiny RPG Character Asset Pack v1.03 by Zerie
Overview: A collection of 20 animated characters suitable for RPGs.
Link: Tiny RPG Character Asset Pack

5. Free - Pixel Art Asset Pack - Topdown Tileset - 16x16 Sprites by Anokolisa
Overview: A free asset pack containing a variety of sprites for top-down games.
Link: Pixel Art Asset Pack

6. Sprout Lands - Asset Pack by Cup Nooble
Overview: A cute pixel pastel farming asset pack.
Link: Sprout Lands - Asset Pack

*Guide to integrate Tileson into your project:*

✅ Step 1: Export Your Map from Tiled
Open your map in Tiled.  
Navigate to File > Export As.  
Choose JSON map files (.json) as the format.  
Save the file, e.g., map.json.  
Ensure that your tilesets are either embedded within the map or that you have access to the external .tsx files referenced.

✅ Step 2: Integrate Tileson into Your Project
Download Tileson:  
Visit the Tileson GitHub repository and download the latest version.  
Include Tileson in Your Project:  
Add the tileson.hpp file to your project's include directory.  
Ensure your compiler supports C++17.  
Include the Header in Your Code:

✅ Step 3: Parse the Map Using Tileson
Here's how you can load and parse the map:
```cpp
#include "tileson.hpp"

tson::Tileson parser;
std::unique_ptr<tson::Map> map = parser.parse("assets/maps/map.json");

if (map->getStatus() != tson::ParseStatus::OK) {
    std::cerr << "Failed to load map: " << map->getStatusMessage() << std::endl;
    return;
}

// Access map properties
int tileWidth = map->getTileSize().x;
int tileHeight = map->getTileSize().y;
int mapWidth = map->getSize().x;
int mapHeight = map->getSize().y;
```
✅ Step 4: Load Tileset Textures
Load the tileset image(s) referenced in your map:  
<< source code >>

✅ Step 5: Render the Map Layers
Iterate through each layer and render the tiles:  
<< source code >>

✅ Step 6: Implement Collision Detection
If you've designated a layer (e.g., named "Collision") for collision detection:  
<< source code >>

✅ Step 7: Handle Layer Rendering Order
To render elements like tree leaves above the player:  
Layer Order: Ensure that in Tiled, the "Above" layer (e.g., tree leaves) is placed above the "Ground" and "Collision" layers.  
- Rendering Sequence:  
- Render the "Ground" and "Collision" layers first.  
- Render the player character.  
- Render the "Above" layer last.  

This sequence ensures that the player appears behind elements like tree leaves but cannot walk through tree trunks if the collision is properly set.

**Decision & Application:**  
- Most of the assets cannot be use, but we have found other in the same web (itch.io). 
- The step to parse map from Tiled is now using in
 our map, we have change map design layer to suit out gameplay. 
---

## 🔁 Phase 2: Implementation & Iteration

### 🏗 Boilerplate & Code Generation

**Prompt Used:**  

**AI Response Summary:**  
- [How AI explained or helped fix the issue]

**Decision & Application:**  
- [Was the fix correct? Did it help identify the root cause?]

---
### Spawning and Zone Transition in tilemap
**Prompt Used:** *(For generate code)*  
Implement a TiledMapLoader::LoadSpawnPointsFromMap function that reads from an object layer named ‘Spawning’ and returns a SpawnPoints struct with player, princess, and zombie spawn points using tileson.hpp. This is the example of the tilemap.json. Ask me question until you can do properly.  
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
**Prompt Used:** *(For Debug)*  
The logic and mechanism run well on seperate stage. But the problem is, the item is not in the same tileset on map, so the gid might be different right. I'm using Tiled for parsing. So if i manage an inventory through out the game, i couldn't, how do i fix this?

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

