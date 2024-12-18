#include "HealthRecoveryItemGame3.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

bool HealthRecoveryItemGame3::init() {
    if (!ItemBaseGame3::init()) {
        return false;
    }
    this->setTexture("assets_game/items/HealthRecoveryItem.png");
    this->setScale(SpriteController::updateSpriteScale(this, 0.07f));

    // Set the physics body info
    this->setPhysicsBodyInfo("physicsBody/HealthRecoveryItem.plist", "HealthRecoveryItem");
    initialScale = this->getScale();
    return true;
}

void HealthRecoveryItemGame3::applyEffect() {
}
