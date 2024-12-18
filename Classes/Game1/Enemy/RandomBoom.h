#ifndef __RANDOM_BOOM_H__
#define __RANDOM_BOOM_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class RandomBoom : public cocos2d::Sprite, public SpriteController {
public:
    virtual bool init() override;
    void spawn(const cocos2d::Vec2& startPosition);
    void showWarning(const cocos2d::Vec2& position);
    void launchMissile(const cocos2d::Vec2& targetPosition);
    void onMissileHitTarget();
    Size GetSize();
    CREATE_FUNC(RandomBoom);
    virtual ~RandomBoom();
    void reset();

private:
    static cocos2d::Vector<RandomBoom*> _pool;

    cocos2d::SpriteBatchNode* _spriteBatchNodeWarning;
    cocos2d::SpriteBatchNode* _spriteBatchNodeMissile;
    cocos2d::SpriteBatchNode* _spriteBatchNodeExplosion;

    cocos2d::Sprite* _warningSprite;
    cocos2d::Sprite* _missileSprite;
    cocos2d::Sprite* explosionSprite;
    cocos2d::Sprite* explosionSpriteDump;
    PhysicsBody* explosionBody;
};
#endif // __RANDOM_BOOM_H__