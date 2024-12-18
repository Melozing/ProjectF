// PlayerGame1.cpp
#include "Game1/Player/PlayerGame1.h"
#include "Constants/Constants.h"
#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "utils/PhysicsShapeCache.h"

USING_NS_CC;

bool PlayerGame1::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    _health = 3;

    float restrictedWidth = SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDWIDTH);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float restrictedHeight = visibleSize.height - SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDHEIGHT);

    float centerX = visibleSize.width / 2;
    float halfRestrictedWidth = restrictedWidth / 2;
    minX = centerX - halfRestrictedWidth;
    maxX = centerX + halfRestrictedWidth;

    float centerY = visibleSize.height / 2;
    float halfRestrictedHeight = restrictedHeight / 2;
    minY = centerY - halfRestrictedHeight;
    maxY = centerY + halfRestrictedHeight;

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerGame1::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerGame1::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    playerMovement = new PlayerMovement(this, Constants::PLAYER_MOVESPEED, minX, maxX, minY, maxY);
    playerMovement->setEnabled(true);
    this->schedule(CC_SCHEDULE_SELECTOR(PlayerGame1::updateShieldPosition), 0.0001f);

    return true;
}

void PlayerGame1::removePhysicsBody() {
    if (this->getPhysicsBody()) {
        this->removeComponent(this->getPhysicsBody());
    }
}

void PlayerGame1::disableMovement() {
    if (playerMovement) {
        playerMovement->setEnabled(false);
    }
}

void PlayerGame1::takeDamage()
{
    if (canTakeDamage()) {
        _health -= 1;
        _lastDamageTime = Director::getInstance()->getTotalFrames();

        playDamageEffect();

        if (_health <= 0) {
            CCLOG("Game Over");
        }
        else {
            CCLOG("Player health: %d", _health);
        }
    }
}

void PlayerGame1::setShield(ShieldSkill* shield) {
    _shield = shield;
    if (_shield) {
        _shield->setPosition(this->getPosition());
        _shield->activate(Constants::SHIELD_DURATION);
        this->scheduleOnce([this](float) {
            _shield = nullptr;
            }, Constants::SHIELD_TIME_OF_EXISTENCE + Constants::SHIELD_DURATION, "deactivate_shield_key");
    }
}

bool PlayerGame1::canTakeDamage()
{
    float currentTime = Director::getInstance()->getTotalFrames();
    return (currentTime - _lastDamageTime) >= _damageCooldown * 60 && !attributes->IsDead();
}

PlayerGame1* PlayerGame1::createPlayer()
{
    PlayerGame1* player = new (std::nothrow) PlayerGame1();
    if (player && player->init())
    {
        player->autorelease();
        player->initAnimation();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

void PlayerGame1::initAnimation()
{
    auto spriteBatchNode = SpriteBatchNode::create("assets_game/player/Canon.png");
    this->addChild(spriteBatchNode);

    modelCharac = Sprite::createWithSpriteFrameName("Canon1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.13f));

    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(createAnimation("Canon", 33, 0.017f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

Size PlayerGame1::GetSize() {
    return GetContentSizeSprite(modelCharac);
}

void PlayerGame1::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    playerMovement->onKeyPressed(keyCode);
}

void PlayerGame1::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    playerMovement->onKeyReleased(keyCode);
}

void PlayerGame1::update(float delta)
{
    playerMovement->update(delta);
}

void PlayerGame1::updateShieldPosition(float dt) {
    if (_shield && _shield->isActive()) {
        _shield->setPosition(this->getPosition());
    }
}

void PlayerGame1::playDamageEffect() {
    auto blinkAction = Blink::create(1.0f, 3);
    auto tintToRed = TintTo::create(0.1f, 255, 0, 0);
    auto tintToNormal = TintTo::create(0.1f, 255, 255, 255);
    auto sequence = Sequence::create(
        tintToRed, 
        blinkAction, 
        tintToNormal, 
        CallFunc::create([this]() {
            this->modelCharac->setVisible(true); // Ensure the sprite is visible
            }), 
        nullptr);
    modelCharac->runAction(sequence);
}

void PlayerGame1::playHealthIncreaseEffect() {
    auto blinkAction = Blink::create(0.3f, 3);
    auto tintToGreen = TintTo::create(0.1f, 0, 255, 0);
    auto tintToNormal = TintTo::create(0.1f, 255, 255, 255);
    auto sequence = Sequence::create(
        tintToGreen,
        blinkAction,
        tintToNormal,
        CallFunc::create([this]() {
            this->modelCharac->setVisible(true); // Ensure the sprite is visible
            }),
        nullptr
    );
    modelCharac->runAction(sequence);
}

void PlayerGame1::fadeOutAndDisable() {
    modelCharac->runAction(Sequence::create(
        CallFunc::create([this]() {
            this->disableMovement();
            }),
        FadeOut::create(1.0f),
        CallFunc::create([this]() {
            this->setVisible(false);
            }),
        nullptr
    ));
}

void PlayerGame1::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBodyFromPlist("physicsBody/PlayerGame1.plist", "PlayerGame1", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "PlayerGame1", originalSize, 1.35f);

    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setCollisionBitmask(0x01);
        this->setPhysicsBody(physicsBody);
    }
}