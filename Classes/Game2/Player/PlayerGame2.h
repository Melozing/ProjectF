#pragma once

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "Manager/PlayerMovementManager.h"
#include "Grenade/BulletGame2.h"
#include "Grenade/PoolBulletGame2.h"
#include "PlayerAttributes/PlayerAttributes.h"

class PlayerGame2 : public cocos2d::Sprite, public SpriteController {
public:
    PlayerGame2();
    virtual ~PlayerGame2();

    static PlayerGame2* createPlayerGame2();
    virtual bool init();
    void initAnimation();
    void startMovementAnimation();
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void update(float delta);
    void RotateToMouse();
    void shootBullet(const cocos2d::Vec2& direction);
    void throwGrenade(const cocos2d::Vec2& direction, float duration);
    void reload();
    void takeDamage(int damage);
    void die();
    void pickUpHealth(int healthAmount);
    void pickUpAmmo(int ammoAmount);
    void pickUpGrenade(int grenadeAmount);

    const int maxMagazineSize = 30;
    const int initialAmmo = 120;
    const int maxGrenades = 5; // Giới hạn số lượng lựu đạn

    enum class FireMode { SINGLE, AUTO, BURST };
    void switchFireMode();
    void fireBurst();


	void playShootSound();
	void playReloadSound();
	void playGrenadeSound();
private:
    cocos2d::Vec2 _mousePos;
    bool _isMouseDown;
    float _mousePressDuration;
    bool _isThrowingGrenade;
    cocos2d::Sprite* modelCharac;
    cocos2d::Sprite* _reloadSprite;
    PlayerMovement* playerMovement;
    PoolBulletGame2 bulletPool;
    int totalAmmo;
    int currentMagazine;
    bool isReloading;
    float reloadTime;
    int currentGrenades; // Số lượng lựu đạn hiện tại
    cocos2d::Label* _ammoLabel;
    void updateAmmoDisplay();

    PlayerAttributes* attributes;
    void createPhysicsBody();

    FireMode currentFireMode;
    float burstCooldown;
    bool isAutoFiring;
};

