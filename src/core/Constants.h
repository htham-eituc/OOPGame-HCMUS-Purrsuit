#pragma once 
#include <SDL.h>

inline constexpr int SCREEN_WIDTH = 900;
inline constexpr int SCREEN_HEIGHT = 600;
inline constexpr int TILE_SIZE = 32;

inline constexpr SDL_Rect CAMERA_DEADZONE = {
    SCREEN_WIDTH  * 4 / 10,
    SCREEN_HEIGHT * 4 / 10,
    SCREEN_WIDTH  * 2 / 10,
    SCREEN_HEIGHT * 2 / 10
};


inline const char* MAP_PATH[] = {"","assets/maps/Level1_TransitionMap1.json",
                                    "assets/maps/Level2_Stage1.json",
                                    "assets/maps/Level3_House.json",
                                    "assets/maps/Level4_House_after.json",
                                    "assets/maps/Level5_Stage1_after.json",
                                    "assets/maps/Level6_Stage2.json",
                                    "assets/maps/Level7_Stage3.json",
                                    "assets/maps/Level8_TransitionMap2.json",
                                    "assets/maps/Level9_Stage4.json",
                                    "assets/maps/Level10_Stage4_after.json",
                                    "assets/maps/Level11_Castle.json",
                                };

namespace audio {
    // music
    constexpr const char* title = "assets/music/titleSoundtrack.mp3";
    constexpr const char* lv1m = "assets/music/level1-soundtrack.mp3";
    constexpr const char* lv4m = "assets/music/level4_soundtrack.mp3";
    constexpr const char* lv6m = "assets/music/level6_soundtrack.mp3";
    constexpr const char* lv7m = "assets/music/level7_soundtrack.mp3";
    constexpr const char* lv8m = "assets/music/level8_soundtrack.mp3";
    constexpr const char* lv9m = "assets/music/level9_soundtrack.mp3";
    constexpr const char* lv10m = "assets/music/level10_soundtrack.mp3";
    constexpr const char* lv11m = "assets/music/level11_soundtrack.mp3";
    constexpr const char* cutscene_1_1 = "assets/music/cutscene_1_1.mp3";
    constexpr const char* cutscene_1_2 = "assets/music/cutscene_1_2.mp3";
    constexpr const char* cutscene_1_3 = "assets/music/cutscene_1_3.mp3";
    constexpr const char* cutscene_2_1 = "assets/music/cutscene_2_1.mp3";
    constexpr const char* cutscene_2_2 = "assets/music/cutscene_2_2.mp3";
    constexpr const char* cutscene_3_1 = "assets/music/cutscene_3_1.mp3";
    constexpr const char* cutscene_3_2 = "assets/music/cutscene_2_2.mp3";

    // sfx
    constexpr const char* ping = "assets/music/itemPickupSound.wav";
    constexpr const char* move = "assets/music/character_sound/walk_on_grass.wav";
    constexpr const char* growl = "assets/music/character_sound/zombie_growl.mp3";
    constexpr const char* zombie_eating = "assets/music/character_sound/zombie_eating.mp3";
    constexpr const char* meow = "assets/music/character_sound/meow.mp3";
    constexpr const char* inventory = "assets/music/inventory.wav";
}

namespace texture {
    // character 
    constexpr const char* player_idle = "assets/character/main_cat_idle.png";
    constexpr const char* player_walk = "assets/character/main_cat_walk.png";
    constexpr const char* zombie_idle = "assets/character/zombie_cat_idle.png";
    constexpr const char* zombie_walk = "assets/character/zombie_cat_walk.png";
    constexpr const char* zombie_stand_walk = "assets/character/zombie_cat_stand_walk.png";
    constexpr const char* player_dead = "assets/character/main_cat_idle.png";
    constexpr const char* princess_idle = "assets/character/rin_cat_idle.png";


    // Asset
    constexpr const char* title_screen = "assets/ui/forg2kg.png";
    constexpr const char* cutscene_1_1 = "assets/cutscene/cutscene_1_1.png";
    constexpr const char* cutscene_1_2 = "assets/cutscene/cutscene_1_2.png";
    constexpr const char* cutscene_1_3 = "assets/cutscene/cutscene_1_3.png";
    constexpr const char* cutscene_2_1 = "assets/cutscene/cutscene_2_1.png";
    constexpr const char* cutscene_2_2 = "assets/cutscene/cutscene_2_2.png";
    constexpr const char* cutscene_3_1 = "assets/cutscene/cutscene_3_1.png";
    constexpr const char* cutscene_3_2 = "assets/cutscene/cutscene_3_2.png";
    constexpr const char* level1_exit_zone = "assets/ui/level1_exit_zone.png";
    constexpr const char* death_screen = "assets/ui/death-screen.png";
    constexpr const char* win_screen = "assets/ui/win-screen.png";

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
    constexpr const char* t_onscreen = "assets/ui/t_onscreen.png";
    constexpr const char* i_onscreen = "assets/ui/i_onscreen.png";
    constexpr const char* items = "assets/tiles/map1/items.png";

        //Mouse
    constexpr const char* mouse_default = "assets/ui/mouse_default.png";
    constexpr const char* mouse_on_click = "assets/ui/mouse_on_click.png";   
}