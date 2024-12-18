#include "IncreaseBulletCountItemGame3.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

bool IncreaseBulletCountItemGame3::init() {
    if (!ItemBaseGame3::init()) {
        return false;
    }
    this->setTexture("assets_game/items/IncreaseBulletCountItem.png");
    this->setScale(SpriteController::updateSpriteScale(this, 0.07f));

    // Set the physics body info
    this->setPhysicsBodyInfo("physicsBody/IncreaseBulletCountItem.plist", "IncreaseBulletCountItem");
    initialScale = this->getScale();

    return true;
}

void IncreaseBulletCountItemGame3::applyEffect() {
}