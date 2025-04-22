#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>
#include "main.h"
using namespace std;

class Player {
public:
    int x, y;
    int initialY;
    int width;
    int height;
    int flyingWidth;
    int flyingHeight;
    int speed; // Tốc độ di chuyển trong FLYING
    int runningSpeed; // Tốc độ chạy trong RUNNING
    int score;
    int hp;
    bool isJumping;
    double jumpHeight;
    vector<SDL_Texture*> runTextures;
    SDL_Texture* textureFlying;
    int currentFrame;
    int frameDelay;
    int frameCounter;

    Player(int startX, int startY, SDL_Renderer* renderer);
    ~Player();
    void updateJump();
    void movePlane(const Uint8* keystates) {
        if (keystates[SDL_SCANCODE_UP]) this->y -= this->speed;
        if (keystates[SDL_SCANCODE_DOWN]) this->y += this->speed;
        if (this->y < 0) this->y = 0;
        if (this->y > WINDOW_HEIGHT - flyingHeight) this->y = WINDOW_HEIGHT - flyingHeight;
    }
    void updateAnimation();
    SDL_Rect getRect(GameState state);
    SDL_Texture* getCurrentTexture(GameState state);
    void takeDamage(int damage);
    bool isAlive() const;
    void setRunningSpeed(int newSpeed);
    int getRunningSpeed() const { return runningSpeed; }
    void reset(int startX, int startY); // Hàm mới để đặt lại trạng thái
};

#endif
