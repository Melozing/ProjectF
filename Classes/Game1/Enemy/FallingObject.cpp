#include "FallingObject.h"
#include "cocos2d.h"

USING_NS_CC;

bool FallingObject::init() {
    if (!Sprite::init()) {
        return false;
    }
    return true;
}

void FallingObject::spawn(const Vec2& startPosition) {
    this->setPosition(startPosition);
    this->setVisible(true);

    Vec2 endPosition = Vec2(startPosition.x, -SpriteController::calculateScreenRatio(Constants::FALLINGROCK_ITEMS_OFFSET));
    float distance = startPosition.distance(endPosition);
    float duration = distance / _speed;

    auto moveDown = MoveTo::create(duration, endPosition);
    auto removeObject = CallFunc::create([this]() {
        this->returnToPool();
        });

    this->runAction(Sequence::create(moveDown, removeObject, nullptr));
}

void FallingObject::reset() {
    this->setVisible(false);
    this->removeFromParentAndCleanup(false);
}

void FallingObject::returnToPool() {
    this->reset();
    // Derived classes should implement their own return to pool logic
}

Size FallingObject::GetSize() {
    return GetContentSizeSprite(_currentSprite);
}