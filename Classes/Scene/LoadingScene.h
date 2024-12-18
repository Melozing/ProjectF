#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "LoadingBar/CustomLoadingBar.h"
#include "Game2/Cursor/Cursor.h"

USING_NS_CC;

class LoadingScene : public Scene {
public:
    static Scene* createScene(const std::string& sceneName);

    virtual bool init() override;

    CREATE_FUNC(LoadingScene);

    void startLoading();
    void setNextSceneName(const std::string& sceneName);

private:
    void initLoadingBar();
    void initBackground();
    void initCursor();
    void preloadAssets(); // Add this line

    Cursor* _cursor;
    ui::LoadingBar* loadingBar;
    CustomLoadingBar* customLoadingBar;
    Sprite* border;
    std::string nextSceneName; // Store the name of the next scene
};

#endif // __LOADING_SCENE_H__
