#ifndef __GAME1SCENE_SCENE_H__
#define __GAME1SCENE_SCENE_H__

#include "Background/Background.h"
#include "Game1/Items/CollectibleItem.h"
#include "Game1/Player/PlayerGame1.h"

#include "Enemy/SpawnEvent.h"
#include "Game1/Effect/EffectObject.h"
#include "Game1/Player/HealthPlayerGame1.h"
#include "Items/AmmoItem.h"
#include "Items/HealthItem.h"
#include "Skills/ShieldSkill.h"
#include "LoadingBar/CustomLoadingBar.h"
#include "Scene/BaseScene.h"
#include "ui/UILoadingBar.h"
#include "Controller/SoundController.h"
#include <functional>

class Game1Scene : public BaseScene {
public:
    // Public methods
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(Game1Scene);
    virtual void update(float delta);

    // New method to reset game state
    void resetGameState();

private:
    // Physics world
    cocos2d::PhysicsWorld* world;
    void setPhysicWorld(cocos2d::PhysicsWorld* m_world) { world = m_world; }

    // Player attributes
    PlayerAttributes* _playerAttributes;
    PlayerGame1* _player;
    HealthPlayerGame1* _healthPlayerGame1;
    bool _canTakeDamage;
    bool _isGameOver;
    bool _invincible;

    // Background
    Background* background;

    // Enemy management
    std::vector<Vec2> usedPositions;
    std::vector<int> previousSpawnEdges;

    // Music and sound
    float musicDuration;

    // UI elements
    CustomLoadingBar* customLoadingBar;
    cocos2d::ui::LoadingBar* _loadingBar;
    Sprite* border;

    // Enemy spawning
    void SpawnFallingRockAndBomb(cocos2d::Size size);
    void SpawnRandomBoom(cocos2d::Size size);
    void SpawnFanBullet(cocos2d::Size size);
    bool isPositionOccupied(const Vec2& position);
    void trackUsedPosition(const Vec2& position);
    void resetSchedules();

    // Collectible spawning
    void scheduleCollectibleSpawning();
    void SpawnCollectibleItem(const Size& size);
    Vec2 getRandomSpawnPosition(const Size& size);

    // Collision handling
    void setPhysicsBodyChar(cocos2d::PhysicsBody* physicBody, int num);
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    void setupContactListener();
    void handlePlayerDamage();
    void handleCollectibleCollision(Node* collectibleNode);

    // Game over handling
    void checkGameOver();

    // UI updates
    void updateLoadingBar(float dt);

    // JSON-based spawn schedule
    std::vector<SpawnEvent> spawnSchedule;
    std::unordered_map<std::string, std::function<void(const cocos2d::Size&)>> enemySpawnMap;

    // Method to initialize the spawn schedule from JSON
    void initializeSpawnSchedule();

    // Effect spawning
    void spawnEffect(const cocos2d::Size& size);
    float _lastEffectYPosition = -1.0f;
    float elapsedTime;

    // Initialization functions
    void initPhysics(const Size& visibleSize);
    void initBackground();
    void initPools();
    void initPlayer(const Size& visibleSize);
    void initUI(const Size& visibleSize);
    void initEvents();
    void initSound();
    void initSpawning();

    // Shield
    ShieldSkill* _shield;
    void activateShield();
    void deactivateShield();

    // Cursor
    void initCursor();

    //Preload 
    void preloadAssets();
};

#endif // __GAME1SCENE_SCENE_H__
