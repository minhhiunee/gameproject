#include "Player.h"
#include <SDL.h>

const int PLAYER_SIZE = 50;
const int WINDOW_HEIGHT = 600;

Player::Player(int startX, int startY) {
    x = startX;
    y = startY;
    speed = 5;
    score = 0;
    isJumping = false;
    jumpHeight = 0;
}

void Player::updateJump() {
    if (isJumping) {
        y -= jumpHeight;
        jumpHeight -= 1;
        if (y >= WINDOW_HEIGHT - PLAYER_SIZE - 50) {
            y = WINDOW_HEIGHT - PLAYER_SIZE - 50;
            isJumping = false;
        }
    }
}

void Player::movePlane(const Uint8* keystates) {
    if (keystates[SDL_SCANCODE_UP]) y -= speed;
    if (keystates[SDL_SCANCODE_DOWN]) y += speed;
    if (y < 0) y = 0;
    if (y > WINDOW_HEIGHT - PLAYER_SIZE) y = WINDOW_HEIGHT - PLAYER_SIZE;
}

SDL_Rect Player::getRect() {
    return {x, y, PLAYER_SIZE, PLAYER_SIZE};
}
