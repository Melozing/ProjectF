#include "UpgradeBulletItemGame3.h"
#include "Controller/SpriteController.h"
#include "utils/PhysicsShapeCache.h"
#include "Game3/Player/PlayerGame3.h"
#include "Constants/Constants.h"

USING_NS_CC;

bool UpgradeBulletItemGame3::init() {
    if (!ItemBaseGame3::init()) {
        return false;
    }
    this->setTexture("assets_game/items/UpgradeBulletItem.png");
    this->setScale(SpriteController::updateSpriteScale(this, 0.07f));

    // Set the physics body info
    this->setPhysicsBodyInfo("physicsBody/UpgradeBulletItem.plist", "UpgradeBulletItem");
    initialScale = this->getScale();

    return true;
}

void UpgradeBulletItemGame3::applyEffect() {
}