#include "LevelState.h"
#include "Constants.h"
#include "Game.h"

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

void Level3State::exit(Game *game) {
    game->getInventory()->removeItem("Key");
}

const char *Level3State::getMapPath()
{
    return MAP_PATH_3;
}

const char* Level3State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level3State::getLevelNumber() {
    return 3;
}

const char* Level4State::getMapPath() {
    return MAP_PATH_4;
}

const char* Level4State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level4State::getLevelNumber() {
    return 4;
}

const char* Level5State::getMapPath() {
    return MAP_PATH_5;
}

const char* Level5State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level5State::getLevelNumber() {
    return 5;
}