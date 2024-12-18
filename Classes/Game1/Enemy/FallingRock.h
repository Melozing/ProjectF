#ifndef __FALLING_ROCK_H__
#define __FALLING_ROCK_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "Constants/Constants.h"
#include "FallingObject.h"

class FallingRock : public FallingObject {
public:
    static FallingRock* create();

    virtual bool init() override;
    void initAnimation() override;
    Size GetSize();
    void spawn(const cocos2d::Vec2& startPosition);
    void reset();
    void initSprite();
    void createPhysicsBody();
private:
    void returnToPool();
    cocos2d::Sprite* _currentSprite;
};

#endif // __FALLING_ROCK_H__
