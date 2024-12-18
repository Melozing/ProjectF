#ifndef ITEMSSPAWN_H
#define ITEMSSPAWN_H

#include "cocos2d.h"

class ItemsSpawn : public cocos2d::Sprite {
public:
    enum class ItemType {
        AMMO,
        HEALTH,
        GRENADE
    };

    static ItemsSpawn* create(ItemType type, const cocos2d::Vec2& position);
    virtual bool init(ItemType type, const cocos2d::Vec2& position);
    void update(float delta) override;
    ItemType getType() const;

private:
    ItemType _type;
    cocos2d::Vec2 _initialPosition;
    float _time;
    void createPhysicsBody();
    void startBlinking();
};

#endif // ITEMSSPAWN_H
