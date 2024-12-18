#pragma once

#include "Game2/Enemy/EnemyBase.h"
#include "Grenade/PoolBulletGame2.h"
#include "Grenade/BulletGame2.h"
class RifleEnemy : public EnemyBase
{
public:
    RifleEnemy();
    virtual ~RifleEnemy();
    virtual bool init() override;
    CREATE_FUNC(RifleEnemy);

protected:
    virtual cocos2d::Animation* createIdleAnimation() override;
    virtual cocos2d::Animation* createAttackAnimation() override;
    virtual cocos2d::Animation* createDeathAnimation() override;
    virtual void update(float delta) override;
    virtual void createPhysicsBody() override;

private:
    void shootBullet();
    void reload();
    void updateRotationToPlayer();

    PoolBulletGame2* _bulletPool;
    bool _isShooting;
    bool _isReloading;
    int _currentMagazine;
    int _maxMagazineSize;
    float _shootCooldown;
    float _reloadTime;
    float _shootRange;
    float _bulletSpeed;
    cocos2d::Vec2 _velocity;
    bool _isMoving;

    cocos2d::Sprite* _reloadSprite;
    cocos2d::RepeatForever* _idleAnimation;
    cocos2d::Animate* _attackAnimation;
    cocos2d::Animate* _deathAnimation;
};
