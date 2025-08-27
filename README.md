# Purrsuit

A C++ game project built with SDL2 and modern CMake.

## Project Structure

```
Purrsuit/
├── src/
│   ├── main.cpp
│   ├── app/
│   │   ├── loader/
│   │   │   ├── MapFactory.cpp
│   │   │   └── MapFactory.h
│   │   ├── saveGame/
│   │   │   ├── GameSave.cpp
│   │   │   └── GameSave.h
│   │   ├── service/
│   │   │   ├── CollisionHandler.cpp/.h
│   │   │   ├── Initializers.cpp/.h
│   │   │   ├── ItemEffectHandler.cpp/.h
│   │   │   ├── SoundEventManager.cpp/.h
│   │   │   └── TransitionManager.cpp/.h
│   │   └── ui/
│   │       ├── Color.h
│   │       ├── UIInputHandler.cpp/.h
│   │       └── UIRenderer.cpp/.h
│   ├── core/
│   │   ├── Constants.h
│   │   ├── Services.h
│   │   ├── assets/
│   │   │   ├── AudioManager.cpp/.h
│   │   │   ├── UIManager.cpp/.h
│   │   │   └── texture/
│   │   │       ├── InventoryTextureManager.cpp/.h
│   │   │       ├── TextureManager.cpp/.h
│   │   │       └── TutorialTextureManager.cpp/.h
│   │   ├── game/
│   │   │   └── GameState/
│   │   │       ├── Game.cpp/.h
│   │   │       ├── GameHelper.cpp
│   │   │       ├── GameStateBase.h
│   │   │       ├── GameStateMachine.cpp/.h
│   │   │       ├── GameStateType.h
│   │   │       ├── StateFactory.cpp/.h
│   │   │       ├── cutscene/
│   │   │       │   ├── CutScenePlot.cpp
│   │   │       │   └── CutSceneState.cpp/.h
│   │   │       ├── level/
│   │   │       │   ├── LevelConcrete.cpp
│   │   │       │   └── LevelState.cpp/.h
│   │   │       └── special/
│   │   │           ├── DeathState.cpp/.h
│   │   │           ├── TitleState.cpp/.h
│   │   │           └── WinState.cpp/.h
│   │   └── utils/
│   │       ├── Camera.cpp/.h
│   │       ├── MemoryUtils.h
│   │       └── PathFinder.cpp/.h
│   ├── domain/
│   │   ├── entities/
│   │   │   ├── character/
│   │   │   │   ├── Character.cpp/.h
│   │   │   │   ├── Player.cpp/.h
│   │   │   │   ├── Princess.cpp/.h
│   │   │   │   └── ZombieCat.cpp/.h
│   │   │   ├── items/
│   │   │   │   ├── Inventory.cpp/.h
│   │   │   │   └── Item.cpp/.h
│   │   │   └── map/
│   │   │       ├── Map.h
│   │   │       ├── MapTypes.h
│   │   │       └── TransitionZone.h
│   │   ├── services/
│   │   │   └── item/
│   │   │       ├── IItemEffect.h
│   │   │       ├── InvincibleEffect.h
│   │   │       ├── SilentEffect.h
│   │   │       └── SpeedModifyEffect.h
│   │   ├── ui/
│   │   │   ├── Tutorial.cpp/.h
│   │   │   ├── UIButton.cpp/.h
│   │   │   ├── UIElement.h
│   │   │   └── UILabel.cpp/.h
│   │   └── value_object/
│   │       └── DataStruct.h
│   ├── repo/
│   │   ├── SaveGameManager.cpp
│   │   └── map/
│   │       ├── TiledMapLoader.cpp
│   │       └── TiledMapLoader.h
│   └── ui/
│       ├── MainMenu.h
│       ├── presentation/
│       └── renderer/
│           ├── MapRender.cpp
│           └── MapRender.h
├── external/
│   ├── json.hpp
│   └── tileson.hpp
├── assets/
│   ├── character/           # Character sprites and animations
│   ├── cutscene/           # Cutscene images
│   ├── fonts/              # Game fonts
│   ├── maps/               # Level map files (JSON)
│   ├── music/              # Background music and sound effects
│   │   └── character_sound/ # Character-specific sounds
│   ├── tiles/              # Tile graphics and spritesheets
│   │   └── map1/
│   └── ui/                 # UI elements and screens
│       ├── inventory/      # Inventory UI components
│       └── tutorial/       # Tutorial-related UI
├── build/                  # CMake build directory
├── CMakeLists.txt
├── build.bat              # Windows build script
├── package_game.bat       # Game packaging script
├── Purrsuit.exe          # Game executable
├── save.json             # Game save file
├── AI-LOG.md             # AI development log
├── DDD.md                # Domain-driven design documentation
├── UML.puml              # UML diagrams
└── README.md
```

## Requirements

### System Requirements

- Windows OS (MinGW build system)
- CMake 3.16 or higher
- MinGW with GCC/G++ compiler
- C++17 compatible compiler

### Dependencies

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf

## Installation

### 1. Install Dependencies

#### Option A: Using vcpkg (Recommended)

```bash
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install SDL2 libraries
.\vcpkg install sdl2 sdl2-image sdl2-mixer sdl2-ttf
```

#### Option B: Manual Installation

Download and install the following libraries:

- [SDL2 Development Libraries](https://www.libsdl.org/download-2.0.php)
- [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
- [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)

### 2. Install MinGW

Download and install MinGW or use MSYS2:

```bash
# Using MSYS2
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake
```

### 3. Install CMake

Download from [cmake.org](https://cmake.org/download/) or use package manager.

## Building

### Quick Build (Windows)

Simply run the provided batch script:

```cmd
build.bat
```

This script will:

1. Create a `build` directory
2. Configure the project with CMake
3. Build using MinGW Make
4. Run the executable

### Manual Build

```cmd
# Create and enter build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "MinGW Makefiles"

# Build the project
mingw32-make

# Go back to root directory
cd ..

# Run the game
Purrsuit.exe
```

### Game Packaging

Use the packaging script to prepare distribution:

```cmd
package_game.bat
```

### CMake Configuration Options

If using vcpkg, you may need to specify the toolchain file:

```cmd
cmake .. -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
```

## Running the Game

After successful build, the executable `Purrsuit.exe` will be created in the root directory. Simply run:

```cmd
Purrsuit.exe
```

The game will automatically create a `save.json` file to store your progress.

## Development

### Architecture

The project follows a layered architecture pattern:

- **App Layer**: Application logic, loaders, state management, services, UI handling
- **Core Layer**: Game engine, assets management, game states, utilities
- **Domain Layer**: Business logic, entities (characters, items, maps), services, UI components
- **Repository Layer**: Data access, persistence, and map loading
- **UI Layer**: User interface rendering and presentation

### Key Features

- **Game State Management**: Comprehensive state machine handling cutscenes, levels, and special states (death, win, title)
- **Character System**: Player, Princess, and ZombieCat entities with animations
- **Item & Inventory System**: Item effects, inventory management with visual feedback
- **Map System**: Tiled map loading with transition zones between levels
- **Save Game**: JSON-based save system with GameSave functionality
- **Audio System**: Music and sound effects via SDL2_mixer with event-driven sound management
- **Asset Management**: Texture, audio, and UI asset loading and management
- **Tutorial System**: Interactive tutorial with visual guides
- **Collision Detection**: Robust collision handling system

### Game Structure

The game consists of:

- **11 Levels**: Various stages including houses, outdoor areas, and castle
- **Cutscenes**: Story-driven animated sequences
- **Characters**: Main cat, pirate cat, zombie cats, and princess
- **Items**: Collectible items with various effects (speed, invincibility, stealth)
- **Maps**: JSON-based level definitions with tile-based graphics

### Adding New Features

1. Place source files in appropriate `src/` subdirectories following the layered architecture
2. Headers are automatically included via CMake configuration
3. The build system uses `GLOB_RECURSE` to automatically include new `.cpp` files
4. Add new assets to the appropriate `assets/` subdirectories
5. Update map files in `assets/maps/` for new levels

## Troubleshooting

### Common Build Issues

#### CMake not found

Make sure CMake is installed and added to your system PATH.

#### SDL2 libraries not found

- Verify SDL2 libraries are properly installed
- If using vcpkg, ensure the toolchain file is specified
- Check that all required SDL2 components are available

#### MinGW compiler not found

- Install MinGW or MSYS2
- Add MinGW bin directory to your system PATH
- Verify `gcc` and `g++` are accessible from command line

#### Build fails with linking errors

- Ensure all SDL2 development libraries are installed
- Check that library versions are compatible
- Verify CMake found all required dependencies (check the status messages)

### Runtime Issues

#### Game won't start

- Ensure all SDL2 DLLs are in the same directory as the executable
- Check that `assets/` folder is in the correct location relative to the executable
- Verify save.json has proper permissions if it exists

#### Missing textures or sounds

- Confirm all asset files are present in the `assets/` directory
- Check file paths in map JSON files
- Ensure texture and audio files are in supported formats

### Debug Build

For debug builds with additional information:

```cmd
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Follow the existing code structure and naming conventions
4. Maintain the layered architecture pattern
5. Test your changes thoroughly with all game states
6. Update documentation if adding new features
7. Submit a pull request

## Contact

[[Github](https://github.com/htham-eituc/OOPGame-HCMUS-Purrsuit)]
