#include "Game2/Game2Scene.h"
#include "Manager/PhysicsManager.h"
#include "Constants/Constants.h"
#include "Manager/BackgroundManager.h"
#include "Game2/Player/Petard.h"

#include "Game2/Enemy/Enemyh/MeleeEnemy.h"
#include "Game2/Enemy/Enemyh/SniperEnemy.h"
#include "Game2/Enemy/Enemyh/InvEnemy.h"
#include "Game2/Enemy/Enemyh/SuicideBomberEnemy.h"
#include "Game2/Enemy/Enemyh/BossEnemy.h"
#include "Game2/Enemy/Enemyh/RifleEnemy.h"
#include "Controller/GameController.h"
#include "Game2/Items/ItemsSpawn.h"
#include "ui/CocosGUI.h"

#include "audio/include/AudioEngine.h"
USING_NS_CC;

cocos2d::Scene* Game2Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Game2Scene::create();
    scene->addChild(layer);
    return scene;
}

bool Game2Scene::init() {
    if (!BaseScene::init()) {
        return false;
    }

    this->setSceneCreationFunc([]() -> cocos2d::Scene* {
        return Game2Scene::createScene();
        });
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();
    BackgroundManager::getInstance()->setBackground(this, "assets_game/gameplay/game2/bg_game_2.1.png", Constants::ORDER_LAYER_BACKGROUND);

    // Petard
    auto petard = Petard::create();
    if (petard)
    {
        petard->createPhysicsBody();
        this->addChild(petard);
        petard->setupInitialPosition();
        CCLOG("Petard added to scene");
    }
    else
    {
        CCLOG("Failed to add Petard to scene");
    }

    //Loading bar health Petard

    _player = PlayerGame2::createPlayerGame2();
    if (!_player) {
        return false;
    }

    _player->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    _player->setName("PlayerGame2");
    this->addChild(_player);
    setupKeyboardEventListeners();
    setupCursor();
    this->schedule([this](float delta) {
        spawnEnemies();
        }, 5.0f, "spawn_enemy_key");
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Game2Scene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();
    return true;
}

void Game2Scene::checkGameOver() {
    /* if (_playerAttributes->GetHealth() <= 0) {
         _isGameOver = true;
         auto gameOverLabel = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 48);
         gameOverLabel->setPosition(Director::getInstance()->getVisibleSize() / 2);
         this->addChild(gameOverLabel);

         GameController::getInstance()->GameOver(
             [this]() {
                 Director::getInstance()->end();
             },
             []() -> Scene* {
                 return Game2Scene::createScene();
             },
             Constants::pathSoundTrackGame1
         );
     }*/
}

void Game2Scene::resetGameState() {
    this->unscheduleAllCallbacks();
    _isGameOver = false;
    PlayerAttributes::getInstance().SetHealth(Constants::Player_Health2);
    _playerAttributes = &PlayerAttributes::getInstance();
    this->scheduleUpdate();
}

void Game2Scene::setupCursor() {
    _cursor = Cursor::create("assets_game/textures/Cursor/Cursor.png");
    _cursor->setScale(SpriteController::updateSpriteScale(_cursor, 0.03f));
    this->addChild(_cursor, Constants::ORDER_LAYER_CURSOR);
}

void Game2Scene::setupKeyboardEventListeners() {
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (_player) {
            _player->onKeyPressed(keyCode, event);
        }
        };
    eventListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (_player) {
            _player->onKeyReleased(keyCode, event);
        }
        };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
}

void Game2Scene::update(float delta) {
    if (_player) {
        _player->update(delta);
        const auto visibleSize = Director::getInstance()->getVisibleSize();
        const Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 pos = _player->getPosition();

        pos.x = std::max(origin.x, std::min(pos.x, origin.x + visibleSize.width));
        pos.y = std::max(origin.y, std::min(pos.y, origin.y + visibleSize.height));

        _player->setPosition(pos);
    }
    checkGameOver();
}

void Game2Scene::spawnEnemies() {
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 spawnPoints[] = {
        Vec2(585, 330),
        Vec2(1520, 460),
        Vec2(1020, 900)
    };
    for (const auto& point : spawnPoints) {
        float x = point.x * (visibleSize.width / 1920.0f);
        float y = point.y * (visibleSize.height / 1080.0f);
        //spawnEnemy("MeleeEnemy", Vec2(x + origin.x, y + origin.y));
        spawnEnemy("RifleEnemy", Vec2(x + origin.x, y + origin.y));
    }
}

void Game2Scene::spawnEnemy(const std::string& enemyType, const cocos2d::Vec2& position) {
    EnemyBase* enemy = nullptr;

    if (enemyType == "MeleeEnemy") {
        enemy = MeleeEnemy::create();
    }
    else if (enemyType == "RifleEnemy") {
        enemy = RifleEnemy::create();
    }
    /*else if (enemyType == "InvEnemy") {
        enemy = InvEnemy::create();
    }
    else if (enemyType == "SuicideBomberEnemy") {
        enemy = SuicideBomberEnemy::create();
    }
    else if (enemyType == "BossEnemy") {
        enemy = BossEnemy::create();
    }*/
    if (enemy) {
        enemy->setName("Enemy");
        enemy->setPosition(position);
        this->addChild(enemy);
        enemy->scheduleUpdate();
    }
}

bool Game2Scene::onContactBegin(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (!nodeA || !nodeB) {
        return false;
    }

    // Handle collision between player and items
    if ((nodeA->getName() == "PlayerGame2" && nodeB->getName() == "ItemsSpawn") ||
        (nodeB->getName() == "PlayerGame2" && nodeA->getName() == "ItemsSpawn")) {
        auto item = dynamic_cast<ItemsSpawn*>(nodeA->getName() == "PlayerGame2" ? nodeB : nodeA);
        if (item) {
            switch (item->getType()) {
            case ItemsSpawn::ItemType::AMMO:
                _player->pickUpAmmo(30);
                break;
            case ItemsSpawn::ItemType::HEALTH:
                _player->pickUpHealth(20);
                break;
            case ItemsSpawn::ItemType::GRENADE:
                _player->pickUpGrenade(1);
                break;
            }
            item->removeFromParent();
        }
    }


    // Handle collision between player and grenade
    if ((nodeA->getName() == "PlayerGame2" && nodeB->getName() == "Grenade") ||
        (nodeB->getName() == "PlayerGame2" && nodeA->getName() == "Grenade")) {
        if (nodeA->getName() == "Grenade") {
            nodeA->removeFromParent();
        }
        else {
            nodeB->removeFromParent();
        }
        _player->die();
    }

	// Enemy vs Grenade
    if ((nodeA->getName() == "Enemy" && nodeB->getName() == "Grenade") ||
        (nodeB->getName() == "Enemy" && nodeA->getName() == "Grenade")) {
        if (nodeA->getName() == "Grenade") {
            nodeA->removeFromParent();
        }
        else {
            nodeB->removeFromParent();
        }
        if (nodeA->getName() == "Enemy") {
            auto enemy = dynamic_cast<MeleeEnemy*>(nodeA);
            if (enemy) {
                enemy->die();
            }
        }
        else {
            auto enemy = dynamic_cast<MeleeEnemy*>(nodeB);
            if (enemy) {
                enemy->die();
            }
        }
    }

    if ((nodeA->getName() == "Bullet" && nodeB->getName() == "Enemy") ||
        (nodeB->getName() == "Bullet" && nodeA->getName() == "Enemy")) {
        auto bullet = (nodeA->getName() == "Bullet") ? nodeA : nodeB;
        auto enemy = (nodeA->getName() == "Enemy") ? nodeA : nodeB;

        auto enemyBase = dynamic_cast<EnemyBase*>(enemy);
        auto bulletGame2 = dynamic_cast<BulletGame2*>(bullet);
        if (enemyBase && bulletGame2) {
            enemyBase->takeDamage(bulletGame2->getDamage());
        }
        bullet->removeFromParent();
    }
    return true;
}
