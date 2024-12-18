/*#include "Game2/Enemy/Enemyh/SniperEnemy.h"
#include "Constants/Constants.h"
#include "utils/MathFunction.h"
#include "Game2/Player/PlayerGame2.h"
#include "Bullet/BulletPlayerGame3.h"
#include "Game2/Enemy/EnemyUtils.h"

USING_NS_CC;

SniperEnemy::SniperEnemy()
    : _velocity(Vec2::ZERO),
    _speed(Constants::EnemySpeed),
    _isShooting(false),
    _isDead(false),
    _shootCooldown(5.0f), // 5 seconds cooldown
    _shootRange(500.0f) // Giới hạn tầm bắn
{
}

SniperEnemy::~SniperEnemy()
{
}

bool SniperEnemy::init()
{
    if (!EnemyBase::init())
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

    this->setScale(Constants::EnemyScale);
    this->setAnchorPoint(Vec2(0.5, 0.5));

    this->setTag(Constants::EnemyTag);

    // Create animations
    createIdleAnimation();
    createAttackAnimation();
    createDeathAnimation();

    // Schedule update method
    this->scheduleUpdate();

    // Set up collision detection
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(SniperEnemy::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    //CCLOG("SniperEnemy initialized successfully");
    return true;
}

void SniperEnemy::createIdleAnimation()
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

void SniperEnemy::createAttackAnimation()
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

void SniperEnemy::createDeathAnimation()
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

void SniperEnemy::update(float delta)
{
    //CCLOG("SniperEnemy update called");
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

void SniperEnemy::updateRotationToPlayer()
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

void SniperEnemy::shootBullet()
{
    auto player = dynamic_cast<PlayerGame2*>(this->getParent()->getChildByName("PlayerGame2"));
    if (player && EnemyUtils::isWithinRange(this, player, _shootRange))
    {
        _isShooting = true;
        this->runAction(Sequence::create(_shootAnimation, CallFunc::create([this, player]() {
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
            _isShooting = false;
            }), nullptr));
    }
}

void SniperEnemy::die()
{
    _isDead = true;
    this->runAction(Sequence::create(_deathAnimation, CallFunc::create([this]() {
        this->removeFromParent();
        }), nullptr));
}

bool SniperEnemy::onContactBegin(PhysicsContact& contact)
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
*/