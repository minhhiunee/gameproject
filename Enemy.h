#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <vector>
#include "Bullet.h"
using namespace std;
class Enemy {
public:
    int x, y;
    bool active;
    vector<SDL_Texture*> textures;
    SDL_Texture* currentTexture;
    Uint32 lastShotTime; // Thời gian bắn đạn cuối cùng
    static const Uint32 shootCooldown = 1500; // Cooldown 1.5 giây

    // Thuộc tính cho vụ nổ
    SDL_Texture* explosionTexture;
    vector<SDL_Texture*> explosionTextures; // Lưu trữ các texture vụ nổ
    SDL_Texture* currentExplosionTexture; // Texture vụ nổ hiện tại
    bool showExplosion;
    int explosionX, explosionY;
    Uint32 explosionStartTime;
    static const Uint32 EXPLOSION_DURATION = 500; // Hiển thị vụ nổ trong 500ms

    Enemy(int startX, int startY, SDL_Renderer* renderer);
    ~Enemy();

    void update();
    void reset();
    void triggerExplosion(); // Khai báo hàm
    void updateExplosion();  // Khai báo hàm
    void renderExplosion(SDL_Renderer* renderer);
    SDL_Rect getRect();
    void shoot(vector<Bullet>& bullets, SDL_Renderer* renderer); // Hàm bắn đạn
};

#endif
