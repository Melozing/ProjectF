#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class Explosion : public cocos2d::Sprite {
public:
    static Explosion* create(const cocos2d::Vec2& position, std::function<void()> callback, float scale = 0.08f + CCRANDOM_0_1() * (0.1f - 0.08f));
    void reset();

protected:
    bool init(const cocos2d::Vec2& position, std::function<void()> callback, float scale);
    std::function<void()> callback;
};

#endif // EXPLOSION_H
