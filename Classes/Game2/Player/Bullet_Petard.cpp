#include "Constants/Constants.h"
#include "Controller/SoundController.h"
#include "Game2/Player/Bullet_Petard.h"

USING_NS_CC;

Bullet_Petard* Bullet_Petard::createBullet()
{
    Bullet_Petard* bullet = new (std::nothrow) Bullet_Petard();
    if (bullet && bullet->initWithFile("assets_game/player/bullet_petard.png"))
    {
        bullet->autorelease();
        bullet->initOptions();
        bullet->addEvents();
        bullet->addAnimation();
        bullet->setupInitialPosition();
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

bool Bullet_Petard::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // Initialize variables
    return true;
}

void Bullet_Petard::initOptions()
{
    // Initialize options for the bullet
}

void Bullet_Petard::addEvents()
{
    // Add event listeners for the bullet
}

void Bullet_Petard::addAnimation()
{
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(1);

    animFrames.pushBack(SpriteFrame::create("assets_game/player/bullet_petard.png", Rect(0, 0, 64, 64)));

    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    auto animate = Animate::create(animation);

    this->runAction(RepeatForever::create(animate));
}

void Bullet_Petard::setupInitialPosition()
{
    // Get the visible size of the screen
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Calculate the initial position to be in the center and slightly to the right
    float initialPosX = visibleSize.width / 2 + 150; // Center and move right by 50 units
    float initialPosY = visibleSize.height / 2 + 150; // Center vertically

    // Set the initial position of the bullet
    this->setPosition(Vec2(initialPosX, initialPosY));
    this->setAnchorPoint(Vec2(0.5, 0.5));
}

void Bullet_Petard::moveToRandomPosition()
{
    // Get the visible size of the screen
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Generate a random end position within the screen bounds
    float randomX = CCRANDOM_0_1() * visibleSize.width;
    float randomY = CCRANDOM_0_1() * visibleSize.height;
    Vec2 endPosition = Vec2(randomX, randomY);

    // Create the move action
    auto moveAction = MoveTo::create(5.0f, endPosition);
    this->runAction(moveAction);
}
