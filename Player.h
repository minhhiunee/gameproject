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
    int width; //Chiều rộng
    int height; //Chiều dài
    int flyingWidth;
    int flyingHeight;
    int speed; // Tốc độ mặc định trong FLYING
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
    void movePlane(const Uint8* keystates, float speed);
    void updateAnimation();
    SDL_Rect getRect(GameState state);
    SDL_Texture* getCurrentTexture(GameState state);
    void takeDamage(int damage);
    bool isAlive() const;
    void setRunningSpeed(int newSpeed);
    int getRunningSpeed() const { return runningSpeed; }
    void reset(int startX, int startY);
};

#endif
