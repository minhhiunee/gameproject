#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
#include <vector>
using namespace std;

class Object {
private:
    int x, y;
    int speed; // Tốc độ di chuyển của vật cản
    bool active;
    static const int OBJECT_SIZE = 130;
    vector<SDL_Texture*> textures;
    SDL_Texture* currentTexture;

public:
    Object(int windowWidth, int windowHeight, SDL_Renderer* renderer);
    ~Object();
    void update();
    int getX() const { return x; }
    int getY() const { return y; }
    bool isActive() const { return active; }
    static int getSize() { return OBJECT_SIZE; }
    void reset(int windowWidth);
    SDL_Texture* getTexture() const { return currentTexture; }
    SDL_Rect getRect() const;
    void setSpeed(int newSpeed);
    int getSpeed() const { return speed; }
};

#endif
