#ifndef __BADGE_H__
#define __BADGE_H__

#include "cocos2d.h"

class Badge : public cocos2d::Node {
public:
    static Badge* createBadge(const std::string& badgeImage, const std::string& font, float fontSize);
    void setBadgePosition(const cocos2d::Vec2& position);
    void updateLabel(std::string& text);
    void blinkRed();

private:
    bool init(const std::string& badgeImage, const std::string& font, float fontSize);
    cocos2d::Sprite* badgeSprite;
    cocos2d::Label* badgeLabel;
    std::string fontLabel;
    float fontSizeLabel;
    bool isBlinking = false;
};

#endif // __BADGE_H__
