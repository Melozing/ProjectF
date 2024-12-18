// CollectibleItem.cpp
#include "Game1/Items/CollectibleItem.h"
#include "Controller/SpriteController.h"
#include "cocos2d.h"
USING_NS_CC;

bool CollectibleItem::init() {
    if (!Node::init()) {
        return false;
    }
    return true;
}

void CollectibleItem::spawn(const Vec2& startPosition) {
    auto fadeIn = FadeIn::create(0.5f);
    this->setOpacity(255);
    this->initAnimation();
    this->initPhysicsBody();
    _currentSprite->runAction(fadeIn);
    this->setVisible(true);
    // Define target position off-screen at the bottom
    Vec2 endPosition = Vec2(startPosition.x, -SpriteController::calculateScreenRatio(Constants::FALLINGROCK_ITEMS_OFFSET));

    // Calculate distance and duration based on speed
    float distance = startPosition.distance(endPosition);
    float duration = distance / _speed;

    // Move down action
    auto moveDown = MoveTo::create(duration, endPosition);

    // Callback to remove item when it moves off-screen
    auto removeItem = CallFunc::create([this]() {
        this->removeWhenOutOfScreen();
        });

    // Set initial position and run move action and remove when done
    this->setPosition(startPosition);
    this->runAction(Sequence::create(moveDown, removeItem, nullptr));
}

Size CollectibleItem::GetSize() const {
    return _currentSprite->getContentSize();
}

void CollectibleItem::update(float delta) {
    removeWhenOutOfScreen();
}

void CollectibleItem::removeWhenOutOfScreen() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (this->getPosition().y < -this->getContentSize().height - 50.0f) {
        this->returnToPool(); // Return to pool instead of removing
    }
}

void CollectibleItem::playEffectAndRemove() {
}

void CollectibleItem::returnToPool() {
    // Override this function in derived classes
}

Size CollectibleItem::getScaledSize() const {
    Size originalSize = _currentSprite->getContentSize();
    return Size(originalSize.width * _spriteScale, originalSize.height * _spriteScale);
}

void CollectibleItem::initAnimation() {
}

void CollectibleItem::initPhysicsBody() {
}