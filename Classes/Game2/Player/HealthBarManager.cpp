#include "Game2/Player/HealthBarManager.h"
#include "Constants/Constants.h"

USING_NS_CC;

HealthBar* HealthBar::create(const std::string& backgroundImage,
    const std::string& avatarImage,
    const std::string& healthBarImage) {
    HealthBar* healthBar = new (std::nothrow) HealthBar();
    if (healthBar && healthBar->init(backgroundImage, avatarImage, healthBarImage)) {
        healthBar->autorelease();
        return healthBar;
    }
    CC_SAFE_DELETE(healthBar);
    return nullptr;
}

bool HealthBar::init(const std::string& backgroundImage,
    const std::string& avatarImage,
    const std::string& healthBarImage) {
    if (!Node::init()) {
        return false;
    }

    // Khung nền
    backgroundFrame = Sprite::create(backgroundImage);
    this->addChild(backgroundFrame);

    // Khung avatar
    avatarFrame = Sprite::create(avatarImage);
    avatarFrame->setPosition(Vec2(-backgroundFrame->getContentSize().width / 2 + avatarFrame->getContentSize().width / 2, 0));
    this->addChild(avatarFrame);

    // Thanh máu
    healthBar = Sprite::create(healthBarImage);
    healthBar->setAnchorPoint(Vec2(0, 0.5)); // Thanh máu chạy từ trái sang phải
    healthBar->setPosition(Vec2(-backgroundFrame->getContentSize().width / 2 + avatarFrame->getContentSize().width, 0));
    this->addChild(healthBar);

    // Text hiển thị máu
    healthText = Label::createWithTTF("0/0", Constants::FONT_GAME, 20);
    healthText->setPosition(Vec2(healthBar->getPositionX() + healthBar->getContentSize().width / 2, -20));
    this->addChild(healthText);

    // Giá trị mặc định
    maxHealth = 100;
    currentHealth = 100;
    updateHealth(currentHealth);

    return true;
}

void HealthBar::updateHealth(float newHealth) {
    currentHealth = newHealth;

    // Cập nhật tỉ lệ thanh máu
    float healthPercent = currentHealth / maxHealth;
    healthBar->setScaleX(healthPercent); // Điều chỉnh chiều dài

    // Cập nhật text
    std::string healthString = StringUtils::format("%.0f/%.0f", currentHealth, maxHealth);
    healthText->setString(healthString);
}

void HealthBar::setAvatar(const std::string& avatarImage) {
    avatarFrame->setTexture(avatarImage);
}

void HealthBar::setPositionCustom(const Vec2& position) {
    this->setPosition(position);
}