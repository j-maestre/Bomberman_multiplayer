#include "gameManager.h"
#include "scenario.h"
#include <stdio.h>
#include "oxml/Vec4.h"
#include "player.h"

GameManager* GameManager::instance_ = nullptr;

GameManager::GameManager(){
    currentScenario_ = 0;
    total_players_ = 1;
    is_server_ = false;
    InitScenarios();
    //scenarios_[0] = new Scenario(); 
    printf("Constructor GameManager\n");
}

GameManager::~GameManager(){
  
}


void GameManager::InitScenario0(){

    //10 filas y 15 coumnas
    
    for (int i = 0; i < 18; i++){
        for (int j = 0; j < 31; j++){
            scenarios_[0].casillas_[i][j].color = oxml::Vec4(200.0f,200.0f,200.0f,255.0f);

        }
        
    }
    
    printf("*** Scenario 0 iniciado correctamente ***\n");

}
void GameManager::InitScenarios(){
    printf("Iniciando Scenarios...\n");
    InitScenario0();
    printf("Todos los scenarios inciados correctamente\n");
}

void GameManager::DrawScenario(){
    scenarios_[currentScenario_].Render();

}

void GameManager::DrawPlayer(){
    for (int i = 0; i < total_players_; i++){
        players_[i].DrawPlayer();
    }
    
    
}

void GameManager::PlantBomb(float x, float y){
    printf("Bomba en %f:%f -> %d:%d\n",x,y,(int) (x/45.0f),(int) (y/45.0f));
    scenarios_[0].casillas_[(int) (y/45.0f)][(int) (x/45.0f)].color = oxml::Vec4(255,0,0,255);
}