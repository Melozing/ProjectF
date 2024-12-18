#pragma once

#include "cocos2d.h"

class Bullet_Petard : public cocos2d::Sprite
{
public:
    static Bullet_Petard* createBullet();
    virtual bool init();
    void initOptions();
    void addEvents();
    void addAnimation();
    void setupInitialPosition();
    void moveToRandomPosition();

   
};
