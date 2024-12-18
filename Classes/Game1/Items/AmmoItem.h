#ifndef __AMMO_ITEM_H__
#define __AMMO_ITEM_H__

#include "CollectibleItem.h"
#include "Constants/Constants.h"

class AmmoItem : public CollectibleItem {
public:
    static AmmoItem* create();
    virtual bool init() override;
    virtual void applyEffect() override;
    virtual cocos2d::Size getScaledSize() const override;
    virtual void returnToPool() override;
    virtual void playEffectAndRemove() override;
    void reset();
    virtual void initAnimation() override;
    virtual void initPhysicsBody() override;

private:
	void RemovePhysicBody();
};

#endif // __AMMO_ITEM_H__
