#include "CutSceneState.h"
#include "Game.h"

std::vector<SDL_Texture*> Cutscene1State::getImages()  {
    return {
        core::textures->getTexture(texture::cutscene_1_1),
        core::textures->getTexture(texture::cutscene_1_2),
        core::textures->getTexture(texture::cutscene_1_3)
    };
}

std::vector<std::string> Cutscene1State::getAudioFiles() {
    return { audio::cutscene_1_1, audio::cutscene_1_2, audio::cutscene_1_3 };
}

std::vector<std::vector<std::pair<std::string, float>>> Cutscene1State::getSubtitles() {
    return {
        { {"Once upon a time, there were happy little cats.", 5.0f}, {"Living their life, showing their love to one another.", 8.0f} },
        { {"But one day, the zombie cats striked!", 5.0f}, {"Villages were pillaged and the princess now kidnapped.", 8.0f} },
        { {"Who would save her? Who would step up to the challenge?", 6.0f}, {"Let the story begin.", 8.0f} }
    };
}

void Cutscene1State::onCutsceneComplete(Game *game) {
    game->startLevel(1);
}

std::vector<SDL_Texture*> Cutscene2State::getImages()  {
    return {
        core::textures->getTexture(texture::cutscene_2_1),
        core::textures->getTexture(texture::cutscene_2_2)
    };
}

std::vector<std::string> Cutscene2State::getAudioFiles() {
    return { audio::cutscene_2_1, audio::cutscene_2_2};
}

std::vector<std::vector<std::pair<std::string, float>>> Cutscene2State::getSubtitles() {
    return {
        { {"The secret lies in the diary.", 6.0f}, {"He now knows the forbidden magic!", 6.0f} },
        { {"The potion shall eradicate the zombie!", 10.0f} }
    };
}

void Cutscene2State::onCutsceneComplete(Game *game) {
    game->startLevel(4);
}

std::vector<SDL_Texture*> Cutscene3State::getImages()  {
    return {
        core::textures->getTexture(texture::cutscene_3_1),
        core::textures->getTexture(texture::cutscene_3_2)
    };
}

std::vector<std::string> Cutscene3State::getAudioFiles() {
    return { audio::cutscene_3_1, audio::cutscene_3_2 };
}

std::vector<std::vector<std::pair<std::string, float>>> Cutscene3State::getSubtitles() {
    return {
        { {"Using the potent potion, all evils were cleansed.", 10.0f} },
        { {"His story shall come to an end, a happy one.", 10.0f} }
    };
}

void Cutscene3State::onCutsceneComplete(Game *game) {
    game->startLevel(10);
}

std::vector<SDL_Texture*> Cutscene4State::getImages()  {
    return {
        core::textures->getTexture(texture::cutscene_4_1),
        core::textures->getTexture(texture::cutscene_4_2)
    };
}

std::vector<std::string> Cutscene4State::getAudioFiles() {
    return { audio::cutscene_3_1, audio::cutscene_3_2 };
}

std::vector<std::vector<std::pair<std::string, float>>> Cutscene4State::getSubtitles() {
    return {
        { {"Oe oe.", 10.0f} },
        { {"deo lam nua.", 10.0f} }
    };
}

void Cutscene4State::onCutsceneComplete(Game *game) {
    game->changeState(StateFactory::createWinState());
}
