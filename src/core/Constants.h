#pragma once 

inline constexpr int SCREEN_WIDTH = 1200;
inline constexpr int SCREEN_HEIGHT = 750;
inline constexpr int TILE_SIZE = 32;

inline constexpr SDL_Rect CAMERA_DEADZONE = {
    SCREEN_WIDTH  * 4 / 10,
    SCREEN_HEIGHT * 4 / 10,
    SCREEN_WIDTH  * 2 / 10,
    SCREEN_HEIGHT * 2 / 10
};


inline const char* MAP_PATH_1 = "assets/maps/TransitionMap.json";
inline const char* MAP_PATH_2 = "assets/maps/Stage1_New.json";

namespace audio {
    // music
    constexpr const char* title = "assets/music/titleSoundtrack.mp3";
    constexpr const char* lv1m = "assets/music/level1-soundtrack.mp3";
    constexpr const char* cutscene_1_1 = "assets/music/cutscene_1_1.mp3";
    constexpr const char* cutscene_1_2 = "assets/music/cutscene_1_2.mp3";
    constexpr const char* cutscene_1_3 = "assets/music/cutscene_1_3.mp3";

    // sfx
    constexpr const char* ping = "assets/music/itemPickupSound.wav";
    constexpr const char* move = "assets/music/character_sound/walk_on_grass.wav";
    constexpr const char* growl = "assets/music/character_sound/zombie_growl.mp3";
    constexpr const char* zombie_eating = "assets/music/character_sound/zombie_eating_win.mp3";
}

namespace texture {
    // character 
    constexpr const char* player_idle = "assets/character/main_cat_idle.png";
    constexpr const char* player_walk = "assets/character/main_cat_walk.png";
    constexpr const char* zombie_idle = "assets/character/zombie_cat_idle.png";
    constexpr const char* zombie_walk = "assets/character/zombie_cat_walk.png";
    constexpr const char* zombie_stand_walk = "assets/character/zombie_cat_stand_walk.png";
    constexpr const char* player_dead = "assets/character/pirate_cat_idle.png";


    // Asset
    constexpr const char* title_screen = "assets/ui/forg2kg.png";
    constexpr const char* cutscene_1_1 = "assets/cutscene/cutscene_1_1.png";
    constexpr const char* cutscene_1_2 = "assets/cutscene/cutscene_1_2.png";
    constexpr const char* cutscene_1_3 = "assets/cutscene/cutscene_1_3.png";
    constexpr const char* level1_exit_zone = "assets/ui/level1_exit_zone.png";
    constexpr const char* death_screen = "assets/ui/death-screen.png";

    // UI
        //Button
    constexpr const char* start_button = "assets/ui/startButton.png";
    constexpr const char* load_button = "assets/ui/load_button.png";
    constexpr const char* save_button = "assets/ui/save_button.png";
    constexpr const char* resume_button = "assets/ui/resume_button.png";
    constexpr const char* quit_button = "assets/ui/quit_button.png";

        //Tutorial
    constexpr const char* wasd_onscreen = "assets/ui/wasd_onscreen.png";
    constexpr const char* esc_onscreen = "assets/ui/esc_onscreen.png";
}