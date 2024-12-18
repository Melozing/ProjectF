// PlayerMovement.h
#ifndef __PLAYERMOVEMENT_H__
#define __PLAYERMOVEMENT_H__

#include "cocos2d.h"
#include <limits>

class PlayerMovement
{
public:
    PlayerMovement(cocos2d::Node* player, float speed,
        float minX = std::numeric_limits<float>::lowest(),
        float maxX = std::numeric_limits<float>::max(),
        float minY = std::numeric_limits<float>::lowest(),
        float maxY = std::numeric_limits<float>::max());
    ~PlayerMovement();

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode);
    void update(float delta);
    float getSpeed() const;
    void setEnabled(bool enabled);

private:
    void updateDirection();
    bool _enabled = true;
    cocos2d::Node* _player;
    float _speed;
    cocos2d::Vec2 _direction;
    std::map<cocos2d::EventKeyboard::KeyCode, bool> _keys;

    float _minX, _maxX, _minY, _maxY;
};

#endif // __PLAYERMOVEMENT_H__
