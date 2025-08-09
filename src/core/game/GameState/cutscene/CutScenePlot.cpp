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
        { {"He came from the shadows.", 3.0f}, {"But the world wasn't ready for him.", 6.0f} },
        { {"They feared what they couldn't understand.", 5.0f} },
        { {"Yet, silence could no longer protect them.", 5.0f} }
    };
}

void Cutscene1State::onCutsceneComplete(Game *game) {
    game->startLevel(1);
}
