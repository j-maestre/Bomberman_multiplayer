/*
    Author: Xema Maestre
    contact: <maestrequi@esat-alumni.com>
*/

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__ 1

#include "scenario.h"

class GameManager{
    public:
    ~GameManager();

    // Singleton
    static GameManager& Instance(){
        if(GameManager::instance_ == nullptr){
            GameManager::instance_ = new GameManager();
        }
        return *GameManager::instance_;
    }
    void DrawScenario();
    
    private:
    GameManager();
    GameManager(const GameManager&){};
    void InitScenarios();
    void InitScenario0();

    static GameManager* instance_;
    int currentScenario_;
    Scenario scenarios_[1];

};

#endif
