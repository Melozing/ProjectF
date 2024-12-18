#ifndef __FAN_BULLET_H__
#define __FAN_BULLET_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class FanBullet : public cocos2d::Sprite, public SpriteController {
public:
    static FanBullet* create();
    void reset();
    void spawn(const cocos2d::Vec2& startPosition, float angle);
    void update(float delta) override;
    void removeWhenOutOfScreen();
    cocos2d::Size GetSize();

private:
    bool init() override;
    void initAnimation();
    void createPhysicsBody();
    cocos2d::Vec2 _direction;
    float _speed = 500.0f;
    cocos2d::SpriteBatchNode* spriteBatchNode = nullptr;
    cocos2d::Sprite* modelCharac = nullptr;
};


#endif // __FAN_BULLET_H__
