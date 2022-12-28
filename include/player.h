

#ifndef _PLAYER_H__
#define _PLAYER_H__ 1

#include <esat/sprite.h>
#include "cliente.h"

class Player{

    public:

    Player();
    Player(const Player&);
    ~Player();

    int positionX_;
    int positionY_;
    int score_;
    int bombs_;
    esat::SpriteHandle sprite_;
    void DrawPlayer();

    float speed_;

    //Cliente& cli_;
    //static const Cliente& cli_ = Cliente::Instance();
    
    private:

    esat::SpriteTransform transform_;
    void MovePlayer();


};


#endif