#include "Manager/ObjectPoolGame3.h"
#include "Bullet/BulletPlayerGame3.h"
#include "Constants/Constants.h"
#include "utils/PhysicsShapeCache.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "FX/Explodable.h"

USING_NS_CC;

BulletPlayerGame3* BulletPlayerGame3::create()
{
    BulletPlayerGame3* bullet = new (std::nothrow) BulletPlayerGame3();
    if (bullet && bullet->init()) {
        bullet->autorelease();
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

bool BulletPlayerGame3::init()
{
    if (!Sprite::init()) {
        return false;
    }

    initAnimation();

    this->setSpeed(Constants::BulletGame3Speed);

    return true;
}

void BulletPlayerGame3::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/player/BulletPlayer3Game.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode, Constants::ORDER_LAYER_PLAYER - 1);
    }

    modelCharac = Sprite::createWithSpriteFrameName("BulletPlayer3Game1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.1f)); // Adjust scale as needed
    spriteBatchNode->addChild(modelCharac, Constants::ORDER_LAYER_PLAYER - 1);

    auto animateCharac = Animate::create(SpriteController::createAnimation("BulletPlayer3Game", 31, 0.005f)); // Adjust frame count and duration as needed
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void BulletPlayerGame3::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/BulletPlayer3Game.plist");

    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBody("BulletPlayer3Game", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "BulletPlayer3Game", originalSize, 0.5f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);
        this->setPhysicsBody(physicsBody);
    }
}

Size BulletPlayerGame3::GetSize() {
    return GetContentSizeSprite(modelCharac);
}

void BulletPlayerGame3::setDirection(const Vec2& direction)
{
    _direction = direction;
    _direction.normalize();
    this->createPhysicsBody();

    // Calculate the angle in degrees between the direction vector and the upward vector (0, 1)
    float angle = -CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x)) + 90;

    // Set the rotation of the bullet sprite
    this->setRotation(angle);

    // Move the bullet in the direction using MoveBy
    float distance = 1000.0f; // Distance the bullet will travel
    Vec2 targetPosition = this->getPosition() + _direction * distance;
    float duration = distance / _speed; // Calculate duration based on speed

    auto moveAction = MoveTo::create(duration, targetPosition);
    this->runAction(Sequence::create(moveAction, nullptr));
}

void BulletPlayerGame3::setSpeed(float speed)
{
    _speed = speed;
}

void BulletPlayerGame3::reset()
{
    this->setRotation(0);
    if (modelCharac) {
        modelCharac->setVisible(true);
    }
    if (explosionSprite) {
        explosionSprite->setVisible(false);
    }
    this->stopAllActions();
    this->unscheduleUpdate(); // Stop the update when resetting
    this->setVisible(true);
}

void BulletPlayerGame3::spawn()
{
    this->scheduleUpdate(); // Schedule the update method
}

void BulletPlayerGame3::hideModelCharac() {
    if (modelCharac) {
        modelCharac->setVisible(false);
    }
}

void BulletPlayerGame3::removeWhenOutOfScreen()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    Vec2 position = this->getPosition();

    if (position.x < origin.x || position.x > origin.x + visibleSize.width ||
        position.y < origin.y || position.y > origin.y + visibleSize.height)
    {
        this->returnPool();
    }
}

void BulletPlayerGame3::returnPool() {
    this->removeFromParentAndCleanup(false);
    BulletPoolPlayerGame3::getInstance()->returnObject(this);
    this->unscheduleUpdate(); // Stop the update when returning to the pool
}

void BulletPlayerGame3::explode() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
    SoundController::getInstance()->playSoundEffect(Constants::EnemyCrepExplodeSFX);
    auto explosion = Explosion::create(this->getPosition(), [this]() {
        });
    if (this->getParent() != nullptr) {
        this->getParent()->addChild(explosion);
    }
    this->returnPool();
}

void BulletPlayerGame3::update(float delta)
{
    this->setPosition(this->getPosition() + _direction * _speed * delta);
    this->removeWhenOutOfScreen();
}
