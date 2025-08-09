#pragma once
#include <memory>
#include <vector>
#include "GameStateBase.h"
#include "Services.h"
#include "Constants.h"
#include "Game.h"

class UILabel;

class CutsceneState : public GameStateBase {
public:
    void enter(Game* game) override;
    void exit(Game* game) override;
    void handleEvent(Game* game, const SDL_Event& event) override;
    void update(Game* game, float deltaTime) override;
    void render(Game* game) override;

protected:
    // Template method pattern for cutscene-specific data
    virtual std::vector<SDL_Texture*> getImages() = 0;
    virtual std::vector<std::string> getAudioFiles() = 0;
    virtual std::vector<std::vector<std::pair<std::string, float>>> getSubtitles() = 0;
    virtual void onCutsceneComplete(Game* game) = 0;
    
    // Common cutscene data
    std::shared_ptr<UILabel> subtitleLabel;
    int currentImageIndex = 0;
    int currentSubtitleIndex = 0;
    float subtitleTimer = 0.0f;
    float zoom = 1.0f;
    const int maxImages = 3; // or make this virtual too
    
    // Shared helper methods
    void setupSubtitleLabel();
    void advanceToNextImage(Game* game);
    void updateSubtitles(float deltaTime);
    void renderCurrentImage(Game* game);
};

class Cutscene1State : public CutsceneState {
protected:
    std::vector<SDL_Texture*> getImages() override {
        return {
            core::textures->getTexture(texture::cutscene_1_1),
            core::textures->getTexture(texture::cutscene_1_2),
            core::textures->getTexture(texture::cutscene_1_3)
        };
    }
    
    std::vector<std::string> getAudioFiles() override {
        return { audio::cutscene_1_1, audio::cutscene_1_2, audio::cutscene_1_3 };
    }
    
    std::vector<std::vector<std::pair<std::string, float>>> getSubtitles() override {
        return {
            { {"He came from the shadows.", 3.0f}, {"But the world wasn't ready for him.", 6.0f} },
            { {"They feared what they couldn't understand.", 5.0f} },
            { {"Yet, silence could no longer protect them.", 5.0f} }
        };
    }
    
    void onCutsceneComplete(Game* game) override {
        game->startLevel1();
    }
};