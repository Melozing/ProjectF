#include "EnemyPlaneBase.h"
#include "EnemyPlaneBoom.h"
#include "EnemyPlaneBullet.h"
#include "Manager/ObjectPoolGame3.h"
#include "Game3/Items/ItemBaseGame3.h"
#include "Game3/Items/ItemPoolGane3.h"
#include "Controller/SoundController.h"
#include "Constants/Constants.h"
#include "FX/Explodable.h"

USING_NS_CC;

EnemyPlaneBase* EnemyPlaneBase::create() {
    EnemyPlaneBase* ret = new (std::nothrow) EnemyPlaneBase();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool EnemyPlaneBase::init() {
    if (!Node::init()) {
        return false;
    }
    this->scheduleUpdate(); // Schedule the update function
    return true;
}

void EnemyPlaneBase::moveFromLeftToRight(const Size& visibleSize, float speed) {
    float startX = -this->getContentSize().width / 2;
    float endX = visibleSize.width + SpriteController::calculateScreenRatio(0.15f);

    float distance = endX - startX;
    float moveDuration = distance / speed;

    auto moveRight = MoveTo::create(moveDuration, Vec2(endX, this->getPositionY()));
    auto sequence = Sequence::create(moveRight, nullptr);
    this->runAction(sequence);
}

void EnemyPlaneBase::moveFromRightToLeft(const Size& visibleSize, float speed) {
    float startX = visibleSize.width + this->getContentSize().width / 2;
    float endX = -this->getContentSize().width / 2 - SpriteController::calculateScreenRatio(0.2f);

    float distance = startX - endX;
    float moveDuration = distance / speed;

    if (this->modelCharac) {
        this->modelCharac->setFlippedX(true);
    }

    auto moveLeft = MoveTo::create(moveDuration, Vec2(endX, this->getPositionY()));
    auto sequence = Sequence::create(moveLeft, nullptr);
    this->runAction(sequence);
}

void EnemyPlaneBase::update(float delta) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto position = this->getPosition();

    if (position.x < -this->getContentSize().width / 2 || position.x > visibleSize.width + this->getContentSize().width / 2) {
        this->returnToPool();
    }
}

void EnemyPlaneBase::returnToPool() {
    this->stopAllActions();
    this->setVisible(false);
    this->removeFromParentAndCleanup(false);
    this->reset();

    if (dynamic_cast<EnemyPlaneBullet*>(this)) {
        EnemyPlaneBulletPool::getInstance()->returnObject(static_cast<EnemyPlaneBullet*>(this));
    }
    else if (dynamic_cast<EnemyPlaneBoom*>(this)) {
        EnemyPlaneBoomPool::getInstance()->returnObject(static_cast<EnemyPlaneBoom*>(this));
    }
}

void EnemyPlaneBase::resetSprite() {
    if (this->modelCharac) {
        this->modelCharac->setFlippedX(false);
    }
    this->stopAllActions();
    this->setVisible(true);
}

void EnemyPlaneBase::explode() {
    // Drop a random item
    this->stopAllActions();

    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
    this->setVisible(false);
    SoundController::getInstance()->playSoundEffect(Constants::EnemyCrepExplodeSFX);
    auto explosion = Explosion::create(this->getPosition(), [this]() {
        this->returnToPool();
        });
    this->getParent()->addChild(explosion);
}

void EnemyPlaneBase::dropRandomItem() {
    // Define the drop chance (e.g., 50% chance to drop an item)
    float dropChance = 0.2f; // 50% chance

    // Generate a random number between 0 and 1
    float randomValue = CCRANDOM_0_1();

    // Check if the random value is less than the drop chance
    if (randomValue < dropChance) {
        // Proceed to drop an item
        int randomItem = random(0, 1); // Assuming 3 types of items
        ItemBaseGame3* item = nullptr;

        switch (randomItem) {
        case 0:
            item = HealthRecoveryItemPool::getInstance()->getItem();
            break;
        case 1:
            item = IncreaseBulletCountItemPool::getInstance()->getItem();
            break;
        }

        if (item) {
            this->getParent()->addChild(item);
            item->setStartPosition(this->getPosition());
            item->moveDown();
        }
    }
}

void EnemyPlaneBase::reset() {
    this->setRotation(0);
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
    this->setVisible(false);
    this->setPosition(Vec2::ZERO);
    this->stopAllActions();
}