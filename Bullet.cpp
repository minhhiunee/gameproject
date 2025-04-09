#ifndef BULLET_H
#define BULLET_H
#include <SDL.h>

class Bullet {
private:
    int x, y;
    bool active;
    SDL_Texture* texture;

public:
    Bullet(SDL_Renderer* renderer);
    Bullet(const Bullet& other); // Copy constructor
    Bullet& operator=(const Bullet& other); // Assignment operator
    ~Bullet();
    void update();
    void shoot(int startX, int startY);
    bool isActive() const { return active; }
    int getX() const { return x; }
    int getY() const { return y; }
    void deactivate() { active = false; }
    SDL_Texture* getTexture() const { return texture; }
    SDL_Rect getRect() const;
};

#endif
