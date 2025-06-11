# Project File Structure (DDD) for SDL2 Game

## 📁 core/

**Purpose**: Engine-like systems used globally (audio, assets, logging, input). Should be stateless or singleton-based services.

* `AudioManager` — loads and plays sound/music
* `TextureManager` — loads and caches textures
* `Services.h` — globally registers and provides access to core services

**Example Usage:**

```cpp
core::audio->playSound("coin");
core::textures->getTexture("player_idle");
```

---

## 📁 domain/

**Purpose**: Core game logic and business rules (entities, models, components, services, value objects).
Avoid dependencies on SDL, Tiled, or any external library.

### Subfolders:

* ### 📁 entity/

  **Active agents with behavior or state changes**

  * `Player.h` — the main controllable character
  * `Enemy.h` — AI-controlled hostile character
  * `NPC.h` — friendly or neutral characters
  * `Character.h` — base class shared by Player, Enemy, NPC

* ### 📁 model/

  **Data-holding structs and persistent world state**

  * `Item.h` — item properties (id, type, effect)
  * `Inventory.h` — list of items player has
  * `GameStateData.h` — persistent flags, keys, progress
  * `TransitionZone.h` — map transition area definition

* ### 📁 value\_object/

  **Stateless objects defined by their value**

  * `Vector2.h` — 2D vector math
  * `Rect.h` — AABB for logic-side collision/positioning

* ### 📁 interface/

  **Abstract interfaces defining behavior**

  * `IInteractable.h` — interactable objects (e.g., items, signs)
  * `IUpdatable.h` — updatable systems/entities with `update(float dt)`

* ### 📁 service/

  **Domain logic operations on models and entities**

  * `CollisionService.h` — domain-side collision checks
  * `InventoryService.h` — logic for item pickup/removal
  
Keep SDL logic in `core/` and `UI/`. Avoid polluting `domain/` with rendering, sound, or file parsing logic.

---

## 📁 app/

**Purpose**: Application orchestration and coordination (map switching, game states, scene control)

### Subfolders:

* `state/` — state machines and transitions
* `loader/` — factories for loading maps, characters

**Example Classes:**

* `GameStateMachine.cpp` — checks if player touches a transition zone
* `MapFactory.cpp` — loads a map and connects domain data

---

## 📁 repo/

**Purpose**: Data loaders, file parsers, saving/loading from disk or external resources

* `JsonMapLoader.cpp` — parses Tiled map JSON
* `SaveSystem.cpp` — saves and loads progress from file

---

## 📁 UI/

**Purpose**: Rendering, input handling, and main loop

* `GameUI.cpp` — main loop
* `MainMenu.cpp` — renders and handles menu logic

---

## 📁 di/ (optional)

**Purpose**: Dependency Injection setup and wiring

---

## 📁 tests/ (optional)

**Purpose**: Unit tests for logic in `domain/` and `app/`

---

## 📁 event/ (optional)

**Purpose**: Event bus and observer system for game-wide communication

---

## Example Runtime Flow:

1. Player touches a transition zone (from `domain/entity/TransitionZone.h`)
2. `GameStateMachine` in `app/state/` detects it
3. Calls `MapFactory` in `app/loader/` to create the next map
4. Plays cutscene via `core::audio`
5. New map and entities drawn from `UI/Game.cpp`

---

