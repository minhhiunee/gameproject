#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <vector> // Thêm vector để lưu danh sách texture

class Enemy {
public:
    int x, y;
    bool active;
    std::vector<SDL_Texture*> textures; // Danh sách các texture
    SDL_Texture* currentTexture;        // Texture hiện tại được chọn

    Enemy(int startX, int startY, SDL_Renderer* renderer);
    ~Enemy();
    void update();
    void reset();
    SDL_Rect getRect();
};

#endif
