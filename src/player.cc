#include "player.h"
#include <esat/input.h>
#include <stdio.h>
#include "gameManager.h"

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

    

}
Player::Player(const Player& p){

}
Player::~Player(){
    esat::SpriteRelease(sprite_);
}

void Player::MovePlayer(){

    if(esat::IsKeyPressed('W')){
        transform_.y -= speed_;
    }
    if(esat::IsKeyPressed('S')){
        transform_.y += speed_;
    }
    if(esat::IsKeyPressed('A')){
        transform_.x -= speed_;
    }
    if(esat::IsKeyPressed('D')){
        transform_.x += speed_;
    }

    if(esat::IsSpecialKeyDown(esat::kSpecialKey_Space)){
        printf("Bomba");

        GameManager::Instance().PlantBomb(transform_.x,transform_.y);
    }
    
}


void Player::DrawPlayer(){
    //esat::DrawSprite(sprite_,positionX_,positionY_);
    MovePlayer();
    
   
    esat::DrawSprite(sprite_,transform_);
}