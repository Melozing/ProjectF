#include "EffectObject.h"
#include "Manager/SpriteBatchNodeManager.h"
#include "Manager/SpriteFrameCacheManager.h"
#include "EffectObjectPool.h" 
#include "Constants/Constants.h" 

EffectObject* EffectObject::create(const std::string& plistFile, const std::string& textureFile) {
    EffectObject* sprite = new (std::nothrow) EffectObject();
    if (sprite) {
        sprite->setPlistFile(plistFile);
        SpriteFrameCacheManager::getInstance().loadSpriteFrames(plistFile);
        sprite->setTextureFile(textureFile);
        if (sprite->init()) {
            sprite->autorelease();
            return sprite;
        }
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

EffectObject::EffectObject() {
    _spriteBatchNode = nullptr; // Initialize SpriteBatchNode to nullptr
    _effectSprite = nullptr; // Initialize effect sprite to nullptr
}

EffectObject::~EffectObject() {
    // Check if the sprite frames are still being used before removing them from cache
    //if (cocos2d::SpriteFrameCache::getInstance()->isSpriteFramesWithFileLoaded(_plistFile)) {
    //    //cocos2d::SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(_plistFile);
    //}
}

bool EffectObject::init() {
    if (!Sprite::init()) {
        return false;
    }

    // Load sprite frames

    // Create sprite batch node
    _spriteBatchNode = SpriteBatchNode::create(_textureFile);
    this->addChild(_spriteBatchNode);
    return true;
}

void EffectObject::setPlistFile(const std::string& plistFile) {
    _plistFile = plistFile;
}

void EffectObject::setTextureFile(const std::string& textureFile) {
    _textureFile = textureFile;
}

void EffectObject::playAnimation(const std::string& namePrefix, int frameCount, float delay) {
    if (!_spriteBatchNode) {
        _spriteBatchNode = SpriteBatchNode::create(_textureFile);
        this->addChild(_spriteBatchNode);
    }

    if (!_effectSprite) {
        _effectSprite = Sprite::createWithSpriteFrameName(namePrefix + "1.png");
        _effectSprite->setScale(SpriteController::updateSpriteScale(_effectSprite, 0.07f));
        _spriteBatchNode->addChild(_effectSprite);
    }
    else if (_effectSprite->getParent() == nullptr) {
        _spriteBatchNode->addChild(_effectSprite);
    }


    auto animation = createAnimation(namePrefix, frameCount, delay);
    if (animation) {
        auto animate = cocos2d::Animate::create(animation);
        _effectSprite->runAction(cocos2d::Sequence::create(
            animate,
            cocos2d::CallFunc::create([this]() {
                this->removeFromParentAndCleanup(false);
                EffectObjectPool::getInstance()->returnEffectObject(this);
                }),
            nullptr
        ));
    }
}

void EffectObject::stopAnimation() {
    _effectSprite->stopAllActions();
    SpriteFrameCacheManager::getInstance().unloadSpriteFrames(_plistFile);
}

void EffectObject::reset() {
    this->setVisible(false);
}
