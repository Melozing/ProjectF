#include "Game3/Items/ItemBaseGame3.h"
#include "utils/PhysicsShapeCache.h"
#include "ItemPoolGane3.h"

USING_NS_CC;

bool ItemBaseGame3::init() {
    if (!Sprite::init()) {
        return false;
    }

    initialScale = this->getScale(); // Store the initial scale

    return true;
}

void ItemBaseGame3::createPhysicsBodyFromPlist(const std::string& plistFile, const std::string& shapeName) {
    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = this->getTexture()->getContentSize();
    auto scaledSize = this->getContentSize();

    auto physicsBody = physicsCache->createBodyFromPlist(plistFile, shapeName, originalSize, scaledSize);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

void ItemBaseGame3::setStartPosition(const Vec2& startPosition) {
    this->setPosition(startPosition);
}

void ItemBaseGame3::setPhysicsBodyInfo(const std::string& plistFile, const std::string& shapeName) {
    _plistFile = plistFile;
    _shapeName = shapeName;
}

void ItemBaseGame3::moveDown() {
    this->resetScale(); // Reset scale before returning to pool
    this->setOpacity(255);
    this->setVisible(true);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto moveAction = MoveBy::create(4.0f, Vec2(0, -visibleSize.height));
    auto delayAction = DelayTime::create(0.2f);
    auto createPhysicsBodyAction = CallFunc::create([this]() {
        this->createPhysicsBodyFromPlist(_plistFile, _shapeName);
        });
    auto sequence = Sequence::create(delayAction, createPhysicsBodyAction, nullptr);
    this->runAction(moveAction);
    this->runAction(sequence);
}

void ItemBaseGame3::stopMovement() {
    this->stopAllActions();
}

void ItemBaseGame3::returnItemToPoolAfterDelay(float delay) {
    // Create a blink action
    auto blinkAction = Blink::create(delay, static_cast<int>(delay * 2)); // Blink twice per second

    // Create a delay action
    auto delayAction = DelayTime::create(delay);

    // Create a return action
    auto returnAction = CallFunc::create([this]() {
        this->resetScale(); // Reset scale before returning to pool
        this->setOpacity(255);
        if (auto upgradeItem = dynamic_cast<UpgradeBulletItemGame3*>(this)) {
            UpgradeBulletItemPool::getInstance()->returnItem(upgradeItem);
        }
        else if (auto increaseItem = dynamic_cast<IncreaseBulletCountItemGame3*>(this)) {
            IncreaseBulletCountItemPool::getInstance()->returnItem(increaseItem);
        }
        else if (auto healthItem = dynamic_cast<HealthRecoveryItemGame3*>(this)) {
            HealthRecoveryItemPool::getInstance()->returnItem(healthItem);
        }
        });

    auto sequence = Sequence::create(blinkAction, delayAction, returnAction, nullptr);
    this->runAction(sequence);
}

void ItemBaseGame3::applyPickupEffect() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
    this->stopAllActions();
    this->setOpacity(255);

    // Create scale and fade actions
    auto scaleAction = ScaleTo::create(0.5f, initialScale * 1.5f); // Scale up by 1.5 times the initial scale
    auto fadeAction = FadeOut::create(0.5f);

    // Create a return action
    auto returnAction = CallFunc::create([this]() {
        this->returnItemToPoolAfterDelay(0.0f);
        });

    // Run the actions in sequence
    this->runAction(Sequence::create(Spawn::create(scaleAction, fadeAction, nullptr), returnAction, nullptr));
}

void ItemBaseGame3::resetScale() {
    this->setScale(initialScale); // Reset to the initial scale
}