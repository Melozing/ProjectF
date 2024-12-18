#pragma once
#include "Game3/Items/UpgradeBulletItemGame3.h"
#include "Game3/Items/IncreaseBulletCountItemGame3.h"
#include "Game3/Items/HealthRecoveryItemGame3.h"
#include <vector>

template <typename T>
class ItemPool {
public:
    static ItemPool* getInstance() {
        static ItemPool instance;
        return &instance;
    }

    void initPool(int size);
    T* getItem();
    void returnItem(T* item);

private:
    std::vector<T*> pool;
};

// Usage
using UpgradeBulletItemPool = ItemPool<UpgradeBulletItemGame3>;
using IncreaseBulletCountItemPool = ItemPool<IncreaseBulletCountItemGame3>;
using HealthRecoveryItemPool = ItemPool<HealthRecoveryItemGame3>;

