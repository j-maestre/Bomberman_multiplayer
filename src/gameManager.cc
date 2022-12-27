#include "gameManager.h"
#include "scenario.h"
#include <stdio.h>
#include "oxml/Vec4.h"

GameManager* GameManager::instance_ = nullptr;

GameManager::GameManager(){
    currentScenario_ = 0;
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