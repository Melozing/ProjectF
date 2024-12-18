#include "FallingRock.h"
#include "Manager/ObjectPoolGame1.h"
#include "Controller/SpriteController.h"
#include "utils/PhysicsShapeCache.h"
#include "cocos2d.h"

USING_NS_CC;

FallingRock* FallingRock::create() {
    FallingRock* ret = new (std::nothrow) FallingRock();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FallingRock::init() {
    if (!Sprite::init()) { // Assuming Sprite is the superclass
        return false;
    }

    initSprite();
    return true;
}

void FallingRock::reset() {
    this->setVisible(false);
}

Size FallingRock::GetSize() {
    return _currentSprite->getContentSize();
}

void FallingRock::initSprite() {
    // Randomly select between two images
    std::string spriteFrameName = (CCRANDOM_0_1() < 0.5f) ? "assets_game/enemies/FallingRock1.png" : "assets_game/enemies/FallingRock2.png";

    // Create the sprite
    _currentSprite = Sprite::create(spriteFrameName);
    this->addChild(_currentSprite);

    // Scale the sprite
    _currentSprite->setScale(SpriteController::updateSpriteScale(_currentSprite, 0.13f));

    // Create physics body
    createPhysicsBody();
}

void FallingRock::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = _currentSprite->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    // Determine the correct plist file and body name based on the current sprite
    std::string plistFile, bodyName;
    if (_currentSprite->getResourceName() == "assets_game/enemies/FallingRock1.png") {
        plistFile = "physicsBody/FallingRock1.plist";
        bodyName = "FallingRock1";
    }
    else {
        plistFile = "physicsBody/FallingRock2.plist";
        bodyName = "FallingRock2";
    }

    auto physicsBody = physicsCache->createBodyFromPlist(plistFile, bodyName, originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, bodyName, originalSize, 0.125f);

    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setCollisionBitmask(0x02);
        this->setPhysicsBody(physicsBody);
    }
}

void FallingRock::initAnimation() {
    // No animation needed
}

void FallingRock::spawn(const Vec2& startPosition) {
    this->setPosition(startPosition);
    this->setVisible(true);

    // Define target position off-screen at the bottom
    Vec2 endPosition = Vec2(startPosition.x, -SpriteController::calculateScreenRatio(Constants::FALLINGROCK_ITEMS_OFFSET));

    // Calculate distance and duration based on speed
    float distance = startPosition.distance(endPosition);
    float duration = distance / _speed;

    // Move down action
    auto moveDown = MoveTo::create(duration, endPosition);

    // Callback to remove rock when it moves off-screen
    auto removeRock = CallFunc::create([this]() {
        this->returnToPool();
        });

    // Run move action and remove when done
    this->runAction(Sequence::create(moveDown, removeRock, nullptr));
}

void FallingRock::returnToPool() {
    this->setVisible(false);
    this->removeFromParentAndCleanup(false);
    FallingRockPool::getInstance()->returnObject(this);
}
