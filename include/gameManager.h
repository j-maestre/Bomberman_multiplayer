/*
    Author: Xema Maestre
    contact: <maestrequi@esat-alumni.com>
*/

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__ 1

#include "scenario.h"
#include "player.h"
#include "paquete.h"
#include <winsock.h>

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
    void DrawPlayer();
    void PlantBomb(float x, float y);
    bool MovePlayer();
    Movimiento Move(int id, Movimiento mov);
    void AddPlayer(int id);
    void RecivePlayer();
    bool is_server_;
    
    private:
    GameManager();
    GameManager(const GameManager&){};
    static GameManager* instance_;
    
    void InitScenarios();
    void InitScenario0();

    const int total_scenarios_ = 1;
    int currentScenario_;
    Scenario scenarios_[1];


    int total_players_;
    static const int max_players_ = 4;
    Player players_[max_players_];

    /*WSADATA wsa_;
    SOCKET sock_;
    sockaddr_in ip_,ips_[4];

    int ip_size_;
    timeval time_;
    fd_set readfs_;*/

};

#endif
