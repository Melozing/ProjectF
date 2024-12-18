#ifndef __HEALTHPLAYERGAME1_H__
#define __HEALTHPLAYERGAME1_H__

#include "cocos2d.h"

class HealthPlayerGame1 : public cocos2d::Sprite {
public:
    static HealthPlayerGame1* createHealth();
    void initHealthSprites(int health); // Method to initialize health sprites
    void updateHealthSprites(int health);
private:
    std::vector<cocos2d::Sprite*> _healthSprites;
    std::vector<cocos2d::Sprite*> _backgroundSprites;
    cocos2d::Sprite* modelHealth;
    cocos2d::Sprite* modelBackground;
    void positionHealthSprites();
};

#endif // __HEALTHPLAYERGAME1_H__
