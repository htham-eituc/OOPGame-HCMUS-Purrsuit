#include "LevelState.h"
#include "Constants.h"
// Concrete level implementations - just need to specify their unique data
const char* Level1State::getMapPath() {
    return MAP_PATH_1;
}

const char* Level1State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level1State::getLevelNumber() {
    return 1;
}

const char* Level2State::getMapPath() {
    return MAP_PATH_2;
}

const char* Level2State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level2State::getLevelNumber() {
    return 2;
}


const char* Level3State::getMapPath() {
    return MAP_PATH_3;
}

const char* Level3State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level3State::getLevelNumber() {
    return 3;
}