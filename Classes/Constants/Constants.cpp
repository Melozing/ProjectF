#include "Constants.h"
#include "cocos2d.h"

// Initialize the scale values
std::string Constants::currentSoundTrackPath = "";

// Main Menu Soundtrack
const std::string Constants::MainMenuTrackPath = "assets_game/sounds/UI/MainMenuBGMusic.mp3";
const std::string Constants::ClickStartGameSFX = "assets_game/sounds/UI/ClickStartGameSFX.mp3";
const std::string Constants::ButtonClickSFX = "assets_game/sounds/UI/ButtonClickSFX.mp3";
const std::string Constants::OpenPanel = "assets_game/sounds/UI/OpenPanel.mp3";

//SCREEN
const cocos2d::Size Constants::DESIGN_SIZE = cocos2d::Size(960, 640);
const std::string Constants::GAME1_SCENE_NAME = "Game1Scene";
const std::string Constants::GAME2_SCENE_NAME = "Game2Scene";
const std::string Constants::GAME3_SCENE_NAME = "Game3Scene";
const std::string Constants::MAINMENU_SCENE_NAME = "MainMenu";

//UI
const int Constants::ORDER_LAYER_CURSOR = 9999;
const int Constants::ORDER_LAYER_UI = 99;
const int Constants::ORDER_LAYER_LAYOUT_UI = 98;
const int Constants::ORDER_LAYER_CHARACTER = 1;
const int Constants::ORDER_LAYER_PLAYER = 2;
const int Constants::ORDER_LAYER_ITEM = 3;
const int Constants::ORDER_LAYER_BACKGROUND = -99;
const float Constants::PADDING_VERTICAL_UI_PANEL = -0.07f;
const float Constants::PADDING_VERTICAL_BUTTONS_MAINMENU = 0.05f;
const float Constants::PADDING_VERTICAL_UI_SLIDER = 0.07f;
const float Constants::PADDING_VERTICAL_UI_SLIDER_PADDING = 0.7f;
const float Constants::PADDING_VERTICAL_UI_SLIDER_SPACING = 0.3f;
const float Constants::PADDING_VERTICAL_UI_EXITBUTTON = -0.11f;
const float Constants::PADDING_HORIZONTAL_UI_PANEL_BUTTON = 0.05f;
const float Constants::PADDING_HORIZONTAL_UI_EXIT_BUTTON = -0.14f;
const float Constants::PADDING_HORIZONTAL_UI_SLIDER = 0.15f;
const float Constants::PADDING_HORIZONTAL_UI_SLIDER_SPACING = 0.07f;

const std::string Constants::FONT_GAME = "fonts/Patriot-Black.otf";

//ITEM
const float Constants::ITEM_SCALE_FACTOR = 0.25f;
const float Constants::ITEM_EFFECT_DURATION = 0.3f;
const float Constants::ITEM_SIZE_RATIO = 0.07f;

//EFFECT_GAME1
const std::string Constants::EFFECT_EXPLOSION_NAME = "explosions_effect";
const float Constants::EFFECT_EXPLOSION_PADDING_SCREEN = 0.15f;
const float Constants::EFFECT_EXPLOSION_PADDING_PLAYER_AREA = 0.008f;
const float Constants::EFFECT_MIN_Y_DISTANCE = 0.1f;
const float Constants::EFFECT_MIN_Y_DISTANCE_BETWEEN_CALLS = 0.1f;

//ENEMY_GAME1
const std::string Constants::ENEMY_FLYING_BULLET_NAME = "FlyingBullet";
const std::string Constants::ENEMY_FALLING_ROCK_AND_BOOM_NAME = "FallingRock";
const std::string Constants::ENEMY_RANDOM_BOOM_NAME = "RandomBoom";
const float Constants::FLYING_BULLET_SPEED = 3.0f;
const float Constants::FALLINGROCK_START_Y = 0.05f;
const float Constants::FALLINGROCK_PADDING = 0.03f;
const float Constants::FANBULLET_OFFSET = 0.04f;
const float Constants::FALLINGROCK_ITEMS_OFFSET = 0.05f;
const float Constants::FALLINGROCK_ITEMS_SPEED = 255.0f;
const float Constants::RANDOMBOOM_RESTRICTEDHEIGHT = 0.12f;
const float Constants::RANDOMBOOM_RESTRICTEDWIDTH = 0.31f;
const float Constants::DISTANCE_ENEMY_FALLING = 0.05f;

//PlayerGame1
const float Constants::PLAYER_RESTRICTEDWIDTH = 0.285f;
const float Constants::PLAYER_RESTRICTEDHEIGHT = 0.15f;
const float Constants::PLAYER_MOVESPEED = 200.0f;
const int Constants::PLAYER_HEALTH = 3;

const float Constants::PLAYER_HEALTH_PADDING_X = 0.008f;
const float Constants::PLAYER_HEALTH_PADDING_X_START = 0.05f;

// SHIELD GAME 1
const float Constants::SHIELD_DURATION = 0.3f;
const float Constants::PLAYER_HEALTH_PADDING_Y = 0.05f;
const float Constants::SHIELD_TIME_OF_EXISTENCE = 5.0f;

//game1
const std::string Constants::pathSoundTrackGame1 = "assets_game/sounds/Game1/bg_music_game1.mp3";
const float Constants::soundtrackGame1Duration = 150.0f;

//game2
const int Constants::Player_Health2 = 100;
const float Constants::BulletSpeed = 1500.f;
const float Constants::PlayerSpeed = 200.0f;
const float Constants::PlayerScale = 0.7f;
const int Constants::InitialEnemyPosX = 300;
const int Constants::InitialEnemyPosY = 200;
const float Constants::AnimationFrameDelay = 0.1f;
const float Constants::EnemySpeed = 100.0f;
const float Constants::AttackRange = 50.0f;
const float Constants::EnemyScale = 0.3f;
const int Constants::BulletTag = 1;
const int Constants::EnemyTag = 2;
const int Constants::InitialPosX = 100;
const int Constants::InitialPosY = 100;
const float Constants::scaleFactor = 2.0f;
const float Constants::BulletScale = 0.3f;

//Petard game 2
const float Constants::PetardHealth = 100.0f;

//game2 Dame and Health
const int Constants::BulletDamage2 = 50;
const float Constants::GrenadeSpeed = 300.0f;
const float Constants::GrenadeThrowDistance = 500.0f;

//boss game2
const float Constants::BossScale = 2.3f;
const float Constants::BossSpeed = 50.0f;
const int Constants::BossTag = 3;
const int Constants::BossDamage = 10;
const int Constants::BossHealth = 200;
const float Constants::BossReloadTime = 1.5f;
const int Constants::BossDamageIncrease = 5;

//bitmask game2
const uint32_t Constants::player2_bitmask = 0x0001;
const uint32_t Constants::enemy2_bitmask = 0x0002;

//game3
const float Constants::GroundY = 70.0f;
const float Constants::DISTANCE_FROM_PLAYER_TO_POINTER = 0.07f;
const float Constants::BackgroundSpeed = 50.0f;
const float Constants::PlayerScale3 = 0.08f; 
const float Constants::PLAYER_SPEED_GAME3 = 250.0f;
const float Constants::BulletGame3Speed = 700.0f;
const float Constants::EnemyPlaneBulletGame3Speed = 150.0f;
const float Constants::EnemyPlaneBoomGame3Speed = 150.0f;
const float Constants::SpeedEnemyPlane_BOSS = 100.0f;
float Constants::HealthEnemyPlaneBoss = 1000.0f;
float Constants::CurrentHealthEnemyPlaneBoss = 0.0f;
const float Constants::BulletDamageGame3 = 60.0f;
int Constants::QuantityBulletPlayerGame3 = 100;
const std::string Constants::PlayerGame3Name = "PlayerGame3Name";
const std::string Constants::UPDATE_BULLET_LABEL = "UPDATE_BULLET_LABEL";

//game3 SFX
const std::string Constants::BossHealthSFX = "assets_game/sounds/Game3/SFX/BossHealthSFX.mp3";
const std::string Constants::PlayerGame3ShootSFX = "assets_game/sounds/Game3/SFX/PlayerShootSFX.mp3";
const std::string Constants::PlayerOutOfAmmoSFX = "assets_game/sounds/Game3/SFX/out_of_ammo.mp3";
const std::string Constants::EnemyCrepExplodeSFX = "assets_game/sounds/Game3/SFX/EnemyCrepExplodeSFX.mp3";
const std::string Constants::CityDamagedSFX = "assets_game/sounds/Game3/SFX/CityDamagedSFX.mp3";

const std::string Constants::pathSoundTrackGame3 = "assets_game/sounds/Game3/Music/EnemyGame3Phase1.mp3";
const std::string Constants::pathSoundBossGame3Phase1 = "assets_game/sounds/Game3/Music/BossGame3Phase1.mp3";

//VictoryPanel
const std::string Constants::VICTORY_SOUNDTRACK_PATH = "assets_game/sounds/Victory/victory.mp3";

//UserDefault UI
const std::string Constants::UD_musicVolume = "musicVolume";
const std::string Constants::UD_effectsVolume = "effectsVolume";