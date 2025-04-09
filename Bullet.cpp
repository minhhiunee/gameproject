#include "Bullet.h"
#include <SDL.h>
#include <SDL_image.h>

const int BULLET_SIZE = 35;

Bullet::Bullet(SDL_Renderer* renderer) : x(0), y(0), active(false) {
    texture = IMG_LoadTexture(renderer, "bullet.png");
    if (!texture) {
        SDL_Log("Không thể tải texture cho Bullet: %s", IMG_GetError());
    }
}

Bullet::Bullet(const Bullet& other) : x(other.x), y(other.y), active(other.active) {
    // Sao chép texture mà không hủy texture gốc
    texture = other.texture ? IMG_LoadTexture(SDL_GetRenderer(SDL_GetWindowFromID(1)), "bullet.png") : nullptr;
    if (!texture && other.texture) {
        SDL_Log("Không thể sao chép texture cho Bullet: %s", IMG_GetError());
    }
}

Bullet& Bullet::operator=(const Bullet& other) {
    if (this != &other) {
        if (texture) SDL_DestroyTexture(texture); // Hủy texture cũ
        x = other.x;
        y = other.y;
        active = other.active;
        texture = other.texture ? IMG_LoadTexture(SDL_GetRenderer(SDL_GetWindowFromID(1)), "bullet.png") : nullptr;
        if (!texture && other.texture) {
            SDL_Log("Không thể gán texture cho Bullet: %s", IMG_GetError());
        }
    }
    return *this;
}

Bullet::~Bullet() {
    if (texture) SDL_DestroyTexture(texture);
}

void Bullet::update() {
    if (active) {
        x += 10;
    }
}

void Bullet::shoot(int startX, int startY) {
    x = startX;
    y = startY;
    active = true;
}

SDL_Rect Bullet::getRect() const {
    return {x, y, BULLET_SIZE, BULLET_SIZE};
}
