// PlayerGame2.cpp
#include "PlayerGame2.h"
#include "Constants/Constants.h"
#include "utils/MathFunction.h"
#include "cocos2d.h"
#include "Grenade/Grenade.h"
#include "Grenade/BulletGame2.h"
#include "Grenade/PoolBulletGame2.h"


#include "Controller/SoundController.h"

USING_NS_CC;

PlayerGame2::PlayerGame2()
    : _mousePos(Vec2::ZERO),
    _isMouseDown(false),
    _mousePressDuration(0.0f),
    _isThrowingGrenade(false),
    playerMovement(nullptr),
    bulletPool(30),
    attributes(new PlayerAttributes(100, 130)),
    isReloading(false),
    reloadTime(2.0f),
    currentFireMode(FireMode::SINGLE),
    burstCooldown(0.0f),
    isAutoFiring(false)
{
    totalAmmo = attributes->GetAmmo();
    currentMagazine = maxMagazineSize;
    currentGrenades = maxGrenades;

    SoundController::getInstance()->preloadMusic("assets_game/sounds/Game2/reload.mp3");
    SoundController::getInstance()->preloadMusic("assets_game/sounds/Game2/shot.mp3");
    SoundController::getInstance()->preloadMusic("assets_game/sounds/Game2/nembom.mp3");
}


PlayerGame2::~PlayerGame2()
{
    delete playerMovement;
	delete attributes;
}

PlayerGame2* PlayerGame2::createPlayerGame2()
{
    PlayerGame2* player = new (std::nothrow) PlayerGame2();
    player->setName("PlayerGame2");
    if (player && player->init())
    {
        player->autorelease();
        player->initAnimation();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool PlayerGame2::init() {
    if (!Sprite::init()) {
        return false;
    }

    this->setPosition(Vec2(Constants::InitialPosX, Constants::InitialPosY));
    this->setScale(Constants::PlayerScale);
    this->setAnchorPoint(Vec2(0.5, 0.5));

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(PlayerGame2::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(PlayerGame2::onMouseDown, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(PlayerGame2::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerGame2::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerGame2::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    this->scheduleUpdate();

    playerMovement = new PlayerMovement(this, Constants::PlayerSpeed);
    
    _ammoLabel = Label::createWithTTF("0/0", Constants::FONT_GAME, 24);
    _ammoLabel->setPosition(Vec2(this->getContentSize().width / 2, -100)); 
    this->addChild(_ammoLabel, 1);


    _reloadSprite = Sprite::create("assets_game/effects/Reload.png");
    _reloadSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 20));
    _reloadSprite->setVisible(false);
	_reloadSprite->setScale(0.5f);
    this->addChild(_reloadSprite, 1);
    updateAmmoDisplay();
    createPhysicsBody();
    return true;
}

void PlayerGame2::initAnimation()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/walkriffle.plist");

    auto spriteBatchNode = SpriteBatchNode::create("assets_game/player/walkriffle.png");
    this->addChild(spriteBatchNode);

    modelCharac = Sprite::createWithSpriteFrameName("walkriffle0.png");
    SpriteController::updateSpriteScale(modelCharac, 0.05f);
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.25f));

    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(createAnimation("walkriffle", 5, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void PlayerGame2::startMovementAnimation()
{
    if (!this->getActionByTag(1))
    {
        auto animateCharac = Animate::create(createAnimation("walkriffle", 5, 0.07f));
        auto repeatAnimate = RepeatForever::create(animateCharac);
        repeatAnimate->setTag(1);
        modelCharac->runAction(repeatAnimate);
    }
}

void PlayerGame2::onMouseMove(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    _mousePos = Vec2(e->getCursorX(), e->getCursorY());

    auto winSize = Director::getInstance()->getWinSize();
    _mousePos.y = winSize.height - _mousePos.y;
}

void PlayerGame2::onMouseDown(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        _isMouseDown = true;
        _mousePressDuration = 0.0f;
        _isThrowingGrenade = false;

        if (currentFireMode == FireMode::AUTO) {
            isAutoFiring = true;
            this->schedule([this](float) {
                auto mousePos = Director::getInstance()->convertToGL(_mousePos);
                Vec2 pos = this->getPosition();
                Vec2 dirToShoot = mousePos - pos;
                shootBullet(dirToShoot);
                }, 0.1f, "auto_fire_key");
        }
    }
    else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
    {
        _isMouseDown = true;
        _mousePressDuration = 0.0f;
        _isThrowingGrenade = true;
    }
}

void PlayerGame2::onMouseUp(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    if (_isMouseDown)
    {
        auto mousePos = Director::getInstance()->convertToGL(_mousePos);
        Vec2 pos = this->getPosition();
        Vec2 dirToShoot = mousePos - pos;

        if (_isThrowingGrenade && e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
        {
            if (currentGrenades > 0) {
                throwGrenade(dirToShoot, _mousePressDuration);
                currentGrenades--;
            }
        }
        else if (!_isThrowingGrenade && e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
        {
            switch (currentFireMode) {
            case FireMode::SINGLE:
                if (currentMagazine > 0) {
                    shootBullet(dirToShoot);
                }
                else {
                    reload();
                }
                break;
            case FireMode::AUTO:
                isAutoFiring = false;
                this->unschedule("auto_fire_key");
                break;
            case FireMode::BURST:
                if (currentMagazine >= 3) {
                    fireBurst();
                }
                else {
                    reload();
                }
                break;
            }
        }

        _isMouseDown = false;
    }
}

void PlayerGame2::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_A ||
        keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_D ||
        keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
        keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
    {
        playerMovement->onKeyPressed(keyCode);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_R)
    {
        reload();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_B)
    {
        switchFireMode();
    }
}

void PlayerGame2::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_A ||
        keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_D ||
        keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
        keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
    {
        playerMovement->onKeyReleased(keyCode);
    }
}

void PlayerGame2::update(float delta)
{
    playerMovement->update(delta);
    RotateToMouse();
    if (playerMovement->getSpeed() > 0)
    {
        startMovementAnimation();
    }
    else
    {
        modelCharac->stopActionByTag(1);
    }

    if (_isMouseDown)
    {
        _mousePressDuration += delta;
    }
    if (isReloading)
    {
        reloadTime -= delta;
        if (reloadTime <= 0)
        {
            int bulletsToReload = maxMagazineSize - currentMagazine;
            if (totalAmmo >= bulletsToReload)
            {
                currentMagazine += bulletsToReload;
                totalAmmo -= bulletsToReload;
            }
            else
            {
                currentMagazine += totalAmmo;
                totalAmmo = 0;
            }
            isReloading = false;
            reloadTime = 2.0f;
            _reloadSprite->stopAllActions();
            _reloadSprite->setVisible(false);
            updateAmmoDisplay();
        }
    }

    if (currentFireMode == FireMode::BURST && burstCooldown > 0.0f) {
        burstCooldown -= delta;
    }
}

void PlayerGame2::switchFireMode()
{
    switch (currentFireMode) {
    case FireMode::SINGLE:
        currentFireMode = FireMode::AUTO;
        break;
    case FireMode::AUTO:
        currentFireMode = FireMode::BURST;
        break;
    case FireMode::BURST:
        currentFireMode = FireMode::SINGLE;
        break;
    }
}

void PlayerGame2::fireBurst()
{
    if (burstCooldown > 0.0f) {
        return;
    }

    auto mousePos = Director::getInstance()->convertToGL(_mousePos);
    Vec2 pos = this->getPosition();
    Vec2 dirToShoot = mousePos - pos;

    for (int i = 0; i < 3; ++i) {
        this->scheduleOnce([this, dirToShoot](float) {
            shootBullet(dirToShoot);
            }, i * 0.1f, "burst_fire_key_" + std::to_string(i));
    }

    burstCooldown = 1.5f;
}

void PlayerGame2::RotateToMouse()
{
    auto mousePos = Director::getInstance()->convertToGL(_mousePos);
    Vec2 pos = this->getPosition();
    Vec2 dirToFace = mousePos - pos;
    dirToFace.normalize();
    float angle = MathFunction::GetDirInDegreesPositive(dirToFace);
    this->setRotation(-angle + 90);
}

void PlayerGame2::shootBullet(const Vec2& direction)
{
    if (isReloading || currentMagazine <= 0)
    {
        reload();
        return;
    }

    Vec2 normalizedDirection = direction.getNormalized();
    BulletGame2* bullet = BulletGame2::createBullet(this->getPosition(), normalizedDirection, Constants::BulletSpeed, Constants::BulletDamage2);
    if (bullet)
    {
        this->getParent()->addChild(bullet);
        playShootSound();
    }
    currentMagazine--;
    updateAmmoDisplay();
}

void PlayerGame2::throwGrenade(const Vec2& direction, float duration)
{
    auto grenade = Grenade::createGrenade(this->getPosition(), direction, duration);
    this->getParent()->addChild(grenade);
	playGrenadeSound();
}

void PlayerGame2::reload()
{
    if (isReloading || currentMagazine == maxMagazineSize || totalAmmo == 0)
    {
        return;
    }

    isReloading = true;
    playReloadSound();
    reloadTime = 2.0f; // Start reload time
    _reloadSprite->setVisible(true);

    auto rotateAction = RotateBy::create(1.0f, 360.0f);
    _reloadSprite->runAction(RepeatForever::create(rotateAction));
    playReloadSound();
}

void PlayerGame2::takeDamage(int damage)
{
    attributes->TakeDamage(damage);
    if (attributes->IsDead())
    {
        die();
    }
}
void PlayerGame2::die()
{
    //animation cho nay
}

void PlayerGame2::pickUpHealth(int healthAmount)
{
    attributes->IncreaseHealth(healthAmount);
}
void PlayerGame2::pickUpAmmo(int ammoAmount)
{
    attributes->SetAmmo(attributes->GetAmmo() + ammoAmount);
    updateAmmoDisplay();
}
void PlayerGame2::pickUpGrenade(int grenadeAmount)
{
    currentGrenades = std::min(currentGrenades + grenadeAmount, maxGrenades);
}

void PlayerGame2::updateAmmoDisplay()
{
    _ammoLabel->setString(StringUtils::format("%d/%d", currentMagazine, totalAmmo));
}

void PlayerGame2::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
    auto physicsBody = PhysicsBody::createBox(this->getContentSize());
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(0x01); // Category bitmask for player
    physicsBody->setCollisionBitmask(0x04); // Collides with items
    physicsBody->setContactTestBitmask(0x04); // Test contact with items
    this->addComponent(physicsBody);
}
void PlayerGame2::playReloadSound() {
    SoundController::getInstance()->playSoundEffect("assets_game/sounds/Game2/reload.mp3");
}

void PlayerGame2::playShootSound() {
    SoundController::getInstance()->playSoundEffect("assets_game/sounds/Game2/shot.mp3");
}
void PlayerGame2::playGrenadeSound() {
    SoundController::getInstance()->playSoundEffect("assets_game/sounds/Game2/nembom.mp3");
}