#include "scenario.h"
#include <esat/sprite.h>
#include <esat/draw.h>
#include <stdio.h>
#include "../include/oxml/Vec4.h"

Scenario::Scenario(){
    printf("Constructor Scenario\n");
    width_ = 45.0f;
    height_ = 45.0f;

}

Scenario::Scenario(const Scenario& s){

}

Scenario::~Scenario(){

}

void Scenario::Render(){
    
    for (int i = 0; i < filas_; i++){
        for (int j = 0; j < columnas_; j++){
            float points[8];

            points[0] = j*width_;
            points[1] = i*height_;

            points[2] = (j*width_)+width_;
            points[3] = i*height_;

            points[4] = (j*width_)+width_;
            points[5] = (i*height_)+height_;

            points[6] = j*width_;
            points[7] = (i*height_)+height_;

            esat::DrawSetStrokeColor(255, 0, 0,255);
            //esat::DrawSetStrokeColor((unsigned char) casillas_[i][j].color.x, (unsigned char) casillas_[i][j].color.y, (unsigned char) casillas_[i][j].color.z, (unsigned char) casillas_[i][j].color.w);
            esat::DrawSetFillColor((unsigned char) casillas_[i][j].color.x, (unsigned char) casillas_[i][j].color.y, (unsigned char) casillas_[i][j].color.z, (unsigned char) casillas_[i][j].color.w);
            esat::DrawSolidPath(points, 4);
        }
        
    }
    

}