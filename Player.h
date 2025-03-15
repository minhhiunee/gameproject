#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

class Player {
public:
    int x, y;
    int speed;
    int score;
    bool isJumping;
    int jumpHeight;

    Player(int startX, int startY);
    void updateJump();
    void movePlane(const Uint8* keystates);
    SDL_Rect getRect();
};

#endif
