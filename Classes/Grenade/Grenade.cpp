#include "Grenade/Grenade.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"
#include "Game2/Enemy/EnemyBase.h"
USING_NS_CC;

Grenade* Grenade::createGrenade(const Vec2& startPosition, const Vec2& direction, float throwDuration)
{
    Grenade* grenade = new (std::nothrow) Grenade();
    if (grenade && grenade->init(startPosition, direction, throwDuration))
    {
        grenade->autorelease();
		grenade->setScale(0.5f);
        return grenade;
    }
    CC_SAFE_DELETE(grenade);
    return nullptr;
}

bool Grenade::init(const Vec2& startPosition, const Vec2& direction, float throwDuration)
{
    if (!Sprite::initWithFile("assets_game/player/grenade.png"))
    {
        return false;
    }

    this->setPosition(startPosition);
    _direction = direction.getNormalized();
    _throwDuration = throwDuration;
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/effects/explosion.plist");

    float throwDistance = calculateThrowDistance(_throwDuration);

    auto rotate = RotateBy::create(throwDuration, 360);
    auto moveBy = MoveBy::create(throwDuration, _direction * throwDistance);
    auto spawn = Spawn::create(rotate, moveBy, nullptr);

    this->runAction(Sequence::create(spawn, CallFunc::create([this]() {
        this->createImpactEffect();
        this->scheduleOnce([this](float) {
            if (this->getParent() != nullptr) {
                this->explode();
            }
            }, 2.0f, "explode_key"); 
        }), nullptr));
    auto physicsBody = PhysicsBody::createCircle(10.0f);
    physicsBody->setContactTestBitmask(true);
    physicsBody->setCollisionBitmask(0x0004);
    physicsBody->setGravityEnable(false);
    this->setPhysicsBody(physicsBody);
    return true;
}

void Grenade::createImpactEffect()
{
    auto impactSprite = Sprite::create("assets_game/effects/warning.png");
    impactSprite->setPosition(this->getPosition());
    this->getParent()->addChild(impactSprite);
    auto blink = Blink::create(2.0f, 7);
    impactSprite->runAction(Sequence::create(blink, CallFunc::create([impactSprite]() {
        impactSprite->removeFromParent();
        }), nullptr));
}

void Grenade::explode()
{
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 6; ++i)
    {
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(
            StringUtils::format("explosion%d.png", i)
        );
        if (frame == nullptr) {
            CCLOG("Frame %d not found in explosion.plist", i);
            return;
        }
        frames.pushBack(frame);
    }

    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto animate = Animate::create(animation);
    auto repeatAnimate = Repeat::create(animate, 1);
    auto explosionSprite = Sprite::createWithSpriteFrame(frames.front());
    explosionSprite->setPosition(this->getPosition());
    this->getParent()->addChild(explosionSprite);
    this->setVisible(false);
    explosionSprite->runAction(Sequence::create(repeatAnimate, CallFunc::create([explosionSprite, this]() {
        explosionSprite->removeFromParent();
        this->removeFromParent();
        }), nullptr));
    this->dealDamage();
}

void Grenade::dealDamage()
{
    float damageRadius = 100.0f;
    auto damageCircle = DrawNode::create();
    damageCircle->drawSolidCircle(this->getPosition(), damageRadius, 0, 50, Color4F(1, 0, 0, 0.5f));
    this->getParent()->addChild(damageCircle);
    auto children = this->getParent()->getChildren();
    for (auto child : children)
    {
        if (child == this) continue;
        if (child->getBoundingBox().intersectsCircle(this->getPosition(), damageRadius))
        {
            auto enemy = dynamic_cast<EnemyBase*>(child);
            if (enemy)
            {
                enemy->setHealth(enemy->getHealth() - 50);
                if (enemy->getHealth() <= 0)
                {
                    enemy->die();
                }
            }
            else if (child->getName() == "PlayerGame2")
            {
                child->removeFromParent();
            }
        }
    }
    damageCircle->removeFromParent();
}


float Grenade::calculateThrowDistance(float holdTime)
{
    const float maxHoldTime = 2.0f;
    const float minThrowDistance = 100.0f;
    const float maxThrowDistance = Constants::GrenadeThrowDistance;
    float normalizedHoldTime = fmod(holdTime, maxHoldTime * 2.0f);

    if (normalizedHoldTime > maxHoldTime)
    {
        normalizedHoldTime = maxHoldTime * 2.0f - normalizedHoldTime;
    }

    return minThrowDistance + (maxThrowDistance - minThrowDistance) * (normalizedHoldTime / maxHoldTime);
}
