

#ifndef _PLAYER_H__
#define _PLAYER_H__ 1

#include <esat/sprite.h>

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
    private:

    esat::SpriteTransform transform_;
    void MovePlayer();


};


#endif