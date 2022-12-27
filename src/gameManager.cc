#include "gameManager.h"
#include "scenario.h"
#include <stdio.h>
GameManager* GameManager::instance_ = nullptr;

GameManager::GameManager(){
    currentScenario_ = 0;
    //scenarios_[0] = new Scenario(); 
    printf("Constructor GameManager\n");
}

GameManager::~GameManager(){
  
}


void GameManager::InitScenario0(){


}
void GameManager::InitScenarios(){
    InitScenario0();
}

void GameManager::DrawScenario(){
    scenarios_[currentScenario_].Render();

}