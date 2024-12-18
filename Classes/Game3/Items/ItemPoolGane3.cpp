#include "ItemPoolGane3.h"

template <typename T>
inline void ItemPool<T>::initPool(int size) {
    for (int i = 0; i < size; ++i) {
        auto item = T::create();
        item->retain();
        pool.push_back(item);
    }
}

template <typename T>
inline T* ItemPool<T>::getItem() {
    if (pool.empty()) {
        return T::create();
    }
    auto item = pool.back();
    pool.pop_back();
    return item;
}

template <typename T>
inline void ItemPool<T>::returnItem(T* item) {
    item->removeFromParent();
    pool.push_back(item);
}

// Explicit template instantiation
template class ItemPool<UpgradeBulletItemGame3>;
template class ItemPool<IncreaseBulletCountItemGame3>;
template class ItemPool<HealthRecoveryItemGame3>;

