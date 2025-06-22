#pragma once 

inline constexpr int SCREEN_WIDTH = 800;
inline constexpr int SCREEN_HEIGHT = 600;
inline constexpr int TILE_SIZE = 32;

inline const char* MAP_PATH_1 = "assets/maps/Stage1.json";
inline const char* MAP_PATH_2 = "assets/maps/testMap2.json";

namespace audio {
    // music
    constexpr const char* title = "assets/music/titleSoundtrack.mp3";
    constexpr const char* lv1m = "assets/music/level1-soundtrack.mp3";

    // sfx
    constexpr const char* ping = "assets/music/itemPickupSound.wav";
    constexpr const char* move = "assets/music/walkOnGrassSound.wav";
}

namespace texture {
    // character 
    constexpr const char* player_idle = "assets/character/MainCatIdle.png";
    constexpr const char* player_walk = "assets/character/MainCatWalk.png";

    constexpr const char* title_screen = "assets/ui/forg2kg.png";
    constexpr const char* start_button = "assets/ui/startButton.png";
    constexpr const char* level1_exit_zone = "assets/ui/level1_exit_zone.png";

    // UI
}