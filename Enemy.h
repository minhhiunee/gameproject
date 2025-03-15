#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

class Enemy {
public:
    int x, y;
    bool active;

    Enemy(int startX, int startY);
    void update();
    void reset();
    SDL_Rect getRect();
};

#endif
