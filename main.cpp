#include <SDL.h>
#include <iostream>
#include <SDL_image.h>

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Object.h"


using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int BULLET_SIZE = 10;

enum GameState {
    RUNNING,
    FLYING
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Escape and Fly", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() <<endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    Player player(100, WINDOW_HEIGHT - 50 - 50);
    Object object(WINDOW_WIDTH, WINDOW_HEIGHT);
    Enemy enemy(WINDOW_WIDTH, 100);
    Bullet bullet;
    GameState state = RUNNING;

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) {
                if (state == RUNNING && e.key.keysym.sym == SDLK_SPACE && !player.isJumping) {
                    player.isJumping = true;
                    player.jumpHeight = 15;
                }
                if (state == FLYING && e.key.keysym.sym == SDLK_SPACE && !bullet.isActive()) {
                    bullet.shoot(player.x + 50, player.y);
                }
            }
        }

        if (state == RUNNING) {
            object.update();
            if (object.getX() < -Object::getSize()) {
                object.reset(WINDOW_WIDTH);
                player.score += 10;
            }

            player.updateJump();

            if (object.isActive() && player.x + 50 > object.getX() &&
                player.x < object.getX() + Object::getSize() &&
                player.y + 50 > object.getY()) {
                cout << "Game Over! Score: " << player.score << endl;
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
            bullet.update();

            if (bullet.isActive() && bullet.getX() > WINDOW_WIDTH) {
                bullet.deactivate();
            }

            if (bullet.isActive() &&
                bullet.getX() + BULLET_SIZE > enemy.x && bullet.getX() < enemy.x + 50 &&
                bullet.getY() + BULLET_SIZE > enemy.y && bullet.getY() < enemy.y + 50) {
                enemy.reset();
                bullet.deactivate();
                player.score += 20;
            }

            if (player.x + 50 > enemy.x && player.x < enemy.x + 50 &&
                player.y + 50 > enemy.y && player.y < enemy.y + 50) {
                cout << "Game Over! Score: " << player.score << endl;
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect playerRect = player.getRect();
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        if (state == RUNNING) {
            SDL_Rect objectRect = {object.getX(), object.getY(), Object::getSize(), Object::getSize()};
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &objectRect);
        }
        else if (state == FLYING) {
            SDL_Rect enemyRect = enemy.getRect();
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &enemyRect);

            if (bullet.isActive()) {
                SDL_Rect bulletRect = {bullet.getX(), bullet.getY(), BULLET_SIZE, BULLET_SIZE};
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(renderer, &bulletRect);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
