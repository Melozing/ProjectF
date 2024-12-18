// BulletGame2.cpp
#include "Grenade/BulletGame2.h"
#include "Constants/Constants.h"
USING_NS_CC;

BulletGame2::BulletGame2(): _direction(Vec2::ZERO), _speed(0.0f), _damage(0){}

BulletGame2::~BulletGame2(){}

BulletGame2* BulletGame2::createBullet(const Vec2& position, const Vec2& direction, float speed,int damage)
{
    BulletGame2* bullet = new (std::nothrow) BulletGame2();
	bullet->setName("Bullet");
    if (bullet && bullet->init())
    {
        bullet->autorelease();
        bullet->setPosition(position);
        bullet->setDirection(direction);
        bullet->setSpeed(speed);
        bullet->_damage = damage;
        bullet->setScale(Constants::BulletScale);

        auto physicsBody = PhysicsBody::createCircle(bullet->getContentSize().width / 2);
        physicsBody->setContactTestBitmask(true);
        physicsBody->setGravityEnable(false);
        physicsBody->setDynamic(false);
        //physicsBody->setVelocity(direction * speed);
		physicsBody->setCategoryBitmask(0x01);
		physicsBody->setCollisionBitmask(0x02);
		physicsBody->setContactTestBitmask(0x02);
        bullet->setPhysicsBody(physicsBody);

        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

bool BulletGame2::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    this->initWithFile("assets_game/player/ball.png");
    this->scheduleUpdate();
    return true;
}

void BulletGame2::update(float delta)
{
    this->setPosition(this->getPosition() + _direction * _speed * delta);
}

void BulletGame2::setDirection(const Vec2& direction)
{
    _direction = direction;
}

void BulletGame2::setSpeed(float speed)
{
    _speed = speed;
}
void BulletGame2::onExit()
{
    // Perform any cleanup specific to BulletGame2 here

    // Call the base class onExit() method
    Sprite::onExit();
}
int BulletGame2::getDamage() const {
    return _damage;
}