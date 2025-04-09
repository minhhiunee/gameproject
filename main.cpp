#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Object.h"
#include "main.h"
using namespace std;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int BULLET_SIZE = 35;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL không thể khởi tạo! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) {
        cerr << "SDL_image không thể khởi tạo! IMG_Error: " << IMG_GetError() << endl;
        SDL_Quit();
        return -1;
    }
    SDL_Window* window = SDL_CreateWindow("Escape and Fly", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Không thể tạo cửa sổ! SDL_Error: " << SDL_GetError() << endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Không thể tạo renderer! SDL_Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Surface* bgSurfaceRunning = IMG_Load("backgroundrunning.png");
    if (!bgSurfaceRunning) {
        cerr << "Không thể tải background_running.png! IMG_Error: " << IMG_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Texture* background1Running = SDL_CreateTextureFromSurface(renderer, bgSurfaceRunning);
    SDL_Texture* background2Running = SDL_CreateTextureFromSurface(renderer, bgSurfaceRunning);

    SDL_Surface* bgSurfaceFlying = IMG_Load("backgroundflying.png");
    if (!bgSurfaceFlying) {
        cerr << "Không thể tải background_flying.png! IMG_Error: " << IMG_GetError() << endl;
        SDL_DestroyTexture(background1Running);
        SDL_DestroyTexture(background2Running);
        SDL_FreeSurface(bgSurfaceRunning);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Texture* background1Flying = SDL_CreateTextureFromSurface(renderer, bgSurfaceFlying);
    SDL_Texture* background2Flying = SDL_CreateTextureFromSurface(renderer, bgSurfaceFlying);

    SDL_FreeSurface(bgSurfaceRunning);
    SDL_FreeSurface(bgSurfaceFlying);

    if (!background1Running || !background2Running || !background1Flying || !background2Flying) {
        cerr << "Không thể tạo texture! SDL_Error: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    int bgWidth = WINDOW_WIDTH;
    int bg1X = 0;
    int bg2X = bgWidth;
    int bgSpeed = 3;

    Player player(100, WINDOW_HEIGHT - 350, renderer);
    Object object(WINDOW_WIDTH, WINDOW_HEIGHT, renderer);
    Enemy enemy(WINDOW_WIDTH, 100, renderer);
    vector<Bullet> bulletTops;
    vector<Bullet> bulletBottoms;
    GameState state = RUNNING;

    bool running = true;
    SDL_Event e;
    Uint32 lastShotTime = 0;
    const Uint32 shootCooldown = 500;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) {
                if (state == RUNNING && e.key.keysym.sym == SDLK_SPACE && !player.isJumping) {
                    player.isJumping = true;
                    player.jumpHeight = 13;
                }
                if (state == FLYING && e.key.keysym.sym == SDLK_SPACE) {
                    Uint32 currentTime = SDL_GetTicks();
                    if (currentTime - lastShotTime >= shootCooldown) {
                        Bullet topBullet(renderer);
                        Bullet bottomBullet(renderer);
                        topBullet.shoot(player.x + 50, player.y);
                        bottomBullet.shoot(player.x + 50, player.y + player.flyingHeight - BULLET_SIZE);
                        bulletTops.push_back(topBullet);
                        bulletBottoms.push_back(bottomBullet); // Sửa thành bottomBullet
                        lastShotTime = currentTime;
                    }
                }
            }
        }

        bg1X -= bgSpeed;
        bg2X -= bgSpeed;
        if (bg1X <= -bgWidth) bg1X = bg2X + bgWidth;
        if (bg2X <= -bgWidth) bg2X = bg1X + bgWidth;

        if (state == RUNNING) {
            object.update();
            if (object.getX() < -Object::getSize()) {
                object.reset(WINDOW_WIDTH);
                player.score += 10;
            }

            player.updateJump();
            player.updateAnimation();
            if (object.isActive() && player.x + 50 > object.getX() &&
                player.x < object.getX() + Object::getSize() &&
                player.y + 50 > object.getY()) {
                cout << "Trò chơi kết thúc! Điểm: " << player.score << endl;
                running = false;
            }
            if (player.score >= 2) {
                state = FLYING;
                player.y = WINDOW_HEIGHT / 2;
                enemy.active = true;
            }
        }
        else if (state == FLYING) {
            player.movePlane(SDL_GetKeyboardState(NULL));
            enemy.update();

            for (size_t i = 0; i < bulletTops.size();) {
                bulletTops[i].update();
                if (bulletTops[i].getX() > WINDOW_WIDTH) {
                    bulletTops.erase(bulletTops.begin() + i);
                } else {
                    i++;
                }
            }

            for (size_t i = 0; i < bulletBottoms.size();) {
                bulletBottoms[i].update();
                if (bulletBottoms[i].getX() > WINDOW_WIDTH) {
                    bulletBottoms.erase(bulletBottoms.begin() + i);
                } else {
                    i++;
                }
            }

            SDL_Rect enemyRect = enemy.getRect();
            for (size_t i = 0; i < bulletTops.size();) {
                SDL_Rect bulletRect = bulletTops[i].getRect();
                if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                    enemy.reset();
                    bulletTops.erase(bulletTops.begin() + i);
                    player.score += 20;
                } else {
                    i++;
                }
            }
            for (size_t i = 0; i < bulletBottoms.size();) {
                SDL_Rect bulletRect = bulletBottoms[i].getRect();
                if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                    enemy.reset();
                    bulletBottoms.erase(bulletBottoms.begin() + i);
                    player.score += 20;
                } else {
                    i++;
                }
            }

            SDL_Rect playerRect = player.getRect(state);
            if (SDL_HasIntersection(&playerRect, &enemyRect)) {
                cout << "End game !!! Score: " << player.score << endl;
                running = false;
            }
        }

        SDL_RenderClear(renderer);

        SDL_Rect bgRect1 = {bg1X, 0, bgWidth, WINDOW_HEIGHT};
        SDL_Rect bgRect2 = {bg2X, 0, bgWidth, WINDOW_HEIGHT};
        if (state == RUNNING) {
            SDL_RenderCopy(renderer, background1Running, NULL, &bgRect1);
            SDL_RenderCopy(renderer, background2Running, NULL, &bgRect2);
        } else {
            SDL_RenderCopy(renderer, background1Flying, NULL, &bgRect1);
            SDL_RenderCopy(renderer, background2Flying, NULL, &bgRect2);
        }

        SDL_Rect playerRect = player.getRect(state);
        SDL_RenderCopy(renderer, player.getCurrentTexture(state), NULL, &playerRect);

        if (state == RUNNING) {
            SDL_Rect objectRect = {object.getX(), object.getY(), Object::getSize(), Object::getSize()};
            SDL_RenderCopy(renderer, object.getTexture(), NULL, &objectRect);
        }
        else if (state == FLYING) {
            SDL_Rect enemyRect = enemy.getRect();
            SDL_RenderCopy(renderer, enemy.currentTexture, NULL, &enemyRect);

            for (const Bullet& bullet : bulletTops) {
                if (bullet.isActive()) {
                    SDL_Rect bulletRect = bullet.getRect();
                    SDL_RenderCopy(renderer, bullet.getTexture(), NULL, &bulletRect);
                }
            }

            for (const Bullet& bullet : bulletBottoms) {
                if (bullet.isActive()) {
                    SDL_Rect bulletRect = bullet.getRect();
                    SDL_RenderCopy(renderer, bullet.getTexture(), NULL, &bulletRect);
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(background1Running);
    SDL_DestroyTexture(background2Running);
    SDL_DestroyTexture(background1Flying);
    SDL_DestroyTexture(background2Flying);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
