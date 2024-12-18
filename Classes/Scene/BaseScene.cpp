#include "BaseScene.h"
#include "Controller/GameController.h"
#include "Controller/SceneController.h"
#include "Controller/SoundController.h"
#include "Constants/Constants.h"

USING_NS_CC;

bool BaseScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // Add keyboard event listener for pause and resume functionality
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
            if (GameController::getInstance()->isPaused()) return;
            if (!GameController::getInstance()->isGameOver()) {
                if (_cursor)
                {
                    _cursor->setVisible(false);
                }
                auto exitAction = []() {
                    Director::getInstance()->end();
                    };
                std::string soundtrackPath = Constants::currentSoundTrackPath;
                SoundController::getInstance()->playSoundEffect(Constants::ClickStartGameSFX);
                GameController::getInstance()->pauseGame(exitAction, _sceneCreationFunc, soundtrackPath);
            }
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
            if (GameController::getInstance()->isPaused()) {
                if (_cursor)
                {
                    _cursor->setVisible(true);
                }
                SoundController::getInstance()->playSoundEffect(Constants::ButtonClickSFX);
                GameController::getInstance()->resumeGame();
            }
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void BaseScene::setSceneCreationFunc(const std::function<cocos2d::Scene* ()>& func) {
    _sceneCreationFunc = func;
}
