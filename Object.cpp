#include "Object.h"
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
using namespace std;

Object::Object(int windowWidth, int windowHeight, SDL_Renderer* renderer)
    : x(windowWidth), y(windowHeight - 330), speed(5), active(true) {
    textures.push_back(IMG_LoadTexture(renderer, "tree 1.png"));
    textures.push_back(IMG_LoadTexture(renderer, "tree 2.png"));
    textures.push_back(IMG_LoadTexture(renderer, "stone 1.png"));
    textures.push_back(IMG_LoadTexture(renderer, "stone 2.png"));
    textures.push_back(IMG_LoadTexture(renderer, "banana skin.png"));

    for (size_t i = 0; i < textures.size(); ++i) {
        if (!textures[i]) {
            SDL_Log("Không thể tải texture object%zu.png: %s", i + 1, IMG_GetError());
        }
    }

    currentTexture = textures[rand() % 5];
}

Object::~Object() {
    for (SDL_Texture* texture : textures) {
        if (texture) SDL_DestroyTexture(texture);
    }
}

void Object::update() {
    x -= speed;
}

void Object::reset(int windowWidth) {
    x = windowWidth + (rand() % 200 ); // Thêm khoảng cách ngẫu nhiên
    currentTexture = textures[rand() % 5];
}

void Object::setSpeed(int newSpeed) {
    speed = newSpeed;
}
