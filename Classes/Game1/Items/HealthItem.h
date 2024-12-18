#ifndef __HEALTH_ITEM_H__
#define __HEALTH_ITEM_H__

#include "CollectibleItem.h"
#include "Constants/Constants.h"

class HealthItem : public CollectibleItem {
public:
    static HealthItem* create();
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

#endif // __HEALTH_ITEM_H__
