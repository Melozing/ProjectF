#include "Landmine.h"
#include "Manager/ObjectPoolGame1.h"
#include "Controller/SpriteController.h"
#include "utils/PhysicsShapeCache.h"
#include "cocos2d.h"

USING_NS_CC;

Landmine* Landmine::create() {
    Landmine* ret = new (std::nothrow) Landmine();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool Landmine::init() {
    if (!Sprite::init()) { // Assuming Sprite is the superclass
        return false;
    }

    // Create sprite batch nodes
    _spriteBatchNodeLandmine = SpriteBatchNode::create("assets_game/enemies/landmine.png");
    _spriteBatchNodePreExplosion = SpriteBatchNode::create("assets_game/enemies/pre_explosion.png");
    _spriteBatchNodeExplosion = SpriteBatchNode::create("assets_game/enemies/ExplosionLandmine.png");

    // Add batch nodes to the current node
    this->addChild(_spriteBatchNodeLandmine);
    this->addChild(_spriteBatchNodePreExplosion);
    this->addChild(_spriteBatchNodeExplosion);

    initAnimation();
    return true;
}

void Landmine::reset() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
    this->setVisible(false);
    if (_currentSprite) {
        _currentSprite->setVisible(false);
        _currentSprite->stopAllActions();
    }
    if (_preExplosionSprite) {
        _preExplosionSprite->setVisible(false);
        _preExplosionSprite->stopAllActions();
    }
    if (_explosionSprite) {
        _explosionSprite->setVisible(false);
        _explosionSprite->stopAllActions();
    }
    this->unscheduleAllCallbacks(); // Unschedule all callbacks
}

Size Landmine::GetSize() {
    return GetContentSizeSprite(_currentSprite);
}

void Landmine::initAnimation() {
    std::string spriteFrameName = "landmine1.png";
    _animationDelay = 0.1f; // Animation delay for landmine

    // Create the sprite and add it to the landmine batch node
    _currentSprite = Sprite::createWithSpriteFrameName(spriteFrameName);
    _spriteBatchNodeLandmine->addChild(_currentSprite);

    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, 0.1f); // Scale for landmine
    _currentSprite->setScale(_spriteScale);

    // Create animation with the customized delay for landmine
    auto animateCharac = Animate::create(createAnimation("landmine", 13, _animationDelay));
    _currentSprite->runAction(RepeatForever::create(animateCharac));

    // Create physics body
    createPhysicsBody();
}

void Landmine::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = _currentSprite->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBodyFromPlist("physicsBody/Landmine.plist", "Landmine", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "Landmine", originalSize, 0.45f);

    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setCollisionBitmask(0x02);
        this->setPhysicsBody(physicsBody);
    }
}

void Landmine::spawn(const Vec2& startPosition) {
    reset(); // Reset the landmine state

    // Set the position of the landmine
    this->setPosition(startPosition);
    this->setVisible(true);
    this->initAnimation();

    // Define target position off-screen at the bottom
    Vec2 endPosition = Vec2(startPosition.x, -SpriteController::calculateScreenRatio(Constants::FALLINGROCK_ITEMS_OFFSET));

    // Calculate distance and duration based on speed
    float distance = startPosition.distance(endPosition);
    float duration = distance / _speed;

    // Move down action
    auto moveDown = MoveTo::create(duration, endPosition);

    // Callback to remove landmine when it moves off-screen
    auto removeLandmine = CallFunc::create([this]() {
        this->returnToPool();
        });

    // Run move action and remove when done
    this->runAction(Sequence::create(moveDown, removeLandmine, nullptr));
}


void Landmine::triggerPreExplosion() {
    // Remove current physics body
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    _animationDelay = 0.017f;

    // Hide current sprite
    if (_currentSprite) {
        _currentSprite->setVisible(false);
    }

    // Change to pre-explosion animation
    if (!_preExplosionSprite) {
        _preExplosionSprite = Sprite::createWithSpriteFrameName("pre_explosion1.png");
        _preExplosionSprite->setScale(SpriteController::updateSpriteScale(_preExplosionSprite, 0.1f));
        _spriteBatchNodePreExplosion->addChild(_preExplosionSprite);
    }
    _preExplosionSprite->setPosition(_currentSprite->getPosition());
    _preExplosionSprite->setVisible(true);

    auto preExplosionAnimation = Animate::create(createAnimation("pre_explosion", 9, _animationDelay));
    auto explosionCallback = CallFunc::create([this]() {
        // Hide pre-explosion sprite
        _currentSprite->setVisible(false);
        _preExplosionSprite->setVisible(false);
        this->triggerExplosion();
        });

    // Run pre-explosion animation and callback
    _preExplosionSprite->runAction(Sequence::create(preExplosionAnimation, explosionCallback, nullptr));
}

void Landmine::triggerExplosion() {
    // Remove current physics body
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    this->stopAllActions();
    Vec2 position = _preExplosionSprite->getPosition();

    _animationDelay = 0.1f;

    // Change to explosion animation
    if (!_explosionSprite) {
        _explosionSprite = Sprite::createWithSpriteFrameName("ExplosionLandmine1.png");
        _explosionSprite->setScale(SpriteController::updateSpriteScale(_explosionSprite, 0.15f));
        _explosionSprite->setPosition(position);
        _spriteBatchNodeExplosion->addChild(_explosionSprite);
    }
    else {
        _explosionSprite->setPosition(position);
        _explosionSprite->setVisible(true);
    }

    auto explosionAnimation = createAnimation("ExplosionLandmine", 5, _animationDelay);
    auto animate = Animate::create(explosionAnimation);

    //Size reducedSize = Size(GetContentSizeSprite(explosionSprite).width, GetContentSizeSprite(explosionSprite).height);
    explosionSpriteDump = Sprite::createWithSpriteFrameName("ExplosionLandmine5.png");
    explosionSpriteDump->setScale(SpriteController::updateSpriteScale(explosionSpriteDump, 0.15f));

    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/ExplosionLandmine.plist");

    auto originalSize = explosionSpriteDump->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto explosionBody = physicsCache->createBody("ExplosionLandmine", originalSize, scaledSize);
    physicsCache->resizeBody(explosionBody, "ExplosionLandmine", originalSize, 7.35f);

    if (explosionBody) {
        explosionBody->setCollisionBitmask(0x02); // Unique bitmask for landmines
        explosionBody->setContactTestBitmask(true);
        explosionBody->setDynamic(false);
        explosionBody->setGravityEnable(false);

        _explosionSprite->setPhysicsBody(explosionBody);
    }

    _explosionSprite->runAction(Sequence::create(
        animate,
        CallFunc::create([this]() {
            _explosionSprite->setVisible(false); // Hide instead of removing
            if (_explosionSprite->getPhysicsBody()) {
                _explosionSprite->removeComponent(_explosionSprite->getPhysicsBody()); // Remove PhysicsBody
            }
            this->reset();
            }),
        CallFunc::create([this]() {
            this->stopAllActions();
            this->removeFromParentAndCleanup(false);
            LandminePool::getInstance()->returnObject(this);
            }),
        nullptr
    ));
}

void Landmine::returnToPool() {
    this->reset();
    this->setVisible(false);
    this->removeFromParentAndCleanup(false);
    LandminePool::getInstance()->returnObject(this);
}
