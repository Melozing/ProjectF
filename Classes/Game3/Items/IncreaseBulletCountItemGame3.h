#pragma once
#include "Game3/Items/ItemBaseGame3.h"

class IncreaseBulletCountItemGame3 : public ItemBaseGame3 {
public:
    virtual bool init() override;
    virtual void applyEffect() override;
    CREATE_FUNC(IncreaseBulletCountItemGame3);
};