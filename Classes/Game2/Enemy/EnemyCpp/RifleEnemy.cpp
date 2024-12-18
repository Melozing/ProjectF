#include "Game2/Enemy/Enemyh/RifleEnemy.h"
#include "Constants/Constants.h"
#include "Game2/Player/PlayerGame2.h"
#include "Game2/Enemy/EnemyUtils.h"
#include "utils/MathFunction.h"

USING_NS_CC;

RifleEnemy::RifleEnemy()
    : _bulletPool(new PoolBulletGame2(20)),
    _isShooting(false),
    _isReloading(false),
    _currentMagazine(10),
    _maxMagazineSize(10),
    _shootCooldown(0.0f),
    _reloadTime(2.0f),
    _bulletSpeed(300.0f), // Tốc độ đạn chậm để người chơi có thể né
    _velocity(Vec2::ZERO),
    _isMoving(false)
{
    _health = 100;
    _damage = 10;
    _speed = Constants::EnemySpeed;
    _shootRange = 120.0f;
}

RifleEnemy::~RifleEnemy()
{
    delete _bulletPool;
    CC_SAFE_RELEASE(_idleAnimation);
    CC_SAFE_RELEASE(_attackAnimation);
    CC_SAFE_RELEASE(_deathAnimation);
}

bool RifleEnemy::init()
{
    if (!EnemyBase::init())
    {
        return false;
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/sniper-enemy.plist");
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("idlegun0.png");
    if (!spriteFrame)
    {
        CCLOG("Sprite frame 'idlegun0.png' not found in the cache");
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

    _idleAnimation = RepeatForever::create(Animate::create(createIdleAnimation()));
    _idleAnimation->retain();
    _attackAnimation = Animate::create(createAttackAnimation());
    _attackAnimation->retain();
    _deathAnimation = Animate::create(createDeathAnimation());
    _deathAnimation->retain();

    _reloadSprite = Sprite::create("assets_game/effects/Reload.png");
    _reloadSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 20));
    _reloadSprite->setVisible(false);
    _reloadSprite->setScale(0.5f);
    this->addChild(_reloadSprite, 1);

    createPhysicsBody();
    this->scheduleUpdate();
    return true;
}

cocos2d::Animation* RifleEnemy::createIdleAnimation()
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

    return Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
}

cocos2d::Animation* RifleEnemy::createAttackAnimation()
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

    return Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
}

cocos2d::Animation* RifleEnemy::createDeathAnimation()
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

    return Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
}

void RifleEnemy::update(float delta)
{
    if (_isDead)
    {
        return;
    }

    _shootCooldown -= delta;
    if (_shootCooldown <= 0 && !_isReloading)
    {
        shootBullet();
        _shootCooldown = 0.3f; // Bắn mỗi 0.3 giây
    }
    updateRotationToPlayer();
    if (!_isShooting)
    {
        moveToPlayer();
    }
}

void RifleEnemy::updateRotationToPlayer()
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

void RifleEnemy::shootBullet()
{
    if (_currentMagazine == 0)
    {
        reload();
        return;
    }

    auto player = dynamic_cast<PlayerGame2*>(this->getParent()->getChildByName("PlayerGame2"));
    if (player && EnemyUtils::isWithinRange(this, player, _shootRange))
    {
        _isShooting = true;
        this->runAction(Sequence::create(_attackAnimation, CallFunc::create([this, player]() {
            updateRotationToPlayer(); // Cập nhật hướng trước khi bắn
            Vec2 playerPos = player->getPosition();
            Vec2 pos = this->getPosition();
            Vec2 dirToPlayer = playerPos - pos;
            dirToPlayer.normalize();

            BulletGame2* bullet = BulletGame2::createBullet(pos, dirToPlayer, _bulletSpeed, _damage);
            if (bullet)
            {
                this->getParent()->addChild(bullet);
            }

            _currentMagazine--;
            _isShooting = false;
            }), nullptr));
    }
}

void RifleEnemy::reload()
{
    if (_isReloading || _currentMagazine == _maxMagazineSize)
    {
        return;
    }

    _isReloading = true;
    _reloadSprite->setVisible(true);
    auto rotateAction = RotateBy::create(1.0f, 360.0f);
    _reloadSprite->runAction(RepeatForever::create(rotateAction));

    this->runAction(Sequence::create(DelayTime::create(_reloadTime), CallFunc::create([this]() {
        _currentMagazine = _maxMagazineSize;
        _isReloading = false;
        _reloadSprite->setVisible(false);
        _reloadSprite->stopAllActions();
        }), nullptr));
}

void RifleEnemy::createPhysicsBody()
{
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
    auto physicsBody = PhysicsBody::createCircle(this->getContentSize().width / 2);
    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    this->addComponent(physicsBody);
}
