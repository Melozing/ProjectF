#ifndef ENEMYPLANE_BOSS_H
#define ENEMYPLANE_BOSS_H

#include "cocos2d.h"
#include "EnemyPlaneBase.h"

class EnemyPlaneBoss : public EnemyPlaneBase {
public:
    static EnemyPlaneBoss* create();
    void takeDamage(float damage);

    virtual bool init() override;
    void initAnimation();
    void spawnEnemy();
    void reset();
    Size GetSize();
    void moveLeftRight();
    float getHealth();
    void updatePhase();

private:
    void createPhysicsBody();
    void moveUpAndReturnToPool();
    void startExplosions();
    void executePhaseSkills();
    void dropBooms();
    void graduallyIncreaseHealth();
    void dropUpgradeItem();

    enum class Phase {
        StartPHASE,
        PHASE_1,
        PHASE_2,
        PHASE_3
    };

    Phase currentPhase;
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    bool isExploding;
};

#endif // ENEMYPLANE_BOSS_H
