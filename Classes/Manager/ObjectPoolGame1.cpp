#include "Manager/ObjectPoolGame1.h"

template <typename T>
void ObjectPoolGame1<T>::initPool(int size) {
    for (int i = 0; i < size; ++i) {
        T* enemy = T::create();
        if (enemy) {
            enemy->retain();
            pool.push_back(enemy);
        }
    }
}

template <typename T>
T* ObjectPoolGame1<T>::getObject() {
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
void ObjectPoolGame1<T>::returnObject(T* enemy) {
    if (enemy) {
        enemy->reset();
        pool.push_back(enemy);
    }
}

template <typename T>
void ObjectPoolGame1<T>::resetPool() {
    while (!pool.empty()) {
        T* enemy = pool.back();
        pool.pop_back();
        enemy->release();
    }
}

// Explicit template instantiation
template class ObjectPoolGame1<FallingRock>;
template class ObjectPoolGame1<FallingTree>;
template class ObjectPoolGame1<FanBullet>;
template class ObjectPoolGame1<RandomBoom>;
template class ObjectPoolGame1<HealthItem>;
template class ObjectPoolGame1<AmmoItem>;
template class ObjectPoolGame1<ShieldSkill>;
template class ObjectPoolGame1<Landmine>;
