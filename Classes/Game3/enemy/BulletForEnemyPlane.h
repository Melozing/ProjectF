#ifndef __BULLET_FOR_ENEMY_PLANE_H__
#define __BULLET_FOR_ENEMY_PLANE_H__

#include "cocos2d.h"

class BulletForEnemyPlane : public cocos2d::Sprite {
public:
    static BulletForEnemyPlane* create();
    void reset();
    virtual bool init() override;
    void moveDown(float angle);
    void returnToPool();
    void explode();
    void createPhysicsBody();
private:
    cocos2d::Size GetSize();
    bool isOutOfScreen;
    cocos2d::Sprite* modelCharac;
    cocos2d::SpriteBatchNode* explosionBatchNode;
    cocos2d::Sprite* explosionSprite;
};

#endif // __BULLET_FOR_ENEMY_PLANE_H__