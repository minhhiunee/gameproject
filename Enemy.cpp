#include "Enemy.h"
#include <SDL.h>

const int ENEMY_SIZE = 50;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

Enemy::Enemy(int startX, int startY) {
    x = startX;
    y = startY;
    active = false;
}

void Enemy::update() {
    x -= 5;
    if (x < -ENEMY_SIZE) {
        x = WINDOW_WIDTH;
        y = rand() % (WINDOW_HEIGHT - ENEMY_SIZE);
    }
}

void Enemy::reset() {
    x = WINDOW_WIDTH;
    y = rand() % (WINDOW_HEIGHT - ENEMY_SIZE);
}

SDL_Rect Enemy::getRect() {
    return {x, y, ENEMY_SIZE, ENEMY_SIZE};
}
