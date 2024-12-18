#include "Game1/Player/HealthPlayerGame1.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"
#include "cocos2d.h"

HealthPlayerGame1* HealthPlayerGame1::createHealth()
{
    HealthPlayerGame1* health = new (std::nothrow) HealthPlayerGame1();
    if (health && health->init())
    {
        health->autorelease();
        return health;
    }
    CC_SAFE_DELETE(health);
    return nullptr;
}

void HealthPlayerGame1::initHealthSprites(int health) {
    for (int i = 0; i < health; i++) { // Loop for player health sprites
        modelBackground = Sprite::create("assets_game/player/HP_Dot_BG.png"); // Load background sprite
        modelBackground->setScale(SpriteController::updateSpriteScale(modelBackground, 0.105f));
        this->addChild(modelBackground); // Add background sprite to the scene
        _backgroundSprites.push_back(modelBackground); // Store background sprite in vector

        modelHealth = Sprite::create("assets_game/player/HP_Dot.png"); // Load health sprite
        modelHealth->setScale(SpriteController::updateSpriteScale(modelHealth, 0.1f));
        this->addChild(modelHealth); // Add health sprite to the scene
        _healthSprites.push_back(modelHealth); // Store health sprite in vector
    }
    positionHealthSprites();
}

void HealthPlayerGame1::updateHealthSprites(int health) {
    int currentHealth = health;

    // Ensure the number of health sprites matches the current health
    if (currentHealth > _healthSprites.size()) {
        int additionalSprites = currentHealth - _healthSprites.size();
        for (int i = 0; i < additionalSprites; ++i) {
            modelBackground = Sprite::create("assets_game/player/HP_Dot_BG.png");
            modelBackground->setScale(SpriteController::updateSpriteScale(modelBackground, 0.105f));
            this->addChild(modelBackground);
            _backgroundSprites.push_back(modelBackground);

            modelHealth = Sprite::create("assets_game/player/HP_Dot.png");
            modelHealth->setScale(SpriteController::updateSpriteScale(modelHealth, 0.1f));
            this->addChild(modelHealth);
            _healthSprites.push_back(modelHealth);
        }
    }

    // Update visibility of health sprites
    for (int i = 0; i < _healthSprites.size(); i++) {
        if (i < currentHealth) {
            _healthSprites[i]->setVisible(true); // Show sprite if player has health
            _healthSprites[i]->setOpacity(255); // Ensure the sprite is fully visible
        }
        else {
            auto fadeOut = FadeOut::create(0.5f); // 0.5 second to fade out
            auto hideSprite = CallFunc::create([this, i]() {
                _healthSprites[i]->setVisible(false); // Hide the sprite after fade out
                });
            auto sequence = Sequence::create(fadeOut, hideSprite, nullptr); // Create sequence
            _healthSprites[i]->runAction(sequence); // Run fade out action
        }
    }
    positionHealthSprites();
}

void HealthPlayerGame1::positionHealthSprites() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float startX = SpriteController::calculateScreenRatio(Constants::PLAYER_HEALTH_PADDING_X_START);
    float startY = visibleSize.height - SpriteController::calculateScreenRatio(Constants::PLAYER_HEALTH_PADDING_Y);
    float spacing = SpriteController::calculateScreenRatio(Constants::PLAYER_HEALTH_PADDING_X);

    for (int i = 0; i < _healthSprites.size(); ++i) {
        auto spriteSize = SpriteController::GetContentSizeSprite(_healthSprites[i]);
        _backgroundSprites[i]->setPosition(Vec2(startX + i * (spriteSize.width + spacing), startY));
        _healthSprites[i]->setPosition(Vec2(startX + i * (spriteSize.width + spacing), startY));
    }
}
