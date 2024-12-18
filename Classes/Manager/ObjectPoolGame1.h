#pragma once
#include "Game1/Enemy/FallingRock.h"
#include "Game1/Enemy/FallingTree.h"
#include "Game1/Enemy/FanBullet.h"
#include "Game1/Enemy/RandomBoom.h"
#include "Game1/Enemy/Landmine.h"

#include "Game1/Items/AmmoItem.h"
#include "Game1/Items/HealthItem.h"
#include "Game1/Skills/ShieldSkill.h"

#include <vector>

template <typename T>
class ObjectPoolGame1 {
public:
    static ObjectPoolGame1* getInstance() {
        static ObjectPoolGame1 instance;
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
using FallingRockPool = ObjectPoolGame1<FallingRock>;
using FallingTreePool = ObjectPoolGame1<FallingTree>;
using FanBulletPool = ObjectPoolGame1<FanBullet>;
using RandomBoomPool = ObjectPoolGame1<RandomBoom>;
using AmmoItemPool = ObjectPoolGame1<AmmoItem>;
using HealthItemPool = ObjectPoolGame1<HealthItem>;
using ShieldSkillPool = ObjectPoolGame1<ShieldSkill>;
using LandminePool = ObjectPoolGame1<Landmine>;
