#include "scenario.h"
#include <esat/sprite.h>
#include <esat/draw.h>
#include <stdio.h>

Scenario::Scenario(){
    printf("Constructor Scenario\n");
    prueba.width = 30.0f;
    prueba.height = 30.0f;
    prueba.pos = 0;

}

Scenario::Scenario(const Scenario& s){

}

Scenario::~Scenario(){

}

void Scenario::Render(){
    float points[8];
    points[0] = 0.0f;
    points[1] = 0.0f;
    points[2] = prueba.width;
    points[3] = 0.0f;
    points[4] = prueba.width;
    points[5] = prueba.height;
    points[6] = 0.0f;
    points[7] = prueba.width;
    esat::DrawSolidPath(points,4);

}