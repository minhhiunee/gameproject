#include "Player.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "main.h"
using namespace std;

Player::Player(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    initialY = startY;
    width = 80;
    height = 150;
    flyingWidth = 150;
    flyingHeight = 150;
    speed = 8; // Tốc độ cho FLYING
    runningSpeed = 4; // Tốc độ ban đầu cho RUNNING
    score = 0;
    hp = 100;
    isJumping = false;
    jumpHeight = 0;
    currentFrame = 0;
    frameDelay = 5;
    frameCounter = 0;

    for (int i = 1; i <= 8; i++) {
        string path = "playerun_" + to_string(i) + ".png";
        SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
        if (!texture) {
            SDL_Log("Không thể tải texture player_run%d.png: %s", i, IMG_GetError());
        }
        runTextures.push_back(texture);
    }

    textureFlying = IMG_LoadTexture(renderer, "playerfly_1.png");
    if (!textureFlying) {
        SDL_Log("Không thể tải textureFlying cho Player: %s", IMG_GetError());
    }
}

Player::~Player() {
    for (SDL_Texture* texture : runTextures) {
        if (texture) SDL_DestroyTexture(texture);
    }
    if (textureFlying) SDL_DestroyTexture(textureFlying);
}

void Player::updateJump() {
    if (isJumping) {
        y -= jumpHeight;
        jumpHeight -= 0.6;
        if (y >= initialY) {
            y = initialY;
            isJumping = false;
        }
    }
}

void Player::updateAnimation() {
    // Điều chỉnh frameDelay dựa trên runningSpeed để animation nhanh hơn
    int adjustedFrameDelay = max(2, 6 - (runningSpeed - 4) / 2); // frameDelay giảm khi runningSpeed tăng
    frameCounter++;
    if (frameCounter >= adjustedFrameDelay) {
        frameCounter = 0;
        currentFrame = (currentFrame + 1) % 8;
    }
}

SDL_Rect Player::getRect(GameState state) {
    if (state == RUNNING) {
        return {x, y, width, height};
    }
    return {x, y, flyingWidth, flyingHeight};
}

SDL_Texture* Player::getCurrentTexture(GameState state) {
    if (state == RUNNING) {
        return runTextures[currentFrame];
    }
    return textureFlying;
}

void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

bool Player::isAlive() const {
    return hp > 0;
}

void Player::setRunningSpeed(int newSpeed) {
    runningSpeed = newSpeed;
}

void Player::reset(int startX, int startY) {
    x = startX;
    y = startY;
    initialY = startY;
    score = 0;
    hp = 100;
    isJumping = false;
    jumpHeight = 0;
    currentFrame = 0;
    frameDelay = 5; // Đặt lại frameDelay
    frameCounter = 0;
    runningSpeed = 4; // Đặt lại runningSpeed
}
