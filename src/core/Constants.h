#pragma once 

inline constexpr int SCREEN_WIDTH = 1200;
inline constexpr int SCREEN_HEIGHT = 750;
inline constexpr int TILE_SIZE = 32;

inline constexpr SDL_Rect CAMERA_DEADZONE = {
    SCREEN_WIDTH  / 5,   // x offset from left (20%)
    SCREEN_HEIGHT / 5,   // y offset from top (20%)
    SCREEN_WIDTH  * 3 / 5,  // width = 60% of screen
    SCREEN_HEIGHT * 3 / 5   // height = 60% of screen
};


inline const char* MAP_PATH_1 = "assets/maps/Stage1_ItemsTest.json";
inline const char* MAP_PATH_2 = "assets/maps/testMap2.json";

namespace audio {
    // music
    constexpr const char* title = "assets/music/titleSoundtrack.mp3";
    constexpr const char* lv1m = "assets/music/level1-soundtrack.mp3";
    constexpr const char* cutscene_1_1 = "assets/music/cutscene_1_1.mp3";
    constexpr const char* cutscene_1_2 = "assets/music/cutscene_1_2.mp3";
    constexpr const char* cutscene_1_3 = "assets/music/cutscene_1_3.mp3";

    // sfx
    constexpr const char* ping = "assets/music/itemPickupSound.wav";
    constexpr const char* move = "assets/music/walkOnGrassSound.wav";
}

namespace texture {
    // character 
    constexpr const char* player_idle = "assets/character/MainCatIdle.png";
    constexpr const char* player_walk = "assets/character/MainCatWalk.png";

    // Asset
    constexpr const char* title_screen = "assets/ui/forg2kg.png";
    constexpr const char* cutscene_1_1 = "assets/cutscene/cutscene_1_1.png";
    constexpr const char* cutscene_1_2 = "assets/cutscene/cutscene_1_2.png";
    constexpr const char* cutscene_1_3 = "assets/cutscene/cutscene_1_3.png";
    constexpr const char* level1_exit_zone = "assets/ui/level1_exit_zone.png";

    // UI
    constexpr const char* start_button = "assets/ui/startButton.png";
    constexpr const char* load_button = "assets/ui/load_button.png";
    constexpr const char* save_button = "assets/ui/save_button.png";
}