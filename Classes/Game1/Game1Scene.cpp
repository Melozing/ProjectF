#include "Game1/Game1Scene.h"
#include "Manager/ObjectPoolGame1.h"

#include "Game1/Player/HealthPlayerGame1.h"

#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "Controller/GameController.h"
#include "Game1/Effect/EffectObjectPool.h"

#include "Manager/PositionManager.h"
#include "Constants/Constants.h"
#include "ui/UILoadingBar.h"
#include "Manager/BackgroundManager.h"

#include <ctime> 
#include "json/document.h"
#include "json/filereadstream.h"
#include <fstream>
#include <cmath> 

USING_NS_CC;
using namespace cocos2d::experimental;

cocos2d::Scene* Game1Scene::createScene() {
    auto scene = Scene::createWithPhysics(); // Create scene with physics
    //scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Game1Scene::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}


bool Game1Scene::init() {
    if (!BaseScene::init()) return false;

    srand(static_cast<unsigned int>(time(nullptr)));

    this->setSceneCreationFunc([]() -> cocos2d::Scene* {
        return Game1Scene::createScene();
        });

    _isGameOver = false;
    PlayerAttributes::getInstance().SetHealth(Constants::PLAYER_HEALTH);
    _playerAttributes = &PlayerAttributes::getInstance();
    _canTakeDamage = true;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    preloadAssets();
    initPhysics(visibleSize);
    initBackground();
    initPools();
    initPlayer(visibleSize);
    initUI(visibleSize);
    initEvents();
    initSound();
    initSpawning();
    initCursor();
    setupContactListener();

    this->scheduleUpdate();
    this->scheduleCollectibleSpawning();
    return true;
}

void Game1Scene::preloadAssets() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/landmine.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/pre_explosion.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/ExplosionLandmine.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/flying_bullet.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/falling_rock.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/landmine.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/FallingTree.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/flying_bullet.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/flying_bullet_left.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/warning_rocket.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/rocket.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/items/Ammo.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/items/Health.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/Canon.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/shield.plist");
}

void Game1Scene::initCursor() {
    // Create and add the custom cursor
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
    _cursor = Cursor::create("assets_game/UXUI/Main_Menu/pointer.png");
    _cursor->setAnchorPoint(Vec2(0.5, 0.5));
    _cursor->setScale(SpriteController::updateSpriteScale(_cursor, 0.03f));
    _cursor->setVisible(true);
    _cursor->setInitialPosition();
    if (_cursor) {
        this->addChild(_cursor, Constants::ORDER_LAYER_CURSOR); // Add cursor to the scene with z-order 1
    }
}

void Game1Scene::initPhysics(const Size& visibleSize) {
    auto edgeBody = cocos2d::PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 0);
    edgeBody->setCollisionBitmask(0x04);
    edgeBody->setContactTestBitmask(true);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    edgeNode->setPhysicsBody(edgeBody);
    addChild(edgeNode);
}

void Game1Scene::initBackground() {
    background = Background::createBackground("assets_game/gameplay/BG_loop_new.png", 150.0f);
    this->addChild(background, Constants::ORDER_LAYER_BACKGROUND);
    //BackgroundManager::getInstance()->setBackground(this, "assets_game/gameplay/background_light_layer.png", Constants::ORDER_LAYER_LAYOUT_UI - 1);
}

void Game1Scene::initPools() {
    //FlyingBulletPool::getInstance()->resetPool();
    FallingRockPool::getInstance()->resetPool();
    RandomBoomPool::getInstance()->resetPool();
    FanBulletPool::getInstance()->resetPool();
    FallingTreePool::getInstance()->resetPool();
    HealthItemPool::getInstance()->resetPool();
    AmmoItemPool::getInstance()->resetPool();
    ShieldSkillPool::getInstance()->resetPool();

    FallingRockPool::getInstance()->initPool(10);
    RandomBoomPool::getInstance()->initPool(10);
    FanBulletPool::getInstance()->initPool(50);
    FallingTreePool::getInstance()->initPool(10);
    HealthItemPool::getInstance()->initPool(10);
    AmmoItemPool::getInstance()->initPool(10);
    ShieldSkillPool::getInstance()->initPool(10);
    EffectObjectPool::getInstance()->initPool(20, "assets_game/fx/explosions_effect.png", "assets_game/fx/explosions_effect.plist");
}

void Game1Scene::initPlayer(const Size& visibleSize) {
    _player = PlayerGame1::createPlayer();
    _player->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    _healthPlayerGame1 = HealthPlayerGame1::createHealth();
    _healthPlayerGame1->initHealthSprites(_playerAttributes->GetHealth());
    this->addChild(_healthPlayerGame1, Constants::ORDER_LAYER_LAYOUT_UI);

    _player->createPhysicsBody();
    addChild(_player, Constants::ORDER_LAYER_CHARACTER);
}

void Game1Scene::initUI(const Size& visibleSize) {
    customLoadingBar = CustomLoadingBar::create("assets_game/UXUI/Loading/Load_Bar_Fg.png", "assets_game/UXUI/Loading/Load_Bar_Bg.png", 0.25f);
    customLoadingBar->setLoadingBarRotation(-90);
    customLoadingBar->setLoadingBarPosition(Vec2(visibleSize.width - customLoadingBar->getLoadingBar()->getContentSize().height / 2, visibleSize.height / 2));

    // Calculate the position for the border
    auto loadingPos = customLoadingBar->getLoadingBar()->getPosition();

    customLoadingBar->setBorderPosition(loadingPos);
    customLoadingBar->setBorderRotation(-90);
    customLoadingBar->setBorderScale(SpriteController::updateSpriteScale(customLoadingBar->getBorder(), 0.265f));

    this->addChild(customLoadingBar, Constants::ORDER_LAYER_UI);
    _loadingBar = customLoadingBar->getLoadingBar();
}

void Game1Scene::initEvents() {
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            if (_player) {
                _player->onKeyPressed(keyCode, event);
            }
            break;
        default:
            break;
        }
        };

    eventListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            if (_player) {
                _player->onKeyReleased(keyCode, event);
            }
            break;
        default:
            break;
        }
        };
}

void Game1Scene::setupContactListener() {
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Game1Scene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void Game1Scene::initSound() {
    if (!FileUtils::getInstance()->isFileExist(Constants::pathSoundTrackGame1)) {
        CCLOG("Error: Music file does not exist!");
        return;
    }

    SoundController::getInstance()->preloadMusic(Constants::pathSoundTrackGame1);
    SoundController::getInstance()->playMusic(Constants::pathSoundTrackGame1, false);

    musicDuration = Constants::soundtrackGame1Duration;

    //SoundController::getInstance()->setMusicVolume(Constants::pathSoundTrackGame1, 0.0f);

    this->schedule([this](float dt) {
        updateLoadingBar(dt);
        }, "loading_bar_update_key");
}



void Game1Scene::initSpawning() {
    enemySpawnMap["FallingRock"] = [this](const cocos2d::Size& size) { SpawnFallingRockAndBomb(size); };
    enemySpawnMap["RandomBoom"] = [this](const cocos2d::Size& size) { SpawnRandomBoom(size); };
    enemySpawnMap["FanBullet"] = [this](const cocos2d::Size& size) { SpawnFanBullet(size); };
    enemySpawnMap["Effect"] = [this](const cocos2d::Size& size) { spawnEffect(size); };

    initializeSpawnSchedule();

    this->schedule([this](float dt) {
        elapsedTime += dt;

        for (auto& event : spawnSchedule) {
            if (!event.spawned && elapsedTime >= event.spawnTime) {
                enemySpawnMap[event.enemyType](Director::getInstance()->getVisibleSize());
                event.spawned = true;
            }
        }
        }, "json_based_spawning_key");
}


void Game1Scene::setPhysicsBodyChar(PhysicsBody* physicBody, int num) {
    physicBody->setCollisionBitmask(num);
    physicBody->setContactTestBitmask(true);
    physicBody->setDynamic(false);
    physicBody->setGravityEnable(false);
}

bool Game1Scene::onContactBegin(PhysicsContact& contact) {
    if (_playerAttributes->IsDead() || _invincible) return true;
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    auto nodeA = bodyA->getNode();
    auto nodeB = bodyB->getNode();

    if (!nodeA || !nodeB) return true;

    // Check if player is involved in the collision
    bool isPlayerInvolved = (nodeA == _player || nodeB == _player);
    if (!isPlayerInvolved) return true;

    // Determine the other node involved in the collision
    auto otherNode = (nodeA == _player) ? nodeB : nodeA;

    if (auto landmine = dynamic_cast<Landmine*>(otherNode)) {
        landmine->triggerPreExplosion();
    }
    else if (otherNode->getPhysicsBody()->getCollisionBitmask() == 0x02) { // Explosion collision bitmask
        handlePlayerDamage();
        otherNode->removeComponent(otherNode->getPhysicsBody());
    }
    else if (otherNode->getPhysicsBody()->getCollisionBitmask() == 0x03) { // Collectible item collision bitmask
        handleCollectibleCollision(otherNode);
    }

    return true;
}


void Game1Scene::handlePlayerDamage() {
    if (_shield) {
        CCLOG("Shield is active, deactivating shield.");
        deactivateShield();
    }
    else if (_canTakeDamage && !_invincible) {
        CCLOG("Player takes damage.");
        _playerAttributes->TakeDamage(1);
        _player->playDamageEffect();
        _healthPlayerGame1->updateHealthSprites(_playerAttributes->GetHealth());
        this->checkGameOver();
        _canTakeDamage = false;
        this->scheduleOnce([this](float) { _canTakeDamage = true; }, 1.0f, "damage_delay_key");
    }
}

void Game1Scene::handleCollectibleCollision(Node* collectibleNode) {
    auto collectible = dynamic_cast<CollectibleItem*>(collectibleNode);
    if (collectible) {
        if (auto healthItem = dynamic_cast<HealthItem*>(collectible)) {
            healthItem->applyEffect();
            _player->playHealthIncreaseEffect();
            _healthPlayerGame1->updateHealthSprites(_playerAttributes->GetHealth());
        }
        else if (auto ammoItem = dynamic_cast<AmmoItem*>(collectible)) {
            ammoItem->applyEffect();
            activateShield();
        }
    }
}

void Game1Scene::activateShield() {
    if (_shield) {
        _shield->removeFromParentAndCleanup(true);
    }
    _shield = ShieldSkillPool::getInstance()->getObject();
    if (_shield) {
        if (_shield->getParent() == nullptr) {
            this->addChild(_shield, Constants::ORDER_LAYER_PLAYER);
        }
        _shield->reset();
        _shield->setPosition(_player->getPosition());
        _shield->activate(Constants::SHIELD_TIME_OF_EXISTENCE);
        _player->_shield = _shield;
        _player->setShield(_shield);
        this->scheduleOnce([this](float) {
            _shield = nullptr;
            }, Constants::SHIELD_TIME_OF_EXISTENCE, "deactivate_shield_key");
    }
}


void Game1Scene::deactivateShield() {
    if (_shield) {
        _shield->deactivate();
        _shield->removeFromParent();
        _shield = nullptr;
        _invincible = true;
        this->scheduleOnce([this](float) { _invincible = false; }, 0.3f, "invincibility_delay_key");
    }
}

void Game1Scene::checkGameOver() {
    if (_playerAttributes->IsDead() && !_isGameOver) {
        _isGameOver = true;
        _player->removePhysicsBody();
        _player->fadeOutAndDisable();
        this->runAction(Sequence::create(
            DelayTime::create(2.0f),
            CallFunc::create([this]() {
                GameController::getInstance()->GameOver(
                    [this]() {
                        Director::getInstance()->end();
                    },
                    []() -> Scene* {
                        return Game1Scene::createScene();
                    },
                    Constants::pathSoundTrackGame1 // Add the soundtrack path here
                );
                }),
            nullptr
        ));
    }
}


void Game1Scene::updateLoadingBar(float dt) {
    if (_isGameOver) return;

    float currentPercent = _loadingBar->getPercent();
    if (musicDuration > 0) {
        float increment = (dt / musicDuration) * 100.0f;
        currentPercent += increment;

        if (currentPercent >= 100.0f) {
            currentPercent = 100.0f;
            GameController::getInstance()->Victory(
                []() {
                    Director::getInstance()->end();
                },
                []() -> Scene* {
                    return Game1Scene::createScene();
                },
                Constants::pathSoundTrackGame1
            );
            _cursor->setVisible(false);
            _isGameOver = true;
        }
        _loadingBar->setPercent(currentPercent);
    }
}

void Game1Scene::update(float delta) {
    background->update(delta);
}

void Game1Scene::resetSchedules() {
    this->unschedule("json_based_spawning_key");
    this->unschedule("loading_bar_update_key");
    this->unschedule("damage_delay_key");
    // Add any other schedules you need to reset
}

void Game1Scene::resetGameState() {
    // Unschedule existing tasks
    resetSchedules();

    // Reset game state
    _isGameOver = false;
    PlayerAttributes::getInstance().SetHealth(Constants::PLAYER_HEALTH);
    _playerAttributes = &PlayerAttributes::getInstance();
    _canTakeDamage = true;
    _shield->removeFromParentAndCleanup(true);

    // Reset elapsed time
    elapsedTime = 0.0f;

    // Reinitialize game elements
    initPools();
    initPlayer(Director::getInstance()->getVisibleSize());
    initUI(Director::getInstance()->getVisibleSize());
    initEvents();
    initSound();
    initSpawning();

    // Reschedule tasks
    this->scheduleUpdate();
    this->scheduleCollectibleSpawning();
}

// Function to initialize the spawn schedule from JSON
void Game1Scene::initializeSpawnSchedule() {
    // Log the start of the initialization

    // Example of reading a JSON file and initializing the spawn schedule
    std::string filePath = FileUtils::getInstance()->fullPathForFilename("json/spawn_enemies_game1.json");

    FILE* fp = fopen(filePath.c_str(), "rb");
    if (!fp) {
        CCLOG("Failed to open spawn schedule file.");
        return;
    }

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document document;
    document.ParseStream(is);
    fclose(fp);

    if (document.HasParseError()) {
        CCLOG("Error parsing spawn schedule JSON.");
        return;
    }

    if (!document.IsArray()) {
        CCLOG("Spawn schedule JSON is not an array.");
        return;
    }

    for (const auto& item : document.GetArray()) {
        if (item.IsObject()) {
            std::string enemyType = item["enemyType"].GetString();
            float spawnTime = item["spawnTime"].GetFloat();
            spawnSchedule.push_back(SpawnEvent{ spawnTime, enemyType, false });
        }
    }
}

Vec2 Game1Scene::getRandomSpawnPosition(const Size& size) {
    float restrictedWidth = SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDWIDTH);
    float centerX = size.width / 2;
    float halfRestrictedWidth = restrictedWidth / 2;
    float minX = centerX - halfRestrictedWidth;
    float maxX = centerX + halfRestrictedWidth;

    int spawnOption = rand() % 3;
    Vec2 spawnPosition;
    switch (spawnOption) {
    case 0:
        spawnPosition = Vec2(centerX, size.height + SpriteController::calculateScreenRatio(Constants::FALLINGROCK_START_Y));
        break;
    case 1:
        spawnPosition = Vec2(maxX - SpriteController::calculateScreenRatio(Constants::FALLINGROCK_PADDING), size.height + SpriteController::calculateScreenRatio(Constants::FALLINGROCK_START_Y));
        break;
    case 2:
        spawnPosition = Vec2(minX + SpriteController::calculateScreenRatio(Constants::FALLINGROCK_PADDING), size.height + SpriteController::calculateScreenRatio(Constants::FALLINGROCK_START_Y));
        break;
    }
    return spawnPosition;
}

void Game1Scene::SpawnFanBullet(cocos2d::Size size) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    int numBullets = 5;
    float angleSpread = 95.0f;
    float startAngle = -angleSpread / 2;
    float angleIncrement = angleSpread / (numBullets - 1);

    // Randomly choose an edge: 0 for top, 1 for left, 2 for right
    int edge;
    do {
        edge = rand() % 3;
    } while (std::find(previousSpawnEdges.begin(), previousSpawnEdges.end(), edge) != previousSpawnEdges.end());

    previousSpawnEdges.push_back(edge);
    if (previousSpawnEdges.size() > 2) {
        previousSpawnEdges.erase(previousSpawnEdges.begin());
    }
    Vec2 spawnPosition;
    float baseAngle;
    float offset = SpriteController::calculateScreenRatio(Constants::FANBULLET_OFFSET); // Adjust this value to move the spawn position outside the screen
    switch (edge) {
    case 0: // Top edge
        spawnPosition = Vec2(visibleSize.width / 2, visibleSize.height + offset);
        baseAngle = -90.0f;
        break;
    case 1: // Left edge
        spawnPosition = Vec2(-offset, visibleSize.height / 2);
        baseAngle = 0.0f;
        break;
    case 2: // Right edge
        spawnPosition = Vec2(visibleSize.width + offset, visibleSize.height / 2);
        baseAngle = 180.0f;
        break;
    }

    for (int i = 0; i < numBullets; ++i) {
        float angle = baseAngle + startAngle + i * angleIncrement;
        //float angle = baseAngle + startAngle * angleIncrement;
        FanBullet* fanBullet = FanBulletPool::getInstance()->getObject();
        if (fanBullet) {
            // Spawn the FanBullet
            fanBullet->spawn(spawnPosition, angle);
            // Add the FanBullet to the scene
            this->addChild(fanBullet, Constants::ORDER_LAYER_CHARACTER);
        }
    }
}

void Game1Scene::SpawnFallingRockAndBomb(Size size) {
    Vec2 spawnPosition = getRandomSpawnPosition(size);
    int randomEnemy = rand() % 3;

    switch (randomEnemy) {
    case 0: {
        // Spawn FallingRock
        auto fallingRock = FallingRockPool::getInstance()->getObject();
        if (fallingRock) {
            fallingRock->spawn(spawnPosition);
            fallingRock->createPhysicsBody();
            this->addChild(fallingRock);
        }
        break;
    }
    case 1: {
        // Spawn FallingTree
        auto fallingTree = FallingTreePool::getInstance()->getObject();
        if (fallingTree) {
            fallingTree->spawn(spawnPosition);
            fallingTree->createPhysicsBody();
            this->addChild(fallingTree);
        }
        break;
    }
    case 2: {
        // Spawn Landmine
        auto landmine = LandminePool::getInstance()->getObject();
        if (landmine) {
            landmine->spawn(spawnPosition);
            landmine->createPhysicsBody();
            this->addChild(landmine);
        }
        break;
    }
    default:
        break;
    }
}

//void Game1Scene::SpawnFlyingBullet(cocos2d::Size size, bool directionLeft) {
//    Vec2 spawnPosition = directionLeft ? Vec2(-50, _player->getPosition().y) : Vec2(size.width + 50, _player->getPosition().y);
//
//    auto flyingBullet = FlyingBulletPool::getInstance()->getEnemy();
//    if (flyingBullet) {
//        flyingBullet->reset();
//        flyingBullet->initAnimation(directionLeft);
//        flyingBullet->setPosition(spawnPosition);
//
//        float targetX = directionLeft ? size.width + 50 : -50;
//        auto moveAction = MoveTo::create(Constants::FLYING_BULLET_SPEED, Vec2(targetX, _player->getPosition().y));
//
//        auto sequence = Sequence::create(moveAction, CallFunc::create([flyingBullet, this]() {
//            flyingBullet->removeFromParent();
//            FlyingBulletPool::getInstance()->returnEnemy(flyingBullet);
//            }), nullptr);
//
//        Size reducedSize = Size(flyingBullet->GetSize().width * 0.65, flyingBullet->GetSize().height * 0.65); // Reduce size by 10%
//        auto flyingBulletBody = PhysicsBody::createBox(reducedSize);
//        setPhysicsBodyChar(flyingBulletBody, 0x02);
//        flyingBullet->setPhysicsBody(flyingBulletBody);
//
//        flyingBullet->runAction(sequence);
//        this->addChild(flyingBullet, Constants::ORDER_LAYER_CHARACTER - 1);
//    }
//}

void Game1Scene::SpawnRandomBoom(cocos2d::Size size) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 spawnPosition = Vec2(visibleSize.width / 2, visibleSize.height / 2);

    auto randomBoom = RandomBoomPool::getInstance()->getObject();
    if (randomBoom) {
        randomBoom->spawn(spawnPosition);
        this->addChild(randomBoom, Constants::ORDER_LAYER_CHARACTER - 1);
    }
}

void Game1Scene::SpawnCollectibleItem(const Size& size) {
    Vec2 spawnPosition = getRandomSpawnPosition(size);
    if (!isPositionOccupied(spawnPosition)) {
        // Randomly choose between HealthItem and AmmoItem
        CollectibleItem* item = nullptr;
        if (rand() % 2 == 0) {
            item = HealthItemPool::getInstance()->getObject();
        }
        else {
            item = AmmoItemPool::getInstance()->getObject();
        }
        if (item) {
            item->spawn(spawnPosition);
            this->addChild(item, Constants::ORDER_LAYER_ITEM);
        }
        else {
            CCLOG("Failed to create CollectibleItem");
        }
    }
}

void Game1Scene::trackUsedPosition(const Vec2& position) {
    usedPositions.push_back(position);
}

// Schedule the spawning of collectible items
void Game1Scene::scheduleCollectibleSpawning() {
    this->schedule([this](float dt) {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        this->SpawnCollectibleItem(visibleSize); // Call the function to spawn items
        }, 5.3f, "collectible_item_spawn_key"); // Adjust the interval as needed
}

bool Game1Scene::isPositionOccupied(const Vec2& position) {
    for (auto child : this->getChildren()) {
        // Check for CollectibleItem
        if (auto collectible = dynamic_cast<CollectibleItem*>(child)) {
            if (collectible->getPosition().distance(position) < collectible->GetSize().width) {
                return true; // Position is occupied
            }
        }
        // Check for FallingRock
        else if (auto fallingRock = dynamic_cast<FallingRock*>(child)) {
            if (fallingRock->getPosition().distance(position) < fallingRock->GetSize().width) {
                return true; // Position is occupied
            }
        }
    }
    return false; // Position is free
}

void Game1Scene::spawnEffect(const cocos2d::Size& size) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float padding = SpriteController::calculateScreenRatio(Constants::EFFECT_EXPLOSION_PADDING_SCREEN); // Padding from the edges
    float playerPadding = SpriteController::calculateScreenRatio(Constants::EFFECT_EXPLOSION_PADDING_PLAYER_AREA); // Padding from the player's movement area
    float minYDistance = SpriteController::calculateScreenRatio(Constants::EFFECT_MIN_Y_DISTANCE); // Minimum y-distance between effect pairs
    float minYDistanceBetweenCalls = SpriteController::calculateScreenRatio(Constants::EFFECT_MIN_Y_DISTANCE_BETWEEN_CALLS); // Minimum y-distance between consecutive calls

    float minX = visibleSize.width / 2 - SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDWIDTH) / 2 - playerPadding;
    float maxX = visibleSize.width / 2 + SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDWIDTH) / 2 + playerPadding;
    float minY = visibleSize.height / 2 - SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDHEIGHT) / 2 - playerPadding;
    float maxY = visibleSize.height / 2 + SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDHEIGHT) / 2 + playerPadding;

    cocos2d::Vec2 spawnPositionLeft1, spawnPositionRight1, spawnPositionLeft2, spawnPositionRight2;
    float yPosition1, yPosition2;
    do {
        yPosition1 = cocos2d::RandomHelper::random_real(padding, visibleSize.height - padding);
        yPosition2 = cocos2d::RandomHelper::random_real(padding, visibleSize.height - padding);
        spawnPositionLeft1 = cocos2d::Vec2(padding, yPosition1);
        spawnPositionRight1 = cocos2d::Vec2(visibleSize.width - padding, yPosition1);
        spawnPositionLeft2 = cocos2d::Vec2(padding, yPosition2);
        spawnPositionRight2 = cocos2d::Vec2(visibleSize.width - padding, yPosition2);
    } while (!PositionManager::getInstance().isOutsidePlayerMovementArea(spawnPositionLeft1, visibleSize, minX, maxX, minY, maxY) ||
        !PositionManager::getInstance().isOutsidePlayerMovementArea(spawnPositionRight1, visibleSize, minX, maxX, minY, maxY) ||
        !PositionManager::getInstance().isOutsidePlayerMovementArea(spawnPositionLeft2, visibleSize, minX, maxX, minY, maxY) ||
        !PositionManager::getInstance().isOutsidePlayerMovementArea(spawnPositionRight2, visibleSize, minX, maxX, minY, maxY) ||
        std::abs(yPosition1 - yPosition2) < minYDistance ||
        (_lastEffectYPosition != -1.0f && std::abs(yPosition1 - _lastEffectYPosition) < minYDistanceBetweenCalls));

    // Update the last Y position
    _lastEffectYPosition = yPosition1;

    // spawnPositionLeft1
    auto effectLeft1 = EffectObjectPool::getInstance()->getEffectObject();
    effectLeft1->setPosition(spawnPositionLeft1);
    effectLeft1->playAnimation(Constants::EFFECT_EXPLOSION_NAME, 6, 0.1f);
    this->addChild(effectLeft1, Constants::ORDER_LAYER_CHARACTER);

    // spawnPositionRight1
    auto effectRight1 = EffectObjectPool::getInstance()->getEffectObject();
    effectRight1->setPosition(spawnPositionRight1);
    effectRight1->playAnimation(Constants::EFFECT_EXPLOSION_NAME, 6, 0.1f);
    this->addChild(effectRight1, Constants::ORDER_LAYER_CHARACTER);
}