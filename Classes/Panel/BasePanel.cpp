#include "BasePanel.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

bool BasePanel::init() {
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Create a semi-transparent background
    auto background = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(background);

    // Create board sprite
    boardSprite = Sprite::create("assets_game/UXUI/Panel/Board.png");
    boardSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    boardSprite->setScale(SpriteController::updateSpriteScale(boardSprite, 0.525f));
    boardSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + SpriteController::calculateScreenRatio(0.03f)));
    this->addChild(boardSprite);

    return true;
}

void BasePanel::addTitleLabel(const std::string& titleText) {
    // Create title label
    auto titleLabel = Label::createWithTTF(titleText, Constants::FONT_GAME, 27);
    titleLabel->setAnchorPoint(Vec2(0.5f, 0.5f));

    // Calculate the position to be at the top center of the boardSprite
    auto boardSpriteSize = boardSprite->getContentSize();
    auto boardSpriteScale = boardSprite->getScale();
    auto titleLabelPosition = Vec2(
        boardSprite->getPositionX() - SpriteController::calculateScreenRatio(0.003f),
        boardSprite->getPositionY() + (boardSpriteSize.height * boardSpriteScale / 2) - SpriteController::calculateScreenRatio(0.07f)
    );

    titleLabel->setPosition(titleLabelPosition);
    this->addChild(titleLabel, Constants::ORDER_LAYER_UI);
}

