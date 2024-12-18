#pragma once

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class BulletPlayerGame3 :public cocos2d::Sprite, public SpriteController
{
public:
    static BulletPlayerGame3* create();
    bool init() override;

    void setDirection(const cocos2d::Vec2& direction);
    void setSpeed(float speed);
    Size GetSize();
    void reset();
    void spawn();
    void removeWhenOutOfScreen();
    void returnPool();
    void explode();
    void hideModelCharac();
    void update(float delta) override;

private:
    cocos2d::Vec2 _direction;
    float _speed;
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::Sprite* modelCharac;
    cocos2d::SpriteBatchNode* explosionBatchNode;
    cocos2d::Sprite* explosionSprite;

    void initAnimation();
    void createPhysicsBody();
};
