#include "FlyingBullet.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

FlyingBullet* FlyingBullet::create() {
    FlyingBullet* ret = new (std::nothrow) FlyingBullet();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void FlyingBullet::reset() {
    spriteBatchNodeLeft = SpriteBatchNode::create("assets_game/enemies/flying_bullet_left.png");
    spriteBatchNodeRight = SpriteBatchNode::create("assets_game/enemies/flying_bullet.png");
}

bool FlyingBullet::init() {
    if (!Sprite::init()) {
        return false;
    }

    spriteBatchNodeLeft = SpriteBatchNode::create("assets_game/enemies/flying_bullet_left.png");
    spriteBatchNodeRight = SpriteBatchNode::create("assets_game/enemies/flying_bullet.png");
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/flying_bullet.png");

    this->scheduleUpdate();
    return true;
}

Size FlyingBullet::GetSize() {
    return GetContentSizeSprite(modelCharac);
}

void FlyingBullet::initAnimation(bool fromLeft) {
    std::string spriteFrameName = fromLeft ? "flying_bullet1.png" : "flying_bullet_left1.png";
    spriteBatchNode = fromLeft ? spriteBatchNodeRight : spriteBatchNodeLeft;

    if (spriteBatchNode->getParent() == nullptr) {
       this->addChild(spriteBatchNode);
    }

    modelCharac = Sprite::createWithSpriteFrameName(spriteFrameName);
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.05f));
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(createAnimation(fromLeft ? "flying_bullet" : "flying_bullet_left", 4, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void FlyingBullet::spawn(const cocos2d::Vec2& startPosition) {
    this->setPosition(startPosition);
    _fromLeft = startPosition.x < Director::getInstance()->getVisibleSize().width / 2;
    initAnimation(_fromLeft);

    _direction = _fromLeft ? Vec2(1, 0) : Vec2(-1, 0);
}

void FlyingBullet::update(float delta) {
    //Vec2 currentPosition = this->getPosition();
    //Vec2 movement = _direction * _speed * delta;

    //this->setPosition(currentPosition + movement);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (this->getPosition().x < -this->getContentSize().width - 50.0f || this->getPosition().x > visibleSize.width + this->getContentSize().width + 50.0f) {
        this->removeFromParentAndCleanup(true);
    }
}

FlyingBullet::~FlyingBullet() {
    
}
