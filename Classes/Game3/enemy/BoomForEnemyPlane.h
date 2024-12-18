#ifndef __BOOMFORENEMYPLANE_H__
#define __BOOMFORENEMYPLANE_H__

#include "cocos2d.h"

class BoomForEnemyPlane : public cocos2d::Sprite {
public:
    static BoomForEnemyPlane* create();
    void reset();
    virtual bool init() override;
    void moveDown(bool spawnFromLeft);
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

#endif // __BOOMFORENEMYPLANE_H__
