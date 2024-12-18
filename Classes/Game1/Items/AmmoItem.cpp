#include "AmmoItem.h"
#include "PlayerAttributes/PlayerAttributes.h"
#include "Controller/SpriteController.h"
#include "Manager/ObjectPoolGame1.h"
#include "utils/PhysicsShapeCache.h"

USING_NS_CC;

AmmoItem* AmmoItem::create() {
    AmmoItem* ret = new (std::nothrow) AmmoItem();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool AmmoItem::init() {
    if (!Node::init()) return false;
    //this->initAnimation();
    //this->initPhysicsBody();
    return true;
}

void AmmoItem::initPhysicsBody() {
    this->RemovePhysicBody();

    // Create and attach a physics body
    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/ItemAmmo.plist");

    auto originalSize = _currentSprite->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBody("ItemAmmo", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "ItemAmmo", originalSize, 0.09f);

    if (physicsBody) {
        physicsBody->setCollisionBitmask(0x03);
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        this->setPhysicsBody(physicsBody);
    }
}

void AmmoItem::initAnimation() {
    if (spriteBatchNode == nullptr) {
        spriteBatchNode = SpriteBatchNode::create("assets_game/items/Ammo.png");
        this->addChild(spriteBatchNode);
    }

    _currentSprite = Sprite::createWithSpriteFrameName("Ammo1.png");
    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, Constants::ITEM_SIZE_RATIO);
    _currentSprite->setScale(_spriteScale);

    spriteBatchNode->addChild(_currentSprite);

    auto animateCharac = Animate::create(SpriteController::createForwardReverseAnimation("Ammo", 30, 0.03f));
    _currentSprite->runAction(RepeatForever::create(animateCharac));
}


void AmmoItem::applyEffect() {
    this->stopAllActions();
    this->RemovePhysicBody();
    PlayerAttributes::getInstance().SetAmmo(PlayerAttributes::getInstance().GetAmmo() + 1);

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
    auto sequence = Sequence::create(callRemovePhysicBody, scaleAndFade, callPlayEffectAndRemove,nullptr);

    // Run the sequence on the sprite
    _currentSprite->runAction(sequence);
}

void AmmoItem::RemovePhysicBody() {
    if (this->getPhysicsBody()) {
        this->removeComponent(this->getPhysicsBody()); // Remove PhysicsBody
    }
}

void AmmoItem::playEffectAndRemove() {
	this->returnToPool();
}

Size AmmoItem::getScaledSize() const {
    return SpriteController::GetContentSizeSprite(_currentSprite);;
}

void AmmoItem::returnToPool() {
    this->stopAllActions();
    this->removeFromParentAndCleanup(false);
    AmmoItemPool::getInstance()->returnObject(this);
}

void AmmoItem::reset() {
    if (_currentSprite) {
        _currentSprite->setScale(_spriteScale);
    }
    this->setVisible(false);
}
