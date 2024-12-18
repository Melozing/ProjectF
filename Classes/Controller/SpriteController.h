// SpriteController.h
#ifndef __SPRITE_CONTROLLER_H__
#define __SPRITE_CONTROLLER_H__

#include "cocos2d.h"

USING_NS_CC;

class SpriteController {
public:
    // Animation handling
    static Animation* createAnimation(const std::string& prefixName, int frameCount, float delay);
    static Animation* createForwardReverseAnimation(const std::string& prefixName, int frameCount, float delay);
    void changeAnimation(const std::string& namePrefix, int numOrder);
    static float updateSpriteScale(Sprite* sprite, float size);
    static float updateSpriteScale(cocos2d::Node* node, float size);
    static float calculateScreenRatio(float ratio);
    static Size GetContentSizeSprite(Sprite* sprite);
private:
    // Sprite and animation
    Sprite* modelCharac;
    Animation* animation;
};

#endif // __SPRITE_CONTROLLER_H__