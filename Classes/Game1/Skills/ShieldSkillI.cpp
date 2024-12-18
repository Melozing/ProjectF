#include "Game1/Skills/ShieldSkill.h"
#include "Manager/ObjectPoolGame1.h"

USING_NS_CC;

ShieldSkill* ShieldSkill::create() {
    ShieldSkill* shield = new (std::nothrow) ShieldSkill();
    if (shield && shield->init()) {
        shield->autorelease();
        return shield;
    }
    CC_SAFE_DELETE(shield);
    return nullptr;
}

bool ShieldSkill::init() {
    if (!Sprite::init()) {
        return false;
    }
    _spriteBatchNode = SpriteBatchNode::create("assets_game/player/shield.png");
    _isActive = false;
    this->setVisible(true);
    this->setOpacity(255);
    this->initAnimation();
    return true;
}

void ShieldSkill::reset() {
    this->setVisible(true);
    this->setOpacity(255);
}


void ShieldSkill::initAnimation() {
    if (_spriteBatchNode->getParent() == nullptr) {
        this->addChild(_spriteBatchNode);
    }
    _shieldSprite = Sprite::createWithSpriteFrameName("shield1.png");
    _shieldSprite->setScale(SpriteController::updateSpriteScale(_shieldSprite, 0.13f));
    _spriteBatchNode->addChild(_shieldSprite);

    auto animateShield = Animate::create(createAnimation("shield", 15, 0.07f));
    _shieldSprite->runAction(RepeatForever::create(animateShield));
}

void ShieldSkill::activate(float duration) {
    if (!_isActive) {
        _isActive = true;
        this->setVisible(true);
        this->setOpacity(0);
        auto fadeIn = FadeIn::create(duration);
        this->runAction(Sequence::create(
            fadeIn,
            CallFunc::create([this]() { this->deactivate(); }),
            nullptr
        ));
    }
}

void ShieldSkill::deactivate() {
    if (_isActive) {
        _isActive = false;
        this->removeFromParentAndCleanup(false);
        ShieldSkillPool::getInstance()->returnObject(this);
    }
}

bool ShieldSkill::isActive() const {
    return _isActive;
}
