// Badge.cpp
#include "Badge.h"

USING_NS_CC;

Badge* Badge::createBadge(const std::string& badgeImage, const std::string& font, float fontSize) {
    Badge* badge = new (std::nothrow) Badge();
    if (badge && badge->init(badgeImage, font, fontSize)) {
        badge->autorelease();
        return badge;
    }
    CC_SAFE_DELETE(badge);
    return nullptr;
}

bool Badge::init(const std::string& badgeImage, const std::string& font, float fontSize) {
    if (!Node::init()) {
        return false;
    }

    fontLabel = font;
    fontSizeLabel = fontSize;
    // Create the badge sprite
    badgeSprite = Sprite::create(badgeImage);
    if (!badgeSprite) {
        return false;
    }
    this->addChild(badgeSprite);

    // Create the label
    badgeLabel = Label::createWithTTF("0", font, fontSize);
    if (!badgeLabel) {
        return false;
    }
    badgeLabel->setPosition(badgeSprite->getContentSize() / 2);
    badgeSprite->addChild(badgeLabel);

    return true;
}

void Badge::setBadgePosition(const Vec2& position) {
    this->setPosition(position);
}

void Badge::updateLabel(std::string& text) {
    if (!badgeLabel)
    {
        badgeLabel = Label::createWithTTF(text, fontLabel, fontSizeLabel);
        badgeLabel->setPosition(badgeSprite->getContentSize() / 2);
        badgeSprite->addChild(badgeLabel);
    }
    badgeLabel->setString(text);
}

void Badge::blinkRed() {
    if (isBlinking) return; 

    isBlinking = true; 
    auto tintToRed = TintTo::create(0.1f, 255, 0, 0);
    auto tintToNormal = TintTo::create(0.1f, 255, 255, 255);
    auto sequence = Sequence::create(tintToRed, tintToNormal, CallFunc::create([this]() {
        isBlinking = false; 
        }), nullptr);
    badgeSprite->runAction(sequence);
}

