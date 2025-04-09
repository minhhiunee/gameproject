#include "Enemy.h"
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib> // Để dùng rand()

const int ENEMY_SIZE = 150;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

Enemy::Enemy(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    active = false;

    // Tải 3 texture cho Enemy
    textures.push_back(IMG_LoadTexture(renderer, "enemy_1.png"));
    textures.push_back(IMG_LoadTexture(renderer, "enemy_2.png"));
    textures.push_back(IMG_LoadTexture(renderer, "enemy_3.png"));

    // Kiểm tra lỗi khi tải texture
    for (size_t i = 0; i < textures.size(); ++i) {
        if (!textures[i]) {
            SDL_Log("Không thể tải texture enemy_%zu.png: %s", i + 1, IMG_GetError());
        }
    }

    // Chọn ngẫu nhiên texture ban đầu
    currentTexture = textures[rand() % 3];
}

Enemy::~Enemy() {
    // Giải phóng tất cả texture trong vector
    for (SDL_Texture* texture : textures) {
        if (texture) SDL_DestroyTexture(texture);
    }
}

void Enemy::update() {
    x -= 5;
    if (x < -ENEMY_SIZE) {
        x = WINDOW_WIDTH;
        y = rand() % (WINDOW_HEIGHT - ENEMY_SIZE);
        currentTexture = textures[rand() % 3]; // Chọn ngẫu nhiên texture khi Enemy ra ngoài màn hình
    }
}

void Enemy::reset() {
    x = WINDOW_WIDTH;
    y = rand() % (WINDOW_HEIGHT - ENEMY_SIZE);
    currentTexture = textures[rand() % 3]; // Chọn ngẫu nhiên texture khi reset
}

SDL_Rect Enemy::getRect() {
    return {x, y, ENEMY_SIZE, ENEMY_SIZE};
}
