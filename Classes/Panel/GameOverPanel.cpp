#include "Panel/GameOverPanel.h"
#include "Button/ReplayButton.h"
#include "Button/ExitButton.h"
#include "ui/CocosGUI.h"
#include "Controller/GameController.h"
#include "Constants/Constants.h"

USING_NS_CC;

GameOverPanel* GameOverPanel::createPanel(const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction) {
    GameOverPanel* panel = new (std::nothrow) GameOverPanel();
    if (panel && panel->init(retryAction, exitAction, backAction)) {
        panel->autorelease();
        return panel;
    }
    delete panel;
    return nullptr;
}

bool GameOverPanel::init(const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction) {
    if (!BasePanel::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create retry button
    auto retryButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png");
    retryButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    retryButton->setPosition(Vec2(boardSprite->getContentSize().width / 2 + SpriteController::calculateScreenRatio(0.04f),
        retryButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_PANEL)));
    retryButton->addClickEventListener([retryAction](Ref* sender) {
        retryAction();
        });
    boardSprite->addChild(retryButton);

    // Create back button
    auto backButton = ui::Button::create("assets_game/UXUI/Panel/Back_BTN.png", "assets_game/UXUI/Panel/Back_BTN_Active.png");
    backButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    backButton->setPosition(Vec2(retryButton->getPosition().x - backButton->getContentSize().width - SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_PANEL_BUTTON),
        backButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_PANEL)));
    backButton->addClickEventListener([backAction](Ref* sender) {
        backAction();
        });
    boardSprite->addChild(backButton);

    // Create exit button
    auto exitButton = cocos2d::Sprite::create("assets_game/UXUI/Panel/Close_BTN.png");
    exitButton->setPosition(Vec2(boardSprite->getContentSize().width - exitButton->getContentSize().width / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_EXIT_BUTTON),
        boardSprite->getContentSize().height - exitButton->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_EXITBUTTON)));
    boardSprite->addChild(exitButton);

    this->addTitleLabel("Game Over");
    return true;
}