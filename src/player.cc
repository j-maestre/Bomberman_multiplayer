#include "player.h"
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include "gameManager.h"
#include "cliente.h"
#include "paquete.h"

//Cliente& client = Cliente::Instance();
static Cliente& cli = Cliente::Instance();

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
    transform_.x = 0.0f;
    transform_.y = 0.0f;
    speed_ = 1.1f;
    active_ = false;
    id_ = -1;
    //cli_ = Cliente::Instance();
}

Player::Player(const Player& p){

}
Player::~Player(){
    esat::SpriteRelease(sprite_);
}

bool Player::MovePlayer(){

    bool has_moved = false;
    bool bomb = false;

    float x = 0.0f;
    float y = 0.0f;

    if(esat::IsKeyPressed('W')){
        //y = transform_.y - speed_;
        y = -speed_;
        has_moved = true;
    }
    if(esat::IsKeyPressed('S')){
        //y = transform_.y + speed_;
        y = speed_;
        has_moved = true;
    }
    if(esat::IsKeyPressed('A')){
        //x = transform_.x - speed_;
        x = -speed_;
        has_moved = true;
    }
    if(esat::IsKeyPressed('D')){
        //x = transform_.x + speed_;
        x = speed_;
        has_moved = true;
    }

    if(esat::IsSpecialKeyDown(esat::kSpecialKey_Space)){
        printf("Bomba");

        bomb = true;
        has_moved = true;
    }

    if(has_moved){
        printf("c a movio\n");
        //Si se ha movido, envio al servidor los datos de hacia donde me quiero mover y el sevidor me moverá y luego me devolvera mi nueva posicion
        //cli_.Move(x,y);
        //client.Move(x,y);
        Movimiento new_pos = cli.Move(x,y,bomb);
        transform_.x = new_pos.x;
        transform_.y = new_pos.y;
        if(bomb){
            GameManager::Instance().PlantBomb(transform_.x,transform_.y);
        }
    }

    return has_moved;


    
}


void Player::DrawPlayer(char name[256]){
    //esat::DrawSprite(sprite_,positionX_,positionY_);
    //MovePlayer();
    
    //esat::DrawText((transform_.x + 10),transform_.y,name);
    //printf("*** %s ***\n",name);
    esat::DrawSprite(sprite_,transform_);
}