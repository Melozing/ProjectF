#ifndef __PLAYERGAME1_H__
#define __PLAYERGAME1_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "PlayerAttributes/PlayerAttributes.h"
#include "Controller/GameController.h"
#include "Manager/PlayerMovementManager.h"
#include "Game1/Skills/ShieldSkill.h"

class PlayerGame1 : public cocos2d::Sprite, public SpriteController
{
public:
    static PlayerGame1* createPlayer();

    virtual bool init() override;

    void initAnimation();
    void takeDamage();
    bool canTakeDamage();
    Size GetSize();
    void removePhysicsBody();

    void playDamageEffect();
    void playHealthIncreaseEffect();

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void update(float delta);

    void setShield(ShieldSkill* shield);
    ShieldSkill* _shield;
    void disableMovement();
    void fadeOutAndDisable();
    void createPhysicsBody();
private:
    int _health = 3;
    float minX, maxX, minY, maxY;
    float _damageCooldown = 1.0f;
    float _lastDamageTime = 0.0f;
    PlayerAttributes* attributes;
    GameController* gameController;
    Sprite* modelCharac;
    PlayerMovement* playerMovement;
    void updateShieldPosition(float dt);
};

#endif // __PLAYERGAME1_H__
