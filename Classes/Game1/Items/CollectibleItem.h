#ifndef __COLLECTIBLE_ITEM_H__
#define __COLLECTIBLE_ITEM_H__

#include "Constants/Constants.h"
#include "cocos2d.h"

class CollectibleItem : public cocos2d::Node {
public:
    virtual bool init() override;
    virtual void applyEffect() = 0; 
    void spawn(const cocos2d::Vec2& position);
    cocos2d::Size GetSize() const;
    void update(float delta) override;
    void removeWhenOutOfScreen();
    virtual cocos2d::Size getScaledSize() const;
    virtual void playEffectAndRemove(); 
    virtual void returnToPool(); 
    virtual void initAnimation();
    virtual void initPhysicsBody();
protected:
    cocos2d::Sprite* _currentSprite;
    cocos2d::SpriteBatchNode* spriteBatchNode = nullptr;
    float _speed = Constants::FALLINGROCK_ITEMS_SPEED; 
    float _spriteScale;
};

#endif // __COLLECTIBLE_ITEM_H__
