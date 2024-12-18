#ifndef __FALLING_TREE_H__
#define __FALLING_TREE_H__

#include "FallingObject.h"

class FallingTree : public FallingObject {
public:
    static FallingTree* create();
    virtual bool init() override;
    void initAnimation() override;
    void returnToPool();
    void createPhysicsBody();
private:
    cocos2d::SpriteBatchNode* spriteBatchNode;
};

#endif // __FALLING_TREE_H__
