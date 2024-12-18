#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "PlayerAttributes/PlayerAttributes.h"
#include "ui/CocosGUI.h"
#include "Game2/Cursor/Cursor.h"

class GameController {
public:
    // Singleton instance
    static GameController* getInstance();

    // Game state management
    void GameOver(const std::function<void()>& exitAction, const std::function<cocos2d::Scene* ()>& createSceneFunc, const std::string& soundtrackPath);
    void Victory(const std::function<void()>& exitAction, const std::function<cocos2d::Scene* ()>& createSceneFunc, const std::string& soundtrackPath);
    bool isGameOver() const;
    void pauseGame(const std::function<void()>& exitAction, const std::function<cocos2d::Scene* ()>& createSceneFunc, const std::string& soundtrackPath);
    void resumeGame();
    bool isPaused() const;
    void replayGame(const std::string& soundtrackPath);
    void init();
    void toggleCursorVisibility(bool visible);

    // Music control
    void pauseMusic();
    void resumeMusic();

    // Public attributes
    bool musicStarted;
    int audioID;

private:
    // Private constructor for singleton pattern
    GameController();
    static GameController* instance;

    // Private methods
    void resetGameState();
    void showEndGamePanel(cocos2d::Layer* panel, const std::function<void()>& retryAction, const std::string& soundtrackPath);
    void showPausePanel(cocos2d::Layer* panel, const std::function<void()>& retryAction, const std::string& soundtrackPath);

    // Private attributes
    float gameTime;
    bool gameOver;
    bool paused;
    std::string currentSoundtrackPath;
    Cursor* _cursor;
};

#endif // __GAME_CONTROLLER_H__
