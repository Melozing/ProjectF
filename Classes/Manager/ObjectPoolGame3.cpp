#include "Manager/ObjectPoolGame3.h"

template <typename T>
void ObjectPoolGame3<T>::initPool(int size) {
    for (int i = 0; i < size; ++i) {
        T* enemy = T::create();
        if (enemy) {
            enemy->retain();
            pool.push_back(enemy);
        }
    }
}

template <typename T>
T* ObjectPoolGame3<T>::getObject() {
    if (pool.empty()) {
        T* enemy = T::create();
        if (enemy) {
            enemy->retain();
            return enemy;
        }
        return nullptr;
    }
    T* enemy = pool.back();
    pool.pop_back();
    return enemy;
}

template <typename T>
void ObjectPoolGame3<T>::returnObject(T* enemy) {
    if (enemy) {
        enemy->reset();
        pool.push_back(enemy);
    }
}

template <typename T>
void ObjectPoolGame3<T>::resetPool() {
    while (!pool.empty()) {
        T* enemy = pool.back();
        pool.pop_back();
        enemy->release();
    }
}

// Explicit template instantiation
template class ObjectPoolGame3<EnemyPlaneBullet>;
template class ObjectPoolGame3<EnemyPlaneBoom>;
template class ObjectPoolGame3<EnemyPlaneBoss>;

template class ObjectPoolGame3<BulletPlayerGame3>;
template class ObjectPoolGame3<BulletForEnemyPlane>;
template class ObjectPoolGame3<BoomForEnemyPlane>;


