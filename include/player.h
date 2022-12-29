

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
    bool active_;
    int id_;
    esat::SpriteHandle sprite_;
    void DrawPlayer(char name[256]);
    bool MovePlayer();
    void DrawOtherPlayers(float x, float y);

    float speed_;

    //Cliente& cli_;
    esat::SpriteTransform transform_;
    
    private:



};


#endif