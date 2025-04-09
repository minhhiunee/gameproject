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
    int width;           // Kích thước chung (dùng cho RUNNING)
    int height;
    int flyingWidth;     // Kích thước riêng cho FLYING
    int flyingHeight;
    int speed;
    int score;
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
    void movePlane(const Uint8* keystates);
    void updateAnimation();
    SDL_Rect getRect(GameState state); 
    SDL_Texture* getCurrentTexture(GameState state);
};

#endif
