#include "FX/Explodable.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

Explosion* Explosion::create(const Vec2& position, std::function<void()> callback, float scale) {
    Explosion* ret = new (std::nothrow) Explosion();
    if (ret && ret->init(position, callback, scale)) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

bool Explosion::init(const Vec2& position, std::function<void()> callback, float scale) {
    if (!Sprite::init()) {
        return false;
    }

    this->callback = callback;

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");

    auto explosionBatchNode = SpriteBatchNode::create("assets_game/fx/explosions.png");
    this->addChild(explosionBatchNode);

    auto explosionSprite = Sprite::createWithSpriteFrameName("explosions7.png");
    explosionSprite->setScale(SpriteController::updateSpriteScale(explosionSprite, scale));
    explosionBatchNode->addChild(explosionSprite);
    explosionSprite->setPosition(position);
    explosionSprite->setVisible(true);

    auto explosionAnimation = SpriteController::createAnimation("explosions", 10, 0.041f);
    auto animate = Animate::create(explosionAnimation);

    explosionSprite->runAction(Sequence::create(
        animate,
        CallFunc::create([this]() {
            if (this->callback) {
                this->callback();
            }
            this->removeFromParentAndCleanup(true);
            }),
        nullptr
    ));

    return true;
}

void Explosion::reset() {
    this->stopAllActions();
    this->setVisible(false);
    this->removeAllChildrenWithCleanup(true);
}
