#include "LevelState.h"
#include "Constants.h"
#include "Game.h"

// Concrete level implementations - just need to specify their unique data
const char* Level1State::getMapPath() {
    return MAP_PATH[getLevelNumber()];
}

const char* Level1State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level1State::getLevelNumber() {
    return 1;
}

const char* Level2State::getMapPath() {
    return MAP_PATH[getLevelNumber()];
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
    return MAP_PATH[getLevelNumber()];
}

const char* Level3State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level3State::getLevelNumber() {
    return 3;
}

const char* Level4State::getMapPath() {
    return MAP_PATH[4];
}

const char* Level4State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level4State::getLevelNumber() {
    return 4;
}

const char* Level5State::getMapPath() {
    return MAP_PATH[5];
}

const char* Level5State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level5State::getLevelNumber() {
    return 5;
}

const char* Level6State::getMapPath() {
    return MAP_PATH[getLevelNumber()];
}

const char* Level6State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level6State::getLevelNumber() {
    return 6;
}

const char* Level7State::getMapPath() {
    return MAP_PATH[getLevelNumber()];
}

const char* Level7State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level7State::getLevelNumber() {
    return 7;
}

const char* Level8State::getMapPath() {
    return MAP_PATH[getLevelNumber()];
}

const char* Level8State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level8State::getLevelNumber() {
    return 8;
}

const char* Level9State::getMapPath() {
    return MAP_PATH[getLevelNumber()];
}

const char* Level9State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level9State::getLevelNumber() {
    return 9;
}
const char* Level10State::getMapPath() {
    return MAP_PATH[getLevelNumber()];
}

const char* Level10State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level10State::getLevelNumber() {
    return 10;
}
const char* Level11State::getMapPath() {
    return MAP_PATH[getLevelNumber()];
}

const char* Level11State::getBackgroundMusic() {
    return audio::lv1m;
}

int Level11State::getLevelNumber() {
    return 11;
}