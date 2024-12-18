#ifndef __LANDMINE_H__
#define __LANDMINE_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "Constants/Constants.h"
#include "FallingObject.h"

class Landmine : public FallingObject {
public:
    static Landmine* create();

    virtual bool init() override;
    Size GetSize();
    void spawn(const cocos2d::Vec2& startPosition);
    void reset();
    void initAnimation() override;
    void createPhysicsBody();
    void triggerPreExplosion();
    void triggerExplosion();
private:
    void returnToPool();
    cocos2d::SpriteBatchNode* _spriteBatchNodeLandmine;
    cocos2d::SpriteBatchNode* _spriteBatchNodePreExplosion;
    cocos2d::SpriteBatchNode* _spriteBatchNodeExplosion;
    cocos2d::Sprite* _currentSprite;
    cocos2d::Sprite* _preExplosionSprite;
    cocos2d::Sprite* _explosionSprite;
    cocos2d::Sprite* explosionSpriteDump;
};

#endif // __LANDMINE_H__
