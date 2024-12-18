#ifndef __ENEMY_PLANE_BULLET_H__
#define __ENEMY_PLANE_BULLET_H__

#include "EnemyPlaneBase.h"
#include "Controller/SpriteController.h"

class EnemyPlaneBullet : public EnemyPlaneBase {
public:
    static EnemyPlaneBullet* create();
    virtual bool init() override;
    Size GetSize();
    static void spawnEnemy(cocos2d::Node* parent, float skillTime, bool spawnWithSkill);
    void initAnimation();
    void createPhysicsBody();
    void spawnBullets();
    virtual void returnToPool() override;
private:
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::SpriteBatchNode* explosionBatchNode;
    cocos2d::Sprite* warningSign;
    cocos2d::Sprite* explosionSprite;
    void showWarningSign();
    void hideWarningSign();
};

#endif
