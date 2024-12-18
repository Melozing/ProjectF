#ifndef __ENEMY_PLANE_BASE_H__
#define __ENEMY_PLANE_BASE_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class EnemyPlaneBase : public cocos2d::Sprite, public SpriteController {
public:
    virtual bool init() override;
    static EnemyPlaneBase* create();

    void moveFromLeftToRight(const cocos2d::Size& visibleSize, float speed);
    void moveFromRightToLeft(const cocos2d::Size& visibleSize, float speed);
    virtual void returnToPool();
    void resetSprite();
    void explode();
    void reset();
    void dropRandomItem();
    void update(float delta);
protected:
    cocos2d::Sprite* modelCharac;
};

#endif
