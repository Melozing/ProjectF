#include "RandomBoom.h"
#include "Manager/ObjectPoolGame1.h"
#include "utils/PhysicsShapeCache.h"
#include "Constants/Constants.h"
#include "cocos2d.h"

USING_NS_CC;

bool RandomBoom::init() {
    if (!Node::init()) {
        return false;
    }

    // Create sprite batch nodes
    _spriteBatchNodeWarning = SpriteBatchNode::create("assets_game/enemies/warning_rocket.png");
    _spriteBatchNodeMissile = SpriteBatchNode::create("assets_game/enemies/rocket.png");
    _spriteBatchNodeExplosion = SpriteBatchNode::create("assets_game/fx/explosions.png");

    // Add batch nodes to the current node
    this->addChild(_spriteBatchNodeWarning);
    this->addChild(_spriteBatchNodeMissile);
    this->addChild(_spriteBatchNodeExplosion);

    return true;
}

void RandomBoom::reset() {
    if (_warningSprite) {
        _warningSprite->setVisible(false);
        _warningSprite->stopAllActions();
    }
    if (_missileSprite) {
        _missileSprite->setVisible(false);
        _missileSprite->stopAllActions();
    }
    if (explosionSprite) {
        explosionSprite->setVisible(false);
        explosionSprite->stopAllActions();
    }
    this->unscheduleAllCallbacks(); // Unschedule all callbacks
}

void RandomBoom::spawn(const Vec2& startPosition) {
    reset();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float restrictedWidth = SpriteController::calculateScreenRatio(Constants::RANDOMBOOM_RESTRICTEDWIDTH);
    float restrictedHeight = visibleSize.height - SpriteController::calculateScreenRatio(Constants::RANDOMBOOM_RESTRICTEDHEIGHT);
    float centerX = visibleSize.width / 2;
    float halfRestrictedWidth = restrictedWidth / 2;

    float minX = centerX - halfRestrictedWidth / 2;
    float maxX = centerX + halfRestrictedWidth;

    float spawnX = minX + (rand() % int(maxX - minX));
    float spawnY = SpriteController::calculateScreenRatio(Constants::RANDOMBOOM_RESTRICTEDHEIGHT) + (rand() % int(restrictedHeight));

    Vec2 warningPosition = Vec2(spawnX, spawnY);

    showWarning(warningPosition);

    this->scheduleOnce([this, warningPosition](float) {
        launchMissile(warningPosition);
        }, 0.2f, "missile_launch_key");
}

void RandomBoom::showWarning(const Vec2& position) {
    if (!_warningSprite) {
        _warningSprite = Sprite::createWithSpriteFrameName("warning_rocket1.png");
        _warningSprite->setScale(SpriteController::updateSpriteScale(_warningSprite, 0.1f));
        _spriteBatchNodeWarning->addChild(_warningSprite);
    }
    _warningSprite->setPosition(position);
    _warningSprite->setVisible(true);

    auto warningAnimation = createAnimation("warning_rocket", 50, 0.01f);
    if (warningAnimation) {
        auto animateWarning = Animate::create(warningAnimation);
        _warningSprite->runAction(RepeatForever::create(animateWarning));
    }
}

void RandomBoom::launchMissile(const Vec2& targetPosition) {
    if (!_missileSprite) {
        _missileSprite = Sprite::createWithSpriteFrameName("rocket1.png");
        _missileSprite->setScale(SpriteController::updateSpriteScale(_missileSprite, 0.25f));
        _spriteBatchNodeMissile->addChild(_missileSprite);
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 startPosition = (rand() % 2 == 0) ? Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.9f) : Vec2(visibleSize.width * 0.9f, visibleSize.height * 0.9f);
    _missileSprite->setPosition(startPosition);
    _missileSprite->setVisible(true);

    Vec2 direction = targetPosition - startPosition;
    float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x));
    _missileSprite->setRotation(-angle - 90);

    // Create rocket animation
    auto rocketAnimation = createAnimation("rocket", 11, 0.017f);
    if (rocketAnimation) {
        auto animateRocket = Animate::create(rocketAnimation);
        _missileSprite->runAction(RepeatForever::create(animateRocket));
    }

    float missileSpeed = 0.6f;
    auto moveToTarget = MoveTo::create(missileSpeed, targetPosition);
    auto hitTargetCallback = CallFunc::create([this]() {
        this->onMissileHitTarget();
        });

    _missileSprite->runAction(Sequence::create(moveToTarget, hitTargetCallback, nullptr));
}


Size RandomBoom::GetSize() {
    return GetContentSizeSprite(_warningSprite);
}


void RandomBoom::onMissileHitTarget() {
    if (!_warningSprite || !_missileSprite) {
        CCLOG("Warning sprite or missile sprite is null. Cannot create explosion.");
        return;
    }
    Vec2 position = _missileSprite->getPosition();

    // Cleanup missile and warning sprites
    if (_missileSprite) {
        _missileSprite->setVisible(false);
    }

    if (_warningSprite) {
        _warningSprite->setVisible(false);
    }

    if (!explosionSprite) {
        explosionSprite = Sprite::createWithSpriteFrameName("explosions7.png");
        explosionSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        explosionSprite->setScale(SpriteController::updateSpriteScale(explosionSprite, 0.09f));
        explosionSprite->setPosition(position);
        _spriteBatchNodeExplosion->addChild(explosionSprite);
    }
    else {
        explosionSprite->setPosition(position);
        explosionSprite->setVisible(true);
    }

    auto explosionAnimation = createAnimation("explosions", 10, 0.041f);
    auto animate = Animate::create(explosionAnimation);

    //Size reducedSize = Size(GetContentSizeSprite(explosionSprite).width, GetContentSizeSprite(explosionSprite).height);
    explosionSpriteDump = Sprite::createWithSpriteFrameName("explosions7.png");
    explosionSpriteDump->setScale(SpriteController::updateSpriteScale(explosionSpriteDump, 0.15f));
    
    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/RandomBoomExplosionGame1.plist");

    auto originalSize = explosionSpriteDump->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    explosionBody = physicsCache->createBody("RandomBoomExplosionGame1", originalSize, scaledSize);
    physicsCache->resizeBody(explosionBody, "RandomBoomExplosionGame1", originalSize, 1.0f);
    if (explosionBody) {
        explosionBody->setCollisionBitmask(0x02); // Unique bitmask for missiles
        explosionBody->setContactTestBitmask(true);
        explosionBody->setDynamic(false);
        explosionBody->setGravityEnable(false);

        explosionSprite->setPhysicsBody(explosionBody);
    }

    explosionSprite->runAction(Sequence::create(
        animate,
        CallFunc::create([this]() {
            explosionSprite->setVisible(false); // Hide instead of removing
            if (explosionSprite->getPhysicsBody()) {
                explosionSprite->removeComponent(explosionSprite->getPhysicsBody()); // Remove PhysicsBody
            }
            this->reset();
            }),
        CallFunc::create([this]() {
            this->stopAllActions();
            this->removeFromParentAndCleanup(false);
            RandomBoomPool::getInstance()->returnObject(this);
            }),
        nullptr
    ));
}

RandomBoom::~RandomBoom() {

}
