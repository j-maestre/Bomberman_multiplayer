#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>

#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>
#include <esat_extra/imgui.h>
#include <esat_extra/sqlite3.h>

//#include "data_base_controller.h"
#include "cliente.h"
#include "gameManager.h"


int esat::main(int argc, char **argv) {
  srand(NULL);
  unsigned int fps=120;
  double current_time,last_time;
  esat::WindowInit(1400, 810);
  esat::DrawSetTextFont("../data/test.ttf");
  esat::DrawSetTextSize(25);
  esat::WindowSetMouseVisibility(true);
  esat::DrawSetFillColor(0, 255, 255, 255);
  esat::DrawSetStrokeColor(0, 255, 255);

  //DataBaseController& db_controller = DataBaseController::Instance();
  GameManager& gameManger = GameManager::Instance();
  static Cliente& cli = Cliente::Instance();
  cli.Conect();

  //Cliente::Instance().Conect();

  //db_controller.OpenDB("../data/chinook2.db");
  
  
  while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
    last_time = esat::Time();
    esat::DrawBegin();
    esat::DrawClear(0, 0, 0);

    gameManger.DrawScenario();
    gameManger.DrawPlayer();
    gameManger.MovePlayer();
    gameManger.RecivePlayer();



    do{//Control fps
    		current_time = esat::Time();
    }while((current_time-last_time)<=1000.0/fps);
    
    esat::DrawEnd();
    esat::WindowFrame();
  }
  esat::WindowDestroy();
  return 0;
}
