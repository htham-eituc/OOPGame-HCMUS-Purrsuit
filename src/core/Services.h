// core/Services.h
#pragma once
#include "AudioManager.h"
#include "TextureManager.h"
#include "UIManager.h"
#include "UIInputHandler.h"
#include "UIRenderer.h"
namespace core {
    inline AudioManager* audio = nullptr;
    inline TextureManager* textures = nullptr;
    
    inline UIManager* ui = nullptr;
    inline UIInputHandler* uiInput = nullptr;
    inline UIRenderer* uiRenderer = nullptr;
    // Future:
    // inline InputManager* input = nullptr;
    // inline EventBus* events = nullptr;
}
