#include "Bullet.h"
#include <SDL.h>
#include <SDL_image.h>

const int BULLET_SIZE = 35;

Bullet::Bullet(SDL_Renderer* renderer, bool isEnemy) : x(0), y(0), active(false), isEnemyBullet(isEnemy) {
    // Tải texture khác nhau cho đạn của Player và Enemy
    texture = IMG_LoadTexture(renderer, isEnemy ? "bullet_enemy.png" : "bullet_player.png");
    if (!texture) {
        SDL_Log("Không thể tải texture cho Bullet: %s", IMG_GetError());
    }
}

Bullet::Bullet(const Bullet& other) : x(other.x), y(other.y), active(other.active), isEnemyBullet(other.isEnemyBullet) {
    texture = other.texture ? IMG_LoadTexture(SDL_GetRenderer(SDL_GetWindowFromID(1)), other.isEnemyBullet ? "bullet_enemy.png" : "bullet_player.png") : nullptr;
    if (!texture && other.texture) {
        SDL_Log("Không thể sao chép texture cho Bullet: %s", IMG_GetError());
    }
}

Bullet& Bullet::operator=(const Bullet& other) {
    if (this != &other) {
        if (texture) SDL_DestroyTexture(texture);
        x = other.x;
        y = other.y;
        active = other.active;
        isEnemyBullet = other.isEnemyBullet;
        texture = other.texture ? IMG_LoadTexture(SDL_GetRenderer(SDL_GetWindowFromID(1)), other.isEnemyBullet ? "bullet_enemy.png" : "bullet_player.png") : nullptr;
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
        x += isEnemyBullet ? -10 : 10;
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
