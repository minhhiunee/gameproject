#include "Enemy.h"
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
using namespace std;
const int ENEMY_SIZE = 150;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

Enemy::Enemy(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    active = false;
    lastShotTime = 0;
    showExplosion = false;
    explosionStartTime = 0;
    currentExplosionTexture = nullptr;

    textures.push_back(IMG_LoadTexture(renderer, "enemy_1.png"));
    textures.push_back(IMG_LoadTexture(renderer, "enemy_2.png"));
    textures.push_back(IMG_LoadTexture(renderer, "enemy_3.png"));

    for (size_t i = 0; i < textures.size(); ++i) {
        if (!textures[i]) {
            SDL_Log("Không thể tải texture enemy_%zu.png: %s", i + 1, IMG_GetError());
        }
    }

    // Tải ba texture vụ nổ
    explosionTextures.push_back(IMG_LoadTexture(renderer, "boom_1.png"));
    explosionTextures.push_back(IMG_LoadTexture(renderer, "boom_2.png"));
    explosionTextures.push_back(IMG_LoadTexture(renderer, "boom_3.png"));

    // Kiểm tra và xóa các texture không tải được
    for (size_t i = 0; i < explosionTextures.size();) {
        if (!explosionTextures[i]) {
            explosionTextures.erase(explosionTextures.begin() + i);
        } else {
            i++;
        }
    }
    currentTexture = textures[rand() % 3];
}

Enemy::~Enemy() {
    for (SDL_Texture* texture : textures) {
        if (texture) SDL_DestroyTexture(texture);
    }
    for (SDL_Texture* texture : explosionTextures) {
        if (texture) SDL_DestroyTexture(texture);
    }
}

void Enemy::update() {
    x -= 5;
    if (x < -ENEMY_SIZE) {
        x = WINDOW_WIDTH;
        y = rand() % (WINDOW_HEIGHT - ENEMY_SIZE);
        currentTexture = textures[rand() % 3];
    }
    updateExplosion();
}

void Enemy::reset() {
    x = WINDOW_WIDTH;
    y = rand() % (WINDOW_HEIGHT - ENEMY_SIZE);
    currentTexture = textures[rand() % 3];
}

void Enemy::triggerExplosion() {
    if (!showExplosion) {
        showExplosion = true;
        explosionX = x;
        explosionY = y;
        explosionStartTime = SDL_GetTicks();
        // Chỉ chọn texture nếu vector không rỗng
        if (!explosionTextures.empty()) {
            currentExplosionTexture = explosionTextures[rand() % explosionTextures.size()];
        } else {
            currentExplosionTexture = nullptr;
            SDL_Log("Không có texture vụ nổ để hiển thị!");
        }
    }
}

void Enemy::updateExplosion() {
    if (showExplosion && (SDL_GetTicks() - explosionStartTime >= EXPLOSION_DURATION)) {
        showExplosion = false;
        currentExplosionTexture = nullptr;
    }
}

void Enemy::renderExplosion(SDL_Renderer* renderer) {
    if (showExplosion && currentExplosionTexture) {
        SDL_Rect rect = {explosionX, explosionY, 100, 100};
        SDL_RenderCopy(renderer, currentExplosionTexture, NULL, &rect);
    }
}

SDL_Rect Enemy::getRect() {
    return {x, y, ENEMY_SIZE, ENEMY_SIZE};
}

void Enemy::shoot(vector<Bullet>& bullets, SDL_Renderer* renderer) {
    Uint32 currentTime = SDL_GetTicks();
    if (active && currentTime - lastShotTime >= shootCooldown) {
        if (rand() % 2 == 0) {
            Bullet bullet(renderer, true);
            bullet.shoot(x - ENEMY_SIZE + 130, y + ENEMY_SIZE/2 - 17);
            bullets.push_back(bullet);
            lastShotTime = currentTime;
        }
    }
}
