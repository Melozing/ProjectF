#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
#include "cocos2d.h"

class Constants {
public:
    // UI SOUND
    static std::string currentSoundTrackPath;
    const static std::string MainMenuTrackPath;
    const static std::string ClickStartGameSFX;
    const static std::string ButtonClickSFX;
    const static std::string OpenPanel;

    // Constants for enemy scale
    static const float FLYING_BULLET_SPEED;
    static const float FALLINGROCK_START_Y;
    static const float FALLINGROCK_PADDING;
    // PlayerGame1
    static const float PLAYER_RESTRICTEDWIDTH;
    static const float PLAYER_RESTRICTEDHEIGHT;
    static const float PLAYER_MOVESPEED;
    static const int PLAYER_HEALTH;
    static const float PLAYER_HEALTH_PADDING_X;
    static const float PLAYER_HEALTH_PADDING_X_START;
    static const float PLAYER_HEALTH_PADDING_Y;
    // SHIELD GAME 1
	static const float SHIELD_DURATION;
    static const float SHIELD_TIME_OF_EXISTENCE;
	// ITEM 
	static const float ITEM_SCALE_FACTOR;
	static const float ITEM_EFFECT_DURATION;
	static const float ITEM_SIZE_RATIO;
	// EFFECT_GAME1
    static const std::string EFFECT_EXPLOSION_NAME;
    static const float EFFECT_EXPLOSION_PADDING_SCREEN;
    static const float EFFECT_EXPLOSION_PADDING_PLAYER_AREA;
    static const float EFFECT_MIN_Y_DISTANCE;
    static const float EFFECT_MIN_Y_DISTANCE_BETWEEN_CALLS;
	// EnemyGame1
    static const std::string ENEMY_FLYING_BULLET_NAME;
    static const std::string ENEMY_FALLING_ROCK_AND_BOOM_NAME;
    static const std::string ENEMY_RANDOM_BOOM_NAME;
    static const float FANBULLET_OFFSET;
    static const float FALLINGROCK_ITEMS_OFFSET;
    static const float FALLINGROCK_ITEMS_SPEED;
    static const float RANDOMBOOM_RESTRICTEDHEIGHT;
    static const float RANDOMBOOM_RESTRICTEDWIDTH;
    static const float DISTANCE_ENEMY_FALLING;
    // Game1 sound
    static const std::string pathSoundTrackGame1;
    static const float soundtrackGame1Duration;

    // Screen
    static const cocos2d::Size DESIGN_SIZE;
    static const std::string GAME1_SCENE_NAME;
    static const std::string GAME2_SCENE_NAME;
    static const std::string GAME3_SCENE_NAME;
    static const std::string MAINMENU_SCENE_NAME;
    //Game2
    static const int Player_Health2;
    static const float TIME_TO_WIN;
    static const float BulletSpeed;
    static const float PlayerSpeed;
    static const float PlayerScale;
    static const int InitialEnemyPosX;
    static const int InitialEnemyPosY;
    static const float EnemySpeed;
    static const float EnemyScale;
    static const float AttackRange;
    static const float AnimationFrameDelay;
    static const int BulletTag;
    static const int EnemyTag;
    static const int InitialPosX;
    static const int InitialPosY;
    static const float scaleFactor;
    static const float BulletScale;
    static const int BulletDamage2;
    static const float GrenadeSpeed;
    static const float GrenadeThrowDistance;
	//Petard game 2
	static const float PetardHealth;
    //bossgame2
    static const float BossScale;
    static const float BossSpeed;
    static const int BossTag;
    static const int BossDamage;
    static const int BossHealth;
    static const float BossReloadTime;
    static const int BossDamageIncrease;
    //bitmask game2
    static const uint32_t player2_bitmask;
    static const uint32_t enemy2_bitmask;
    //game3
    static const float GroundY;
    static const float BackgroundSpeed ;
    static const float DISTANCE_FROM_PLAYER_TO_POINTER;
    static const float PlayerScale3;
    static const float PLAYER_SPEED_GAME3;
    static const float BulletGame3Speed;
    static const float EnemyPlaneBulletGame3Speed; //Speed Enemy Plane 3
	static const float EnemyPlaneBoomGame3Speed; //Speed Enemy Plane 1
	static const float SpeedEnemyPlane_BOSS; //Speed Enemy Plane 1
	static float HealthEnemyPlaneBoss;
	static float CurrentHealthEnemyPlaneBoss;
	static const float BulletDamageGame3;
	static int QuantityBulletPlayerGame3;
	static const std::string PlayerGame3Name;
	static const std::string UPDATE_BULLET_LABEL;
    //game3 SFX
    static const std::string BossHealthSFX;
    static const std::string PlayerGame3ShootSFX;
    static const std::string PlayerOutOfAmmoSFX;
    static const std::string EnemyCrepExplodeSFX;
    static const std::string CityDamagedSFX;

    static const std::string pathSoundTrackGame3;
    static const std::string pathSoundBossGame3Phase1;

	//VictoryPanel
	static const std::string VICTORY_SOUNDTRACK_PATH;

    //UI
    static const int ORDER_LAYER_CURSOR;
    static const int ORDER_LAYER_UI;
    static const int ORDER_LAYER_LAYOUT_UI;
    static const int ORDER_LAYER_CHARACTER;
    static const int ORDER_LAYER_PLAYER;
    static const int ORDER_LAYER_ITEM;
    static const int ORDER_LAYER_BACKGROUND;
    static const float PADDING_VERTICAL_BUTTONS_MAINMENU;
    static const float PADDING_VERTICAL_UI_PANEL;
    static const float PADDING_VERTICAL_UI_EXITBUTTON;
    static const float PADDING_VERTICAL_UI_SLIDER;
    static const float PADDING_VERTICAL_UI_SLIDER_PADDING;
    static const float PADDING_VERTICAL_UI_SLIDER_SPACING;
    static const float PADDING_HORIZONTAL_UI_PANEL_BUTTON;
    static const float PADDING_HORIZONTAL_UI_EXIT_BUTTON;
    static const float PADDING_HORIZONTAL_UI_SLIDER;
    static const float PADDING_HORIZONTAL_UI_SLIDER_SPACING;
    static const std::string FONT_GAME;

    //UserDefault UI
    static const std::string UD_musicVolume;
    static const std::string UD_effectsVolume;
};

#endif // __CONSTANTS_H__
