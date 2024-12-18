#include "Scene/MainMenuScene.h"
#include "Controller/SceneController.h"
#include "Scene/LoadingScene.h"
#include "Controller/SpriteController.h"
#include "Controller/GameController.h"
#include "Controller/SoundController.h"
#include "Manager/BackgroundManager.h"

USING_NS_CC;

Scene* MainMenu::createScene() {
    return MainMenu::create();
}

bool MainMenu::init() {
    if (!Scene::init()) {
        return false; // Ensure initialization is successful
    }

    // Set Sound
    Constants::currentSoundTrackPath = Constants::MainMenuTrackPath;
    SoundController::getInstance()->playMusic(Constants::MainMenuTrackPath, true);

    // Set background
    BackgroundManager::getInstance()->setBackgroundWithOverlay(this, "assets_game/UXUI/Background/DienBienPhuVictory.jpg");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto sprite = Sprite::create("assets_game/UXUI/Buttons/Game_1.png");

    // Calculate positions
    float buttonSpacing = SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_BUTTONS_MAINMENU); // Adjust this value to set the spacing between buttons
    float buttonY = visibleSize.height / 2;
    float buttonG2Y = buttonY - buttonSpacing;
    float buttonG3Y = buttonG2Y - buttonSpacing;

    // Create and position button
    auto button = ui::Button::create("assets_game/UXUI/Buttons/Game_1.png");
    button->setScale(SpriteController::updateSpriteScale(sprite, 0.08f));
    button->setPosition(Vec2(visibleSize.width / 2, buttonY));
    button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            SoundController::getInstance()->stopMusic(Constants::currentSoundTrackPath);
            SoundController::getInstance()->playSoundEffect(Constants::ClickStartGameSFX);
            startLoading(Constants::GAME1_SCENE_NAME); // Start loading when button is pressed
        }
        });
    this->addChild(button);

    // Create and position buttonG2
    auto buttonG2 = ui::Button::create("assets_game/UXUI/Buttons/Game_2.png");
    buttonG2->setScale(SpriteController::updateSpriteScale(sprite, 0.08f));
    buttonG2->setPosition(Vec2(visibleSize.width / 2, buttonG2Y));
    buttonG2->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            SoundController::getInstance()->stopMusic(Constants::currentSoundTrackPath);
            SoundController::getInstance()->playSoundEffect(Constants::ClickStartGameSFX);
            startLoading(Constants::GAME2_SCENE_NAME); // Start loading when button is pressed
        }
        });
    this->addChild(buttonG2);

    // Create and position buttonG3
    auto buttonG3 = ui::Button::create("assets_game/UXUI/Buttons/Game_3.png");
    buttonG3->setScale(SpriteController::updateSpriteScale(sprite, 0.08f));
    buttonG3->setPosition(Vec2(visibleSize.width / 2, buttonG3Y));
    buttonG3->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            SoundController::getInstance()->stopMusic(Constants::currentSoundTrackPath);
            SoundController::getInstance()->playSoundEffect(Constants::ClickStartGameSFX);
            startLoading(Constants::GAME3_SCENE_NAME); // Start loading when button is pressed
        }
        });
    this->addChild(buttonG3);

    // Create and position exit button
    auto exitButton = ui::Button::create("assets_game/UXUI/Panel/Close_BTN.png", "assets_game/UXUI/Panel/Exit_BTN.png");
    exitButton->setScale(SpriteController::updateSpriteScale(exitButton, 0.08f));
    exitButton->setPosition(Vec2(visibleSize.width / 2, exitButton->getContentSize().height / 2 + 10)); // Position at the bottom
    exitButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            SoundController::getInstance()->stopMusic(Constants::currentSoundTrackPath);
            SoundController::getInstance()->playSoundEffect(Constants::ButtonClickSFX);
            Director::getInstance()->end(); // Exit the game
        }
        });
    this->addChild(exitButton);

    // Create and add the custom cursor
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
    _cursor = Cursor::create("assets_game/UXUI/Main_Menu/pointer.png");
    _cursor->setAnchorPoint(Vec2(0.5, 0.5));
    _cursor->setScale(SpriteController::updateSpriteScale(_cursor, 0.03f));
    _cursor->setInitialPosition();
    if (_cursor) {
        this->addChild(_cursor, Constants::ORDER_LAYER_CURSOR); // Add cursor to the scene with z-order 1
    }

    return true; // Initialization succeeded
}

void MainMenu::startLoading(std::string nameScene) {
    auto loadingScene = LoadingScene::createScene(nameScene);
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, loadingScene));
}
