#include "player.h"
#include <esat/input.h>
#include <stdio.h>
#include "gameManager.h"
#include "cliente.h"

Player::Player(){
    positionX_ = 0;
    positionY_ = 0;
    score_ = 0;
    bombs_ = 3;
    sprite_ = esat::SpriteFromFile("../data/sprite_sheet_ampliado.png");
    sprite_ = esat::SubSprite(sprite_, 4, 137, 73, 111);
    esat::SpriteTransformInit(&transform_);
    transform_.scale_x = 0.6f;
    transform_.scale_y = 0.4f;
    speed_ = 1.1f;
    //cli_ = Cliente::Instance();
}

Player::Player(const Player& p){

}
Player::~Player(){
    esat::SpriteRelease(sprite_);
}

void Player::MovePlayer(){

    bool has_moved = false;

    float x;
    float y;

    if(esat::IsKeyPressed('W')){
        y = transform_.y - speed_;
        has_moved = true;
    }
    if(esat::IsKeyPressed('S')){
        y = transform_.y + speed_;
        has_moved = true;
    }
    if(esat::IsKeyPressed('A')){
        x = transform_.x - speed_;
        has_moved = true;
    }
    if(esat::IsKeyPressed('D')){
        x = transform_.x + speed_;
        has_moved = true;
    }

    if(esat::IsSpecialKeyDown(esat::kSpecialKey_Space)){
        printf("Bomba");

        GameManager::Instance().PlantBomb(transform_.x,transform_.y);
        has_moved = true;
    }

    if(has_moved){
        //Si se ha movido, envio al servidor los datos de hacia donde me quiero mover y el sevidor me moverá y luego me devolvera mi nueva posicion
        //cli_.Move(x,y);
    }


    
}


void Player::DrawPlayer(){
    //esat::DrawSprite(sprite_,positionX_,positionY_);
    MovePlayer();
    
   
    esat::DrawSprite(sprite_,transform_);
}