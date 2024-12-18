#ifndef __BASE_PANEL_H__
#define __BASE_PANEL_H__

#include "cocos2d.h"
#include "Game2/Cursor/Cursor.h"

class BasePanel : public cocos2d::Layer {
public:
    virtual bool init() override;
    void addTitleLabel(const std::string& titleText);

protected:
    Cursor* _cursor;
    cocos2d::Sprite* boardSprite;
};

#endif // __BASE_PANEL_H__
