#include "PlayerGame3.h"
#include "Constants/Constants.h"
#include "Controller/SoundController.h"
#include "utils/MathFunction.h"
#include "Manager/PlayerMovementManager.h"
#include "Manager/ObjectPoolGame3.h"
#include "Controller/GameController.h"
#include "Game3/Enemy/EnemyPlaneBoss.h"
#include "utils/PhysicsShapeCache.h"
#include "cocos2d.h"

USING_NS_CC;

PlayerGame3* PlayerGame3::createPlayerGame3()
{
    PlayerGame3* player = new (std::nothrow) PlayerGame3();
    if (player && player->init())
    {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool PlayerGame3::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    setupInitialPosition();
    setupTurret();
    setupEventListeners();
    setupManagers();
    initAnimation();
    // Schedule update method
    this->scheduleUpdate();

    // Initialize shooting variables
    isMouseDown = false;
    shootDelay = 0.15f;
    timeSinceLastShot = 0.0f;
    bulletCount = 1;

    // Preload shoot sound effect
    Constants::QuantityBulletPlayerGame3 = 100;

    return true;
}

void PlayerGame3::setupInitialPosition()
{
    // Get the visible size of the screen
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Calculate the initial position based on the padding
    float initialPosX = visibleSize.width / 2; // Center horizontally
    float initialPosY = SpriteController::calculateScreenRatio(0.05f); // Padding from the bottom

    // Set the initial position of the player
    this->setPosition(Vec2(initialPosX, initialPosY));
    this->setAnchorPoint(Vec2(0.5, 0.5));
}

void PlayerGame3::setupTurret()
{
    if (GameController::getInstance()->isGameOver() || GameController::getInstance()->isPaused()) return;

    // Create turret sprite and add to tank
    turretSprite = Sprite::create("assets_game/player/tank_barrel_2.png");
    if (turretSprite) {
        turretSprite->setScale(SpriteController::updateSpriteScale(turretSprite, 0.08f));
        turretSprite->setAnchorPoint(Vec2(0.5, 0.0f)); // Center-bottom
        turretSprite->setPosition(Vec2((this->getContentSize().width / 2) + SpriteController::calculateScreenRatio(0.005f), this->getContentSize().height + SpriteController::calculateScreenRatio(0.016f)));
        this->addChild(turretSprite, Constants::ORDER_LAYER_PLAYER);
    }
    else {
        CCLOG("Failed to load texture for Turret");
    }
}

void PlayerGame3::setupEventListeners()
{
    // Add keyboard event listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerGame3::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerGame3::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // Add mouse event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(PlayerGame3::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(PlayerGame3::onMouseDown, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(PlayerGame3::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void PlayerGame3::setupManagers()
{
    playerMovement = new PlayerMovement(this, Constants::PLAYER_SPEED_GAME3);
}

void PlayerGame3::initAnimation()
{
    modelCharac = Sprite::createWithSpriteFrameName("tank_1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, Constants::PlayerScale3));
    this->addChild(modelCharac, Constants::ORDER_LAYER_PLAYER);

    auto animateCharac = Animate::create(createAnimation("tank_", 8, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
    createPhysicsBody();
}

void PlayerGame3::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D
        || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
    {
        playerMovement->onKeyPressed(keyCode);
    }
}

void PlayerGame3::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D
        || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
    {
        playerMovement->onKeyReleased(keyCode);
    }
}

void PlayerGame3::onMouseDown(Event* event)
{
    if (GameController::getInstance()->isPaused())
    {
        return;
    }

    isMouseDown = true;
}

void PlayerGame3::onMouseUp(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        isMouseDown = false;
    }
}

void PlayerGame3::shootBullet(const Vec2& target) {
    if (GameController::getInstance()->isGameOver() || GameController::getInstance()->isPaused()) return;

    Vec2 turretPosition = this->convertToWorldSpace(turretSprite->getPosition());
    if (!updateDistanceToMouse(turretPosition)) {
        return; // Do not shoot if the mouse is too close
    }

    if (timeSinceLastShot < shootDelay) return;
    if (Constants::QuantityBulletPlayerGame3 < 1) {
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::UPDATE_BULLET_LABEL, &Constants::QuantityBulletPlayerGame3);
        // Send notification to blink red
        SoundController::getInstance()->playSoundEffect(Constants::PlayerOutOfAmmoSFX);
        __NotificationCenter::getInstance()->postNotification("BlinkRedBadge", nullptr);
        return;
    }
        

    direction = target - this->getPosition();
    direction.normalize();
    if (direction.y < 0) {
        direction.y = 0;
    }

    std::vector<float> angles;
    if (bulletCount == 1) {
        angles = { 0.0f };
    }
    else if (bulletCount == 2) {
        angles = { -7.5f, 7.5f };
    }
    else if (bulletCount == 3) {
        angles = { -15.0f, 0.0f, 15.0f };
    }

    for (float angleOffset : angles) {
        float angleInRadians = CC_DEGREES_TO_RADIANS(angleOffset);
        Vec2 BulletTestDirection = Vec2(
            direction.x * cos(angleInRadians) - direction.y * sin(angleInRadians),
            direction.x * sin(angleInRadians) + direction.y * cos(angleInRadians)
        );

        auto BulletPlayerGame3 = BulletPoolPlayerGame3::getInstance()->getObject();
        BulletPlayerGame3->setPosition(turretPosition);
        BulletPlayerGame3->setDirection(BulletTestDirection);
        BulletPlayerGame3->spawn();
        this->getParent()->addChild(BulletPlayerGame3, Constants::ORDER_LAYER_PLAYER - 99);
        Constants::QuantityBulletPlayerGame3 --;
    }

    SoundController::getInstance()->playSoundEffect(Constants::PlayerGame3ShootSFX);
    timeSinceLastShot = 0.0f;

    // Send notification about bullet count change
    __NotificationCenter::getInstance()->postNotification("BulletCountChanged", nullptr);
}

void PlayerGame3::increaseBulletCount() {
    if (bulletCount < 3) {
        bulletCount++;
    }
}

void PlayerGame3::onMouseMove(Event* event)
{
    if (GameController::getInstance()->isPaused())
    {
        return;
    }

    EventMouse* e = (EventMouse*)event;
    _mousePos = Vec2(e->getCursorX(), e->getCursorY());

    updateTurretRotation();
}

void PlayerGame3::update(float delta)
{
    playerMovement->update(delta);

    // Get the visible size and origin of the screen
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Define padding
    float padding = SpriteController::calculateScreenRatio(0.05f); // Adjust this value as needed

    // Get the player's current position
    Vec2 currentPosition = this->getPosition();

    // Clamp the player's position within the screen boundaries with padding
    float clampedX = std::max(origin.x + padding, std::min(currentPosition.x, origin.x + visibleSize.width - padding));
    float clampedY = std::max(origin.y + padding, std::min(currentPosition.y, origin.y + visibleSize.height - padding));

    // Set the clamped position
    this->setPosition(Vec2(clampedX, clampedY));

    updateTurretRotation();

    timeSinceLastShot += delta;

    if (isMouseDown && timeSinceLastShot >= shootDelay)
    {
        shootBullet(_mousePos);
        timeSinceLastShot = 0.0f;
    }
}

void PlayerGame3::updateTurretRotation() {
    if (GameController::getInstance()->isGameOver() || GameController::getInstance()->isPaused()) return;

    if (turretSprite) {
        // Get the turret's world position
        Vec2 turretPosition = this->convertToWorldSpace(turretSprite->getPosition());

        // Calculate the direction vector from the turret to the mouse position
        Vec2 direction = _mousePos - turretPosition;

        // Normalize the direction vector
        direction.normalize();

        // Calculate the angle in degrees
        float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x));

        // Limit the rotation angle to prevent the turret from pointing downwards
        if (angle < 0.0f) {
            return;
        }
        else if (angle > 180.0f) {
            return;
        }

        // Set the turret's rotation (adjusting for the initial orientation)
        turretSprite->setRotation(-angle + 90);

        // Update the distance between the player and the mouse position
        updateDistanceToMouse(turretPosition);
    }
}

float PlayerGame3::calculateDistanceToMouse(const Vec2& position) {
    return position.distance(_mousePos);
}

bool PlayerGame3::updateDistanceToMouse(const Vec2& position) {
    distanceToMouse = calculateDistanceToMouse(position);

    // Change cursor color based on distance
    auto cursor = dynamic_cast<Cursor*>(this->getParent()->getChildByName("Cursor"));
    if (cursor) {
        if (distanceToMouse < SpriteController::calculateScreenRatio(Constants::DISTANCE_FROM_PLAYER_TO_POINTER)) {
            cursor->changeColor(Color3B::RED);
            return false;
        }
        else {
            cursor->changeColor(Color3B::GREEN);
            return true;
        }
    }
    return false;
}

Size PlayerGame3::GetSize() {
    return SpriteController::GetContentSizeSprite(modelCharac);
}

void PlayerGame3::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBodyFromPlist("physicsBody/PlayerGame3.plist", "PlayerGame3", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "PlayerGame3", originalSize, 0.9f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}