#include "PausePanel.h"
#include "Button/ReplayButton.h"
#include "Button/ExitButton.h"
#include "Scene/LoadingScene.h"
#include "Constants/Constants.h"
#include "Controller/SoundController.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

PausePanel* PausePanel::createPanel(const std::function<void()>& resumeCallback, const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction) {
    PausePanel* panel = new (std::nothrow) PausePanel();
    if (panel && panel->init(resumeCallback, retryAction, exitAction, backAction)) {
        panel->autorelease();
        return panel;
    }
    delete panel;
    return nullptr;
}

bool PausePanel::init() {
    if (!BasePanel::init()) {
        return false;
    }
    return true;
}

bool PausePanel::init(const std::function<void()>& resumeCallback, const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction) {
    if (!BasePanel::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create exit button
    auto exitButton = ui::Button::create("assets_game/UXUI/Panel/Close_BTN.png", "assets_game/UXUI/Panel/Close_BTN.png");
    exitButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    exitButton->setPosition(Vec2(boardSprite->getContentSize().width - exitButton->getContentSize().width / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_EXIT_BUTTON),
        boardSprite->getContentSize().height - exitButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_EXITBUTTON)));
    exitButton->addClickEventListener([this, resumeCallback](Ref* sender) {
        playSoundAndExecuteCallback(resumeCallback);
        });
    boardSprite->addChild(exitButton);

    // Create resume button
    auto resumeButton = ui::Button::create("assets_game/UXUI/Panel/Play_BTN.png", "assets_game/UXUI/Panel/Play_BTN_Active.png");
    resumeButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    resumeButton->setPosition(Vec2(boardSprite->getContentSize().width / 2 - SpriteController::calculateScreenRatio(0.04f),
        resumeButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_PANEL)));
    resumeButton->addClickEventListener([this, resumeCallback](Ref* sender) {
        playSoundAndExecuteCallback(resumeCallback);
        });
    boardSprite->addChild(resumeButton);

    // Create retry button
    auto retryButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png");
    retryButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    retryButton->setPosition(Vec2(resumeButton->getPosition().x + retryButton->getContentSize().width + SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_PANEL_BUTTON),
        retryButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_PANEL)));
    retryButton->addClickEventListener([this, retryAction](Ref* sender) {
        playSoundAndExecuteCallback(retryAction);
        });
    boardSprite->addChild(retryButton);

    // Create back button
    auto backButton = ui::Button::create("assets_game/UXUI/Panel/Back_BTN.png", "assets_game/UXUI/Panel/Back_BTN_Active.png");
    backButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    backButton->setPosition(Vec2(resumeButton->getPosition().x - backButton->getContentSize().width - SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_PANEL_BUTTON),
        backButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_PANEL)));
    backButton->addClickEventListener([this, backAction](Ref* sender) {
        playSoundAndExecuteCallback(backAction);
        });
    boardSprite->addChild(backButton);

    // Create music slider
    musicSlider = ui::Slider::create();
    musicSlider->loadBarTexture("assets_game/UXUI/Panel/Slider_Back.png");
    musicSlider->loadSlidBallTextures("assets_game/UXUI/Panel/SliderNode_Normal.png", "assets_game/UXUI/Panel/SliderNode_Pressed.png", "assets_game/UXUI/Panel/SliderNode_Disabled.png");
    musicSlider->loadProgressBarTexture("assets_game/UXUI/Panel/Slider_PressBar.png");

    // Adjust the size of the ProgressBar
    auto progressBarSize = musicSlider->getContentSize();
    progressBarSize.width *= 0.5f; // Adjust the width to be 90% of the original
    musicSlider->setContentSize(progressBarSize);

    // Restore the saved value
    float savedMusicVolume = UserDefault::getInstance()->getFloatForKey(Constants::UD_musicVolume.c_str(), 0.5f);
    musicSlider->setPercent(savedMusicVolume * 100);

    musicSlider->setPosition(Vec2(boardSprite->getContentSize().width / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_SLIDER), boardSprite->getContentSize().height / 2 - SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_SLIDER)));
    musicSlider->addEventListener([this](Ref* sender, ui::Slider::EventType type) {
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            float volume = musicSlider->getPercent() / 100.0f;
            SoundController::getInstance()->setMusicVolume(volume);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_musicVolume.c_str(), volume); // Save the value
            updateMusicButtonState();
        }
        });
    boardSprite->addChild(musicSlider);

    // Create sound effect slider
    soundEffectSlider = ui::Slider::create();
    soundEffectSlider->loadBarTexture("assets_game/UXUI/Panel/Slider_Back.png");
    soundEffectSlider->loadSlidBallTextures("assets_game/UXUI/Panel/SliderNode_Normal.png", "assets_game/UXUI/Panel/SliderNode_Normal.png", "assets_game/UXUI/Panel/SliderNode_Normal.png");
    soundEffectSlider->loadProgressBarTexture("assets_game/UXUI/Panel/Slider_PressBar.png");

    // Adjust the size of the ProgressBar
    auto soundEffectProgressBarSize = soundEffectSlider->getContentSize();
    soundEffectProgressBarSize.width *= 0.5f; // Adjust the width to be 90% of the original
    soundEffectSlider->setContentSize(soundEffectProgressBarSize);

    // Restore the saved value
    float savedEffectsVolume = UserDefault::getInstance()->getFloatForKey(Constants::UD_effectsVolume.c_str(), 0.5f);
    soundEffectSlider->setPercent(savedEffectsVolume * 100);

    soundEffectSlider->setPosition(Vec2(musicSlider->getPosition().x, musicSlider->getPosition().y - SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_SLIDER_SPACING)));
    soundEffectSlider->addEventListener([this](Ref* sender, ui::Slider::EventType type) {
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            float volume = soundEffectSlider->getPercent() / 100.0f;
            SoundController::getInstance()->setEffectsVolume(volume);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_effectsVolume.c_str(), volume); // Save the value
            updateSoundEffectButtonState();
        }
        });
    boardSprite->addChild(soundEffectSlider);

    // Create music button
    musicButton = ui::Button::create("assets_game/UXUI/Panel/Music_BTN_Active.png", "assets_game/UXUI/Panel/Music_BTN_Active.png");
    musicButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    musicButton->setPosition(Vec2(boardSprite->getContentSize().width / 2 - musicSlider->getContentSize().width / 2 - SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_SLIDER_SPACING),
        musicSlider->getPosition().y));
    musicButton->addClickEventListener([this](Ref* sender) {
        if (musicSlider->getPercent() == 0) {
            musicSlider->setPercent(100);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_musicVolume.c_str(), 100);
            SoundController::getInstance()->setMusicVolume(1.0f);
        }
        else {
            musicSlider->setPercent(0);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_musicVolume.c_str(), 0);
            SoundController::getInstance()->setMusicVolume(0);
        }
        updateMusicButtonState();
        });

    boardSprite->addChild(musicButton);

    // Create sound effect button
    soundEffectButton = ui::Button::create("assets_game/UXUI/Panel/SoundEffect_BTN_Active.png", "assets_game/UXUI/Panel/SoundEffect_BTN_Active.png");
    soundEffectButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    soundEffectButton->setPosition(Vec2(musicButton->getPosition().x, soundEffectSlider->getPosition().y));
    soundEffectButton->addClickEventListener([this](Ref* sender) {
        if (soundEffectSlider->getPercent() == 0) {
            soundEffectSlider->setPercent(100);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_effectsVolume.c_str(), 100);
            SoundController::getInstance()->setEffectsVolume(1.0f);
        }
        else {
            soundEffectSlider->setPercent(0);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_effectsVolume.c_str(), 0);
            SoundController::getInstance()->setEffectsVolume(0);
        }
        updateSoundEffectButtonState();
        });

    boardSprite->addChild(soundEffectButton);

    // Update button states based on initial slider values
    updateSlidersAndButtons();

    this->addTitleLabel("Setting");

    return true;
}

void PausePanel::playSoundAndExecuteCallback(const std::function<void()>& callback) {
    SoundController::getInstance()->playSoundEffect(Constants::ButtonClickSFX);
    if (callback) {
        callback();
    }
}

void PausePanel::updateMusicButtonState() {
    if (musicSlider->getPercent() == 0) {
        musicButton->loadTextures("assets_game/UXUI/Panel/Music_BTN_Disabled.png", "assets_game/UXUI/Panel/Music_BTN_Disabled.png");
    }
    else {
        musicButton->loadTextures("assets_game/UXUI/Panel/Music_BTN_Active.png", "assets_game/UXUI/Panel/Music_BTN_Active.png");
    }
}

void PausePanel::updateSoundEffectButtonState() {
    if (soundEffectSlider->getPercent() == 0) {
        soundEffectButton->loadTextures("assets_game/UXUI/Panel/SoundEffect_BTN_Disabled.png", "assets_game/UXUI/Panel/SoundEffect_BTN_Disabled.png");
    }
    else {
        soundEffectButton->loadTextures("assets_game/UXUI/Panel/SoundEffect_BTN_Active.png", "assets_game/UXUI/Panel/SoundEffect_BTN_Active.png");
    }
}

void PausePanel::updateSlidersAndButtons() {
    float savedMusicVolume = UserDefault::getInstance()->getFloatForKey(Constants::UD_musicVolume.c_str(), 0.5f);
    float savedEffectsVolume = UserDefault::getInstance()->getFloatForKey(Constants::UD_effectsVolume.c_str(), 0.5f);

    musicSlider->setPercent(savedMusicVolume * 100);
    soundEffectSlider->setPercent(savedEffectsVolume * 100);

    updateMusicButtonState();
    updateSoundEffectButtonState();
}
