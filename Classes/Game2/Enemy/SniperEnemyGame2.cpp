// SniperEnemyGame2.cpp
#include "Game2/Enemy/SniperEnemyGame2.h"
#include "Constants/Constants.h"
#include "utils/MathFunction.h"
#include "Game2/Player/PlayerGame2.h"
#include "Bullet/BulletPlayerGame3.h"

USING_NS_CC;

SniperEnemyGame2::SniperEnemyGame2()
    : _velocity(Vec2::ZERO),
    _speed(Constants::EnemySpeed),
    _isShooting(false),
    _isDead(false),
    _shootCooldown(5.0f) // 5 seconds cooldown
{
}

SniperEnemyGame2::~SniperEnemyGame2()
{
}

SniperEnemyGame2* SniperEnemyGame2::createSniperEnemyGame2()
{
    SniperEnemyGame2* enemy = new (std::nothrow) SniperEnemyGame2();
    if (enemy && enemy->init())
    {
        enemy->autorelease();
        return enemy;
    }
    else
    {
        delete enemy;
        return nullptr;
    }
}

bool SniperEnemyGame2::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // Load the sprite frames
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/sniper-enemy.plist");

    // Initialize the sprite with the idle frame
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("idlegun0.png");
    if (!spriteFrame)
    {
        CCLOG("Sprite frame 'IdleSniper0.png' not found in the cache");
        return false;
    }

    if (!Sprite::initWithSpriteFrame(spriteFrame))
    {
        CCLOG("Failed to initialize sprite with sprite frame");
        return false;
    }

    this->setPosition(Vec2(Constants::InitialEnemyPosX, Constants::InitialEnemyPosY));
    this->setScale(Constants::EnemyScale);
    this->setAnchorPoint(Vec2(0.5, 0.5));

    // Add PhysicsBody to the enemy
    auto physicsBody = PhysicsBody::createBox(this->getContentSize());
    physicsBody->setContactTestBitmask(true);
    physicsBody->setGravityEnable(false); // Disable gravity
    physicsBody->setDynamic(false);
    this->setPhysicsBody(physicsBody);

    this->setTag(Constants::EnemyTag); // Set the tag for the enemy

    // Create animations
    createIdleAnimation();
    createShootAnimation();
    createDeathAnimation();

    // Schedule update method
    this->scheduleUpdate();

    // Set up collision detection
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(SniperEnemyGame2::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    //CCLOG("SniperEnemyGame2 initialized successfully");
    return true;
}

void SniperEnemyGame2::createIdleAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < 8; i++)
    {
        sprintf(str, "idlegun%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
    _idleAnimation = RepeatForever::create(Animate::create(animation));
    _idleAnimation->retain();
}

void SniperEnemyGame2::createShootAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < 8; i++)
    {
        sprintf(str, "Gunshot%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
    _shootAnimation = Animate::create(animation);
    _shootAnimation->retain();
}

void SniperEnemyGame2::createDeathAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < 6; i++)
    {
        sprintf(str, "death%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
    _deathAnimation = Animate::create(animation);
    _deathAnimation->retain();
}

void SniperEnemyGame2::update(float delta)
{
    //CCLOG("SniperEnemyGame2 update called");
    if (_isDead)
    {
        return;
    }

    _shootCooldown -= delta;
    if (_shootCooldown <= 0)
    {
        shootBullet();
        _shootCooldown = 5.0f; // Reset cooldown
    }

    updateRotationToPlayer();
}

void SniperEnemyGame2::updateRotationToPlayer()
{
    auto player = dynamic_cast<PlayerGame2*>(this->getParent()->getChildByName("PlayerGame2"));
    if (player)
    {
        Vec2 playerPos = player->getPosition();
        Vec2 pos = this->getPosition();
        Vec2 dirToPlayer = playerPos - pos;
        float angle = CC_RADIANS_TO_DEGREES(-dirToPlayer.getAngle());
        this->setRotation(angle + 90);
    }
    else
    {
        CCLOG("Player not found");
    }
}

void SniperEnemyGame2::shootBullet()
{
    _isShooting = true;
    this->runAction(Sequence::create(_shootAnimation, CallFunc::create([this]() {
        auto player = dynamic_cast<PlayerGame2*>(this->getParent()->getChildByName("PlayerGame2"));
        if (player)
        {
            Vec2 playerPos = player->getPosition();
            Vec2 pos = this->getPosition();
            Vec2 dirToPlayer = playerPos - pos;
            dirToPlayer.normalize();

            auto bullet = Sprite::create("assets_game/player/shot.png");
            bullet->setPosition(pos);
            bullet->setRotation(this->getRotation()); // Set bullet rotation to match enemy's rotation
            this->getParent()->addChild(bullet);

            auto bulletBody = PhysicsBody::createBox(bullet->getContentSize());
            bulletBody->setContactTestBitmask(true);
            bulletBody->setGravityEnable(false);
            bulletBody->setDynamic(true);
            bullet->setPhysicsBody(bulletBody);

            bullet->runAction(Sequence::create(MoveBy::create(2.0f, dirToPlayer * 1000), RemoveSelf::create(), nullptr));
        }
        _isShooting = false;
        }), nullptr));
}


void SniperEnemyGame2::die()
{
    _isDead = true;
    this->runAction(Sequence::create(_deathAnimation, CallFunc::create([this]() {
        this->removeFromParent();
        }), nullptr));
}

bool SniperEnemyGame2::onContactBegin(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if ((nodeA && nodeA->getTag() == Constants::BulletTag) || (nodeB && nodeB->getTag() == Constants::BulletTag))
    {
        this->die();
        return true;
    }

    return false;
}
