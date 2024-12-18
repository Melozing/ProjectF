// PlayerMovement.cpp
#include "Manager/PlayerMovementManager.h"

USING_NS_CC;

PlayerMovement::PlayerMovement(Node* player, float speed, float minX, float maxX, float minY, float maxY)
    : _player(player), _speed(speed), _direction(Vec2::ZERO), _minX(minX), _maxX(maxX), _minY(minY), _maxY(maxY)
{
}

PlayerMovement::~PlayerMovement()
{
}

void PlayerMovement::onKeyPressed(EventKeyboard::KeyCode keyCode)
{
    _keys[keyCode] = true;
    updateDirection();
}

void PlayerMovement::onKeyReleased(EventKeyboard::KeyCode keyCode)
{
    _keys[keyCode] = false;
    updateDirection();
}

void PlayerMovement::update(float delta)
{
    if (!_enabled) return;
    if (_direction != Vec2::ZERO)
    {
        Vec2 newPosition = _player->getPosition() + _direction * _speed * delta;

        newPosition.x = std::max(_minX, std::min(newPosition.x, _maxX));
        newPosition.y = std::max(_minY, std::min(newPosition.y, _maxY));

        _player->setPosition(newPosition);
    }
}

float PlayerMovement::getSpeed() const
{
    return _speed;
}

void PlayerMovement::updateDirection()
{
    _direction = Vec2::ZERO;

    if (_keys[EventKeyboard::KeyCode::KEY_W] || _keys[EventKeyboard::KeyCode::KEY_UP_ARROW])
    {
        _direction.y += 1;
    }
    if (_keys[EventKeyboard::KeyCode::KEY_S] || _keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW])
    {
        _direction.y -= 1;
    }
    if (_keys[EventKeyboard::KeyCode::KEY_A] || _keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW])
    {
        _direction.x -= 1;
    }
    if (_keys[EventKeyboard::KeyCode::KEY_D] || _keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW])
    {
        _direction.x += 1;
    }

    _direction.normalize();
}

void PlayerMovement::setEnabled(bool enabled) {
    _enabled = enabled;
}