#include "Game2/Items/ItemsSpawn.h"

USING_NS_CC;

ItemsSpawn* ItemsSpawn::create(ItemType type, const Vec2& position) {
    ItemsSpawn* item = new (std::nothrow) ItemsSpawn();
    if (item && item->init(type, position)) {
        item->autorelease();
		item->setName("ItemsSpawn");
        item->setScale(0.05f);
        return item;
    }
    delete item;
    return nullptr;
}

bool ItemsSpawn::init(ItemType type, const Vec2& position) {
    if (!Sprite::init()) {
        return false;
    }

    _type = type;
    _initialPosition = position;
    _time = 0.0f;

    std::string spritePath;
    switch (type) {
    case ItemType::AMMO:
        spritePath = "assets_game/items/hop_dan2.png";
        break;
    case ItemType::HEALTH:
        spritePath = "assets_game/items/medkit.png";
        break;
    case ItemType::GRENADE:
        spritePath = "assets_game/items/grenade.png";
        break;
    }

    this->setTexture(spritePath);
    this->setPosition(position);
    this->createPhysicsBody();
    this->scheduleUpdate();

    return true;
}

void ItemsSpawn::createPhysicsBody() {
    auto body = PhysicsBody::createCircle(this->getContentSize().width / 2);
    body->setDynamic(false);
    body->setCategoryBitmask(0x04);
    body->setCollisionBitmask(0x08);
    body->setContactTestBitmask(0x08);
    this->addComponent(body);
}

void ItemsSpawn::update(float delta) {
    _time += delta;
    float height = 10.0f * std::sin(_time * 2.0f);
    this->setPositionY(_initialPosition.y + height);
    this->setRotation(this->getRotation() + 90.0f * delta);

    if (_time >= 3.0f && _time < 6.0f) {
        startBlinking();
    }
    else if (_time >= 6.0f) {
        this->removeFromParent();
    }
}

void ItemsSpawn::startBlinking() {
    if (!this->getActionByTag(1)) {
        auto blinkAction = Blink::create(3.0f, 6);
        blinkAction->setTag(1);
        this->runAction(blinkAction);
    }
}

ItemsSpawn::ItemType ItemsSpawn::getType() const {
    return _type;
}
