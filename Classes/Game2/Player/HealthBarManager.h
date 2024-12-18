#ifndef __HEALTH_BAR_H__
#define __HEALTH_BAR_H__

#include "cocos2d.h"

class HealthBar : public cocos2d::Node {
private:
    cocos2d::Sprite* backgroundFrame;   // Khung nền
    cocos2d::Sprite* avatarFrame;       // Khung avatar
    cocos2d::Sprite* healthBar;         // Thanh máu
    cocos2d::Label* healthText;         // Text hiển thị máu

    float maxHealth;  // Máu tối đa
    float currentHealth;  // Máu hiện tại

public:
    static HealthBar* create(const std::string& backgroundImage,
        const std::string& avatarImage,
        const std::string& healthBarImage);

    virtual bool init(const std::string& backgroundImage,
        const std::string& avatarImage,
        const std::string& healthBarImage);

    void updateHealth(float newHealth);  // Cập nhật lượng máu
    void setAvatar(const std::string& avatarImage); // Thay đổi hình avatar

    void setPositionCustom(const cocos2d::Vec2& position); // Cố định vị trí
    float getCurrentHealth() const { return currentHealth; } // Lấy lượng máu hiện tại
};

#endif // __HEALTH_BAR_H__
