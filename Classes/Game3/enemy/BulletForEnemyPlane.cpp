#include "BulletForEnemyPlane.h"
#include "Manager/ObjectPoolGame3.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "Constants/Constants.h"
#include "FX/Explodable.h"
#include "cocos2d.h"

USING_NS_CC;

BulletForEnemyPlane* BulletForEnemyPlane::create() {
    BulletForEnemyPlane* bullet = new (std::nothrow) BulletForEnemyPlane();
    if (bullet && bullet->init()) {
        bullet->autorelease();
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

bool BulletForEnemyPlane::init() {
    if (!Sprite::init()) {
        return false;
    }

    modelCharac = Sprite::create("assets_game/enemies/BulletPlane.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.03f));
    this->addChild(modelCharac);

    this->createPhysicsBody();

    // Initialize explosion batch node
    explosionBatchNode = SpriteBatchNode::create("assets_game/fx/explosions.png");
    this->addChild(explosionBatchNode);

    return true;
}

Size BulletForEnemyPlane::GetSize() {
    return SpriteController::GetContentSizeSprite(modelCharac);
}

void BulletForEnemyPlane::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsBody = PhysicsBody::createBox(this->GetSize() * 0.4f);

    physicsBody->setDynamic(false);
    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    this->setPhysicsBody(physicsBody);
}

void BulletForEnemyPlane::reset() {
    modelCharac->setVisible(true);
    this->setVisible(true);
    this->setRotation(0);
}

// BulletForEnemyPlane.cpp

void BulletForEnemyPlane::moveDown(float angle) {
    this->createPhysicsBody();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Calculate the target position within the screen bounds
    float targetX = this->getPositionX() + visibleSize.height * tan(CC_DEGREES_TO_RADIANS(angle));
    float targetY = origin.y;

    // Ensure the target position is within the screen bounds
    targetX = std::max(origin.x, std::min(targetX, origin.x + visibleSize.width));

    Vec2 targetPosition = Vec2(targetX, targetY);
    float moveDuration = 0.8f;

    // Rotate the bullet sprite to match the angle
    this->setRotation(-angle - 90); // Adjust rotation to match the sprite's initial orientation

    auto moveAction = MoveTo::create(moveDuration, targetPosition);
    auto sequence = Sequence::create(moveAction, CallFunc::create([this]() {
        this->returnToPool();
        }), nullptr);
    this->runAction(sequence);
}

void BulletForEnemyPlane::explode() {
    // Check if the bullet is still part of the scene
    if (!this->getParent()) {
        return;
    }

    // Stop all actions to prevent further movement
    this->stopAllActions();

    // Remove physics body if it exists
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    // Hide the model character
    modelCharac->setVisible(false);
    SoundController::getInstance()->playSoundEffect(Constants::EnemyCrepExplodeSFX);

    // Create explosion effect
    auto explosion = Explosion::create(this->getPosition(), [this]() {
        this->returnToPool();
        });
    this->getParent()->addChild(explosion);
}

void BulletForEnemyPlane::returnToPool() {
    this->stopAllActions();
    this->unscheduleAllCallbacks();
    this->setVisible(false);
    this->removeFromParentAndCleanup(false);
    this->reset();
    BulletForEnemyPlanePool::getInstance()->returnObject(this);
}
