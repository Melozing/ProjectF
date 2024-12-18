#pragma once

#include "cocos2d.h"

class Petard : public cocos2d::Sprite
{
public:
    static Petard* create();
    virtual bool init() override;
    void initOptions();
    void addEvents();
    void initAnimation();
    void setupInitialPosition();
    void shootBullet(float dt);
    void createPhysicsBody();
    void addHealthBar();
private:
	float health;
    cocos2d::Size GetSize();
    cocos2d::Sprite* modelCharac;
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::SpriteBatchNode* explosionBatchNode;
};