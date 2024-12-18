#pragma once
#include "cocos2d.h"
#include "Controller/SpriteController.h"

class ItemBaseGame3 : public cocos2d::Sprite, public SpriteController {
public:
    virtual bool init() override;
    virtual void applyEffect() = 0;
    virtual void createPhysicsBodyFromPlist(const std::string& plistFile, const std::string& shapeName);
    virtual void moveDown();
    void setStartPosition(const cocos2d::Vec2& startPosition);
    void setPhysicsBodyInfo(const std::string& plistFile, const std::string& shapeName);
    void stopMovement();
    void returnItemToPoolAfterDelay(float delay);
    void applyPickupEffect();
    void resetScale();

protected:
    float initialScale;

private:
    std::string _plistFile;
    std::string _shapeName;
};