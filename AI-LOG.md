# AI Collaboration Log —  

- **Project Name**: Purrsuit: A Whisker - A Whisper 
- **Team Members**:  
    1. **Ngo Huynh Tham** [GitHub](https://github.com/htham-eituc) | [Email](mailto:nhtham2413@clc.fitus.edu.vn)  
    2. **Nguyen Cao Thong** [GitHub](https://github.com/merdeput) | [Email](mailto:ncthong2427@clc.fitus.edu.vn)  
    3. **Tran Minh Tien** [GitHub](https://github.com/tien140406) | [Email](mailto:tmtien2418@clc.fitus.edu.vn)
    4. **Huynh Thai Hoang** [GitHub](https://github.com/tohuu123) | [Email](mailto:hthoang2412@clc.fitus.edu.vn)
- **AI Tools Used**: ChatGPT, Claude  
- **Period of Use**: 1/6/2025 – 24/8/2025  

---

## Table of Contents

- [Phase 1: Conceptualization & Architectural Design](#phase-1-conceptualization--architectural-design) 
- [Phase 2: Implementation & Iteration](#phase-2-implementation--iteration)
- [Summary & Reflections](#summary--reflections)

---

## Phase 1: Conceptualization & Architectural Design

### Game Roadmap 

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


### Game plot

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

### Core Class Design

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

### Define AI state

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
### Design Pattern Apply

**Prompt Used:**
"I need to write AI-log for my game. My game is a plot-game so it has to manage a lot of stages. What design patterns would you recommend for handling complex plot progression and stage management?"

**AI Response Summary:**

The AI recommended four key design patterns for managing complex plot stages in a narrative-driven game:

- **State Pattern**: Manage different plot stages as distinct states with their own behaviors, allowing clean separation of stage-specific logic and easy transitions between plot stages
- **Factory Pattern**: Create appropriate plot stage objects based on game conditions, centralizing stage creation logic and making it easy to add new stage types
- **Template Method Pattern**: Define common structure for all plot stages while allowing customization, ensuring consistent stage lifecycle (enter → update → exit) with shared functionality
- **Facade Pattern**: Simplify complex subsystem interactions by providing a unified interface for plot management

Additional patterns mentioned:
- **Command Pattern** for player actions affecting plot progression
- **Observer Pattern** for UI elements reacting to stage changes  
- **Memento Pattern** for save states and plot history

**Decision & Application:**

**What we are using:**
- **State Pattern**: Implementing as core architecture for plot stages - each major story beat becomes a separate state class with clean separation and easy debugging
- **Factory Pattern**: Using for creating appropriate stage instances based on story progress and game conditions
- **Template Method Pattern**: Adopting the lifecycle approach with consistent `onEnter()`, `update()`, `onExit()` methods across all stages

**What we are rejecting:**
- **Facade Pattern**: My game doesn't need this level of abstraction - the plot system isn't complex enough to warrant a facade layer
- **Memento Pattern**: Not implementing save state pattern as suggested - using different approach for game saves

**What we are adapting:**
- **Command Pattern**: Already using this but for item handling system, not plot progression
- **Observer Pattern**: Already implemented for UI updates and sound event manager, not specifically for plot changes

**Final Architecture**: State + Factory + Template Method for clean, maintainable plot stage management without unnecessary complexity.

---

## Phase 2: Implementation & Iteration

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


### Interactive Tutorial System

---

**Prompt Used:** *(For generate code)* 
"I need to create an interactive tutorial system for my 2D SDL game that teaches players the core mechanics. The system should have multiple stages with animated demonstrations and text explanations. Players should be able to progress through different tutorial sections that cover movement, item collection, and enemy behavior. The interface should feel integrated with the game's aesthetic and not disrupt the gameplay flow."

---

**AI Response Summary:**

- **Component Architecture**
-- Designed a **modular system** with `TutorialTextureManager` for assets and a main `Tutorial` class for logic.  
-- Ensures **separation of concerns** and **maintainability**.  

- **Multi-Stage Framework**
--Built a **stage-based system** with `TutorialStage` and `TutorialTip`.  
--Supports **2–3 tips per stage** with easy future expansion.  

**Dynamic Animation Engine**
- Implemented **sprite animation system** with support for **4–7 frames**.  
- Individual **timing controls** for different animations (movement, zombie behaviors).  

**Integrated UI System**
- Integrated with existing **font rendering and UI components**.  
- **Smart text wrapping** within defined bounds.  
- **Visual feedback** through color-coded highlights.  
- **Book-style interface** that matches medieval aesthetic.  

**State & Input Management**
- **Independent pause mechanism** that doesn’t disrupt the game state.  
- **Intuitive controls**: `T` to open, `Space` to progress, `Esc` to quit.  
- **Smooth transitions** with proper cleanup.  
- Tutorial has **input priority** when active.  

**Performance Optimizations**
- Added **texture caching** and efficient frame management.  
- Ensures **low memory usage** and smooth animations across hardware.  

---

**Decision & Application**

- **Scalable Architecture**
-- Component-based design allows **adding content/animations** without touching core systems.  
--  Provides a **foundation for future educational features**.  

- **Visual Learning Success**
-- Combined **animated demonstrations** with **text explanations**.  
-- **Improved player comprehension** and reduced learning curve.  

- **User Experience Excellence**
-- **Book-style presentation** feels **immersive and lore-appropriate**.  
-- Integrated naturally into the game world rather than as an intrusive overlay. 

---

### On-Screen Hint Alignment

---

**Prompt Used:**  *(For generate code)* 
"I want to display tutorial hints at the bottom of the gameplay screen using image assets like `wasd_onscreen.png` and `esc_onscreen.png`, followed by text like 'move' and 'pause'. I need the text to stay in the same place, but the image should sit slightly above the text for better alignment."  

---

**AI Response Summary:**

- **UI Layout Strategy**  
-- Suggested separating the **y-position of the image and text**.  
-- Proposed shifting the image upwards using `y - offset` while keeping the label’s y fixed.  
-- Ensured both elements remain horizontally aligned and spaced consistently.  

- **Implementation Details**  
-- Used `SDL_Rect` for the key image with `y - 8` adjustment.  
-- Rendered `UILabel` for explanatory text below at the original `y`.  
-- Example code snippet demonstrated correct placement of both.  

- **Design Considerations**  
-- Hints should remain **non-intrusive** and easy to read.  
-- Icons and text must feel like a **single unit** while keeping **visual hierarchy** (icon slightly above, text baseline steady).  

---

**Decision & Application**

- **Final Layout**  
-- `[wasd_onscreen] move     [esc_onscreen] pause` displayed at the bottom of the screen.  
-- Icons shifted upward **8 pixels** for a balanced look.  

- **Improved Readability**  
-- Players instantly associate key images with their actions.  
-- Text alignment remains consistent, avoiding visual jitter.  

- **Scalable Approach**  
-- The same technique can be applied to add more hint icons.  
-- Maintains a **clean separation** between rendering logic for icons and labels.  
---
### Custom Mouse Sprite Animation

---

**Prompt Used:**  *(For generate code)* 
"I found a sprite asset `mouse_on_click.png` consisting of multiple frames. I want to animate the mouse cursor so that when the player clicks, the cursor shows a short animation. It should still behave as a functional cursor and not break button interactions."  

---

**AI Response Summary:**

- **Cursor Replacement Strategy**  
-- Recommended disabling the default SDL cursor using `SDL_ShowCursor(SDL_DISABLE)`.  
-- Proposed rendering a **custom texture** at the mouse’s logical position each frame.  
-- Ensured compatibility with `SDL_RenderSetLogicalSize` so the cursor aligns with UI elements.  

- **Animation System**  
-- Suggested slicing `mouse_on_click.png` into individual frames using `SDL_Rect` clips.  
-- Implemented a **frame index with timing logic** (advance every X ms).  
-- Reset animation when `SDL_MOUSEBUTTONDOWN` is received.  

- **Input Preservation**  
-- Kept `UIButton::handleInput` unchanged for hit detection.  
-- Clarified that cursor rendering is **visual only**, input events still use SDL’s raw mouse events.  

---

**Decision & Application**

- **Functional Animated Cursor**  
-- Cursor now shows a **click animation** whenever the player presses LMB.  
-- Animation resets smoothly and does not interfere with normal rendering.  

- **Enhanced Player Feedback**  
-- Provides immediate **visual confirmation** of a click.  
-- Makes the UI feel more polished and responsive.  

- **Maintainable Design**  
-- Easy to extend with additional states (hover, drag).  
-- Keeps rendering and input handling cleanly separated. 
---

### Cutscene Playback System

---

**Prompt Used:**  
"I want to implement cutscene playback logic in my SDL2 game. The system should display still-image scenes with background music, and players should progress through the cutscene by pressing the spacebar. It should feel cinematic but remain lightweight."  

---

**AI Response Summary:**

- **Cutscene Structure**  
-- Designed a **cutscene state** in the game state machine.  
-- Each cutscene consists of an array of **image + audio pairs**.  
-- Progression handled through **spacebar input** (`SDL_KEYDOWN`).  

- **Playback Logic**  
-- On entering cutscene state: load the current image and play associated music track.  
-- On `Space` press: advance to the next scene (swap image, play new audio if specified).  
-- On reaching the final scene: **transition to the next game state** (e.g., gameplay or title).  

- **Rendering Approach**  
-- Used SDL textures for static background images.  
-- Added optional **subtitle text rendering** for story narration.  
-- Integrated with existing render loop for smooth transitions.  

- **Design Considerations**  
-- Ensured cutscenes can be **skipped or advanced cleanly**.  
-- Lightweight logic avoids complex timing — progression is **fully player-controlled**.  
-- Flexible enough to add new cutscenes by simply extending the scene list.  

---

**Decision & Application**

- **Cinematic Flow**  
-- Players experience story moments through **static but atmospheric scenes**.  
-- Music tracks enhance immersion and tone.  

- **Simple & Effective Control**  
-- `Spacebar` provides intuitive progression.  
-- Cutscenes never lock the player in — they can always continue at their pace.  

- **Scalability**  
-- New cutscenes are easy to implement using the same framework.  
-- Supports expansion with additional features like **fade-in/out transitions** or **animated elements** in the future.  

---
### Sequential Cutscene System

---

**Prompt Used:**  
"I want to implement sequential cutscenes in my SDL2 game. Each cutscene should display a series of images or simple motion frames, accompanied by music. Cutscenes can be triggered automatically (e.g., after a level starts) or via player input."  

---

**AI Response Summary:**

- **Cutscene Sequencing**  
-- Designed a **scene list system**, each containing one or more images and an optional audio track.  
-- Supports **sequential progression**, either automatically after a timer or manually via `Spacebar` or other input.  
-- Allows **branching triggers** for different gameplay events.  

- **Playback Logic**  
-- Automatically load the current image/frame and play its associated music.  
-- On input or timer, advance to the next scene with optional subtitle updates.  
-- On reaching the final scene, transition cleanly to the next game state (level, menu, or other).  

- **Rendering & Motion**  
-- Images rendered as SDL textures, scaled to maintain aspect ratio.  
-- Optional **motion effects** like slow zoom, fade-in/fade-out, or simple translations.  
-- Integrated with existing rendering pipeline to maintain frame consistency.  

- **Audio Integration**  
-- Each scene can have its own **music or sound effect track**.  
-- Smooth audio transitions handled to prevent abrupt stops or overlaps.  

- **Design Considerations**  
-- Lightweight system to avoid blocking main game loop.  
-- Fully **data-driven**, allowing easy addition of new cutscenes by extending scene arrays.  
-- Supports both **automatic triggers** (e.g., level start) and **player-initiated triggers** (e.g., menu selection).  

---

**Decision & Application**

- **Immersive Storytelling**  
-- Sequential cutscenes provide narrative flow without interrupting gameplay.  
-- Music adds emotional depth and atmosphere.  

- **Player-Friendly Control**  
-- Cutscenes can be **skipped or advanced manually** to suit player preference.  
-- Automatic triggers allow for cinematic flow without additional code per scene.  

- **Extensibility**  
-- Framework supports future additions such as **animated overlays**, **branching storylines**, or **subtitles**.  
-- System remains modular and maintainable across multiple levels and cutscenes.
---
### Refactoring & Optimization

**Prompt Used:** 
Refactor my SDL2-based cat game to be more professional and extensible. Use clean architecture/DDD, smart pointers, centralized asset & audio management, prepare for NPCs (zombies) with FSM and sound-driven AI, and load Tiled data dynamically.”

**AI Response Summary:**

* **Architecture & Structure**

  * Adopt a **Clean Architecture / DDD** folder layout: `core/`, `domain/`, `app/`, `repo/`, `ui/` (+ optional `di/`, `events/`, `tests/`).
  * Isolate frameworks (SDL2, audio libs) behind interfaces; keep domain logic pure.
* **Memory & Ownership**

  * Replace raw pointers with **`std::unique_ptr` / `std::shared_ptr`**; clarify lifetimes and pass refs where possible.
* **Assets & Audio**

  * Create a centralized **AssetManager** (textures, animations, maps) and **AudioManager** (IDs instead of raw `Mix_Chunk*`).
  * Add **CharacterAssetManager** for character-specific sprites/animations/sfx.
  * Introduce **CharacterAudioController** to decouple audio logic from `Player`/`Character`.
* **Entities & Inheritance**

  * Refactor a base **`Character`** with state (Idle/Walk), animation hooks, movement; derive **`Player`**, **`Zombie`**, etc.
  * Keep `Game` lean; delegate systems (render, input mapping, audio, scene).
* **AI & Gameplay Systems**

  * Implement **FSM** for NPCs; plug in A\* pathfinding (or waypoints).
  * Add **SoundEventManager** for sound-driven behaviors (publish/subscribe of sound cues).
  * Define 5-second **item effects** via `ItemEffectHandler`:

    * Catnip → silent; Apple → stackable speed; Milk → invincible; Mushroom → slow.
  * **Zombie AI (hearing-based)**: react to footsteps & loud events; chase within hearing zone; wander when idle.
* **Maps & Data**

  * Use **tileson.hpp** to load **Tiled** JSON:

    * `TiledMapLoader` reads object layer **“Spawning”** to place Player/Princess/Zombie.
    * Optional **MapFactory** to construct maps/resources cleanly.
* **Rendering & Collisions**

  * Standardize **Vector2** math; AABB collision handling with clear response.
  * Fix Tiled **flip/rotation** handling in renderer (bitmask flags).
* **Testing & Tooling**

  * Add unit tests for pathfinding, FSM transitions, item effects.
  * Keep an **AI\_LOG.md** to capture prompts, responses, and decisions (this doc).

**Decision & Application**

* **Accepted & Implemented**

  * Reorganized repo to **Clean Architecture / DDD** layout → clearer boundaries, easier onboarding.
  * Replaced raw pointers with **smart pointers** → fewer leaks, simpler ownership.
  * Added **AssetManager** and **AudioManager** (+ IDs) and **CharacterAudioController** → decoupled audio, fewer crashes from dangling audio pointers.
  * Refactored `Character`/`Player`; `Game` no longer manages raw audio/resources → improved separation of concerns.
  * Integrated **tileson** loading & **Spawning** layer → dynamic spawn points, zero hard-coded coordinates.
  * Implemented **SoundEventManager** + **ItemEffectHandler** (5-sec timed effects) → modular gameplay buffs/debuffs.
  * Built **Zombie** FSM with hearing logic & chase/wander behaviors → richer stealth gameplay.
* **Planned / In Progress**

  * **MapFactory** to finalize construction flow for maps & scenes.
  * Unit tests for **A**\*, FSM transitions, and item effect timing.
* **Improvements Observed**

  * **Maintainability:** Modules are smaller, responsibilities clearer; easier feature additions.
  * **Stability:** Smart pointers + managers reduced crashes and resource leaks.
  * **Gameplay Depth:** Sound-driven zombie AI and timed item effects increased tension and player choice.
  * **Productivity:** Dynamic Tiled loading sped up iteration (change map → run, no code edits).
  * **Testability:** Core logic more deterministic and mockable (domain separate from SDL).

---

### UI Element System

**Prompt Used:**  
"I need a modular UI system for my SDL2 game, including interactive buttons, labels, and cursor rendering. Buttons should handle hover, click, and press states, and support callbacks. Labels should allow text rendering with outlines. The UI should work across different game states and integrate smoothly with the game’s input system."  

---

**AI Response Summary:**

- **Component Architecture**  
-- Created modular UI elements: `UIButton` for interactive buttons, `UILabel` for text, and `UIInputHandler` to manage event propagation.  
-- Ensures **separation of concerns**, allowing UI logic to be independent from game logic.  

- **Interactive Buttons**  
-- Support **hover, pressed, and normal states** with visual feedback.  
-- Accept **lambda callbacks** for `onClick` events.  
-- Integrates with `UIInputHandler` for **priority-based event handling**.  

- **Label Rendering**  
-- Text rendering using SDL_ttf, with support for **outline and color customization**.  
-- Enables **dynamic text updates**, e.g., subtitles, tutorial hints, or inventory labels.  

- **Cursor & Mouse Feedback**  
-- Implemented **animated cursor sprite** with click effect.  
-- Cursor animation frame management synchronized with delta time for smooth visuals.  

- **Event Handling & State Management**  
-- Input events routed through `UIInputHandler`, allowing **UI elements to consume events** before the main game handles them.  
-- Works across multiple **game states**, including TITLE, LEVEL1/2, and PAUSE screens.  
-- Supports **dynamic registration/unregistration** of UI elements depending on the current game state.  

- **Visual Integration**  
-- UI elements render in logical coordinates consistent with **SDL_RenderSetLogicalSize**.  
-- Supports **hints and overlays** that integrate naturally with gameplay.  

---

**Decision & Application**

- **User Experience Enhancement**  
-- Buttons and labels provide **intuitive feedback** on interaction.  
-- Cursor animations improve **visual clarity of player actions**.  

- **Maintainable Architecture**  
-- Modular design allows **easy addition of new UI elements** or modifications without affecting core systems.  
-- Provides a foundation for future **HUD, menus, or tutorial overlays**.  

- **Cross-State Consistency**  
-- Ensures that UI behaves **predictably across different game states**, maintaining a polished, professional feel.   

---
## Summary & Reflections

**Overall Benefit:**  
- Helped kickstart the brainstorming process accross the project, creating a basis for further discussion and elaboration on topics regarding themes, names and world building.

- Provided crucial technical support, proved to be irreplaceable in idea realization, problems troubleshooting, as well as structure and ideas feedback.

- Through good prompting and controlled usage, proved to be incredibly helpful throughout the completion of the project.

**Limitations Encountered:**  

- One can rely heavily on AIs for problem solving and coming up with ideas, which can be detrimental to the learning process.

- Due to limited budget, free plans can be hindrance to workflow.

- If prompts are lackluster, blindly following AI's suggestion and generated code without understanding can be dangerous.

**Final Thoughts:**  
We have come to the solution that with the correct treatment of AI as a tool or helper, it presents itself as an incredibly helpful tool for multiple purposes. In future usage, we will be sure to incorporate our lessons in making prompts and how to utilize AI to suit each needs, which are invaluable lessons we gained in this project.

---

