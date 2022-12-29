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
#include "server.h"
#include "gameManager.h"


int esat::main(int argc, char **argv) {
  float points[8];

  points[0] = 20.0f;
  points[1] = 20.0f;
  
  points[2] = 200.0f;
  points[3] = 20.0f;

  points[4] = 200.0f;
  points[5] = 80.0f;
  
  points[6] = 20.0f;
  points[7] = 80.0f;

  srand(NULL);
  unsigned int fps=120;
  double current_time,last_time;
  esat::WindowInit(1400, 810);
  esat::DrawSetTextFont("../data/test.ttf");
  esat::DrawSetTextSize(25);
  esat::WindowSetMouseVisibility(true);
  esat::DrawSetFillColor(0, 255, 0, 255);
  esat::DrawSetStrokeColor(0, 255, 0);

  GameManager& gameManger = GameManager::Instance();
  Server& server = Server::Instance();
  gameManger.is_server_ = true;

  server.Deploy();
  
  
  while (!esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
    last_time = esat::Time();
    esat::DrawBegin();
    esat::DrawClear(0, 0, 0);
    server.Listen();

    gameManger.DrawScenario();
    gameManger.DrawPlayer();

    esat::DrawSetFillColor(0, 255, 0, 255);
    esat::DrawSetStrokeColor(0, 255, 0);
    esat::DrawSolidPath(points,4);




    do{//Control fps
    		current_time = esat::Time();
    }while((current_time-last_time)<=1000.0/fps);
    
    esat::DrawEnd();
    esat::WindowFrame();
  }
  esat::WindowDestroy();
  return 0;
}
