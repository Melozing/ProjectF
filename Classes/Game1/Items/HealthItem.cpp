#include "HealthItem.h"
#include "PlayerAttributes/PlayerAttributes.h"
#include "Controller/SpriteController.h"
#include "Manager/ObjectPoolGame1.h"
#include "utils/PhysicsShapeCache.h"

USING_NS_CC;

HealthItem* HealthItem::create() {
    HealthItem* ret = new (std::nothrow) HealthItem();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool HealthItem::init() {
    if (!Node::init()) return false;
    //this->initAnimation(); 
    //this->initPhysicsBody();
    return true;
}

void HealthItem::initAnimation() {
    if (spriteBatchNode == nullptr) {
        spriteBatchNode = SpriteBatchNode::create("assets_game/items/Health.png");
        this->addChild(spriteBatchNode);
    }

    _currentSprite = Sprite::createWithSpriteFrameName("Health1.png");
    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, Constants::ITEM_SIZE_RATIO);
    _currentSprite->setScale(_spriteScale);

    spriteBatchNode->addChild(_currentSprite);

    auto animateCharac = Animate::create(SpriteController::createForwardReverseAnimation("Health", 30, 0.03f));
    _currentSprite->runAction(RepeatForever::create(animateCharac));
}


void HealthItem::initPhysicsBody() {
    this->RemovePhysicBody();

    // Create and attach a physics body
    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/ItemHealth.plist");

    auto originalSize = _currentSprite->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBody("ItemHealth", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "ItemHealth", originalSize, 0.13f);

    if (physicsBody) {
        physicsBody->setCollisionBitmask(0x03);
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        this->setPhysicsBody(physicsBody);
    }
}

void HealthItem::applyEffect() {
    this->stopAllActions();
    this->RemovePhysicBody();
    PlayerAttributes::getInstance().SetHealth(PlayerAttributes::getInstance().GetHealth() + 1);

    // Scale up to _scaleFactor times over 0.5 seconds
    auto scaleUp = ScaleTo::create(Constants::ITEM_EFFECT_DURATION, _spriteScale + Constants::ITEM_SCALE_FACTOR);

    // Fade out over 0.5 seconds
    auto fadeOut = FadeOut::create(Constants::ITEM_EFFECT_DURATION);

    // Run the scale and fade actions simultaneously
    auto scaleAndFade = Spawn::create(fadeOut, scaleUp, nullptr);

    // Call playEffectAndRemove after scale and fade actions are complete
    auto callPlayEffectAndRemove = CallFunc::create([this]() {
        this->playEffectAndRemove();
        });
    auto callRemovePhysicBody = CallFunc::create([this]() {
        this->RemovePhysicBody();
        });

    // Create a sequence to run scaleAndFade, then callPlayEffectAndRemove
    auto sequence = Sequence::create(callRemovePhysicBody, scaleAndFade, callPlayEffectAndRemove, nullptr);

    // Run the sequence on the sprite
    _currentSprite->runAction(sequence);
}

Size HealthItem::getScaledSize() const {
    return SpriteController::GetContentSizeSprite(_currentSprite);;
}

void HealthItem::RemovePhysicBody() {
    if (this->getPhysicsBody()) {
        this->removeComponent(this->getPhysicsBody()); // Remove PhysicsBody
    }
}

void HealthItem::playEffectAndRemove() {
    this->returnToPool();
}

void HealthItem::returnToPool() {
    this->stopAllActions();
    this->removeFromParentAndCleanup(false);
    HealthItemPool::getInstance()->returnObject(this);
}

void HealthItem::reset() {
    if (_currentSprite) {
        _currentSprite->setScale(_spriteScale);
    }
    this->setVisible(false);
}
