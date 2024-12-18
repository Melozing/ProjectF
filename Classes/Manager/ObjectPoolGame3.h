#pragma once
#include <vector>
#include "Bullet/BulletPlayerGame3.h"
#include "Game3/enemy/BulletForEnemyPlane.h"
#include "Game3/enemy/BoomForEnemyPlane.h"
#include "Game3/enemy/EnemyPlaneBoom.h"
#include "Game3/enemy/EnemyPlaneBoss.h"
#include "Game3/enemy/EnemyPlaneBullet.h"

template <typename T>
class ObjectPoolGame3 {
public:
    static ObjectPoolGame3* getInstance() {
        static ObjectPoolGame3 instance;
        return &instance;
    }

    void initPool(int size);
    T* getObject();
    void returnObject(T* enemy);
    void resetPool();

private:
    std::vector<T*> pool;
};

// Usage
using EnemyPlaneBulletPool = ObjectPoolGame3<EnemyPlaneBullet>;
using EnemyPlaneBoomPool = ObjectPoolGame3<EnemyPlaneBoom>;
using EnemyPlaneBossPool = ObjectPoolGame3<EnemyPlaneBoss>;

using BulletPoolPlayerGame3 = ObjectPoolGame3<BulletPlayerGame3>;
using BoomForEnemyPlanePool = ObjectPoolGame3<BoomForEnemyPlane>;
using BulletForEnemyPlanePool = ObjectPoolGame3<BulletForEnemyPlane>;
