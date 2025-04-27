#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Object.h"
#include "UI.h"
#include "main.h"
using namespace std;

const int BULLET_SIZE = 35;

int main(int argc, char* argv[]) {
    // Khởi tạo random seed
    srand(SDL_GetTicks());

    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL không thể khởi tạo! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) {
        cerr << "SDL_image không thể khởi tạo! IMG_Error: " << IMG_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    // Tạo cửa sổ và renderer
    SDL_Window* window = SDL_CreateWindow("Escape from the planet", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

    // Tải nền cho RUNNING
    SDL_Surface* bgSurfaceRunning = IMG_Load("backgroundrunning.png");
    if (!bgSurfaceRunning) {
        cerr << "Không thể tải backgroundrunning.png! IMG_Error: " << IMG_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Texture* background1Running = SDL_CreateTextureFromSurface(renderer, bgSurfaceRunning);
    SDL_Texture* background2Running = SDL_CreateTextureFromSurface(renderer, bgSurfaceRunning);

    // Tải nền cho FLYING
    SDL_Surface* bgSurfaceFlying = IMG_Load("backgroundflying.png");
    if (!bgSurfaceFlying) {
        cerr << "Không thể tải backgroundflying.png! IMG_Error: " << IMG_GetError() << endl;
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

    // Giải phóng surface
    SDL_FreeSurface(bgSurfaceRunning);
    SDL_FreeSurface(bgSurfaceFlying);

    // Kiểm tra texture
    if (!background1Running || !background2Running || !background1Flying || !background2Flying) {
        cerr << "Không thể tạo texture! SDL_Error: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Khởi tạo các biến và đối tượng
    int bgWidth = WINDOW_WIDTH;
    int bg1X = 0;
    int bg2X = bgWidth;
    int bgSpeed = 3;
    int lastScoreMilestone = 0;
    Uint32 lastEnemySpawnTime = 0;
    Uint32 enemySpawnCooldown = 1000; // 1000ms ban đầu
    Uint32 lastShotTime = 0;
    const Uint32 defaultShootCooldown = 750; // Mặc định 750ms
    const Uint32 fastShootCooldown = 375; // Tăng tốc bắn: 375ms
    Uint32 shootCooldown = defaultShootCooldown;
    int powerUpType = 0; // 0: không có, 1: tăng tốc bắn, 2: bất tử, 3: hồi máu
    Uint32 powerUpStartTime = 0;
    const Uint32 powerUpDuration = 10000; // 10 giây
    const Uint32 defaultEnemyShootCooldown = 750; // Cooldown bắn enemy mặc định
    Uint32 enemyShootCooldown = defaultEnemyShootCooldown; // Cooldown bắn enemy hiện tại
    Uint32 lastEnemyShootTime = 0;
    const float defaultPlayerSpeed = 8.0f;
    float playerSpeed = defaultPlayerSpeed; // Tốc độ người chơi hiện tại
    Uint32 lastHealthRegenTime = 0; // Thời gian hồi máu gần nhất

    Player player(100, WINDOW_HEIGHT - 350, renderer);
    Object object(WINDOW_WIDTH, WINDOW_HEIGHT, renderer);
    Enemy enemy(WINDOW_WIDTH, 100, renderer);
    vector<Bullet> bulletTops;
    vector<Bullet> bulletBottoms;
    vector<Bullet> enemyBullets;
    GameState state = START;
    GameState lastGameMode = RUNNING;
    int finalScore = 0;

    UI ui(renderer);

    // Vòng lặp game
    bool running = true;
    SDL_Event e;

    while (running) {
        Uint32 frameStart = SDL_GetTicks(); // Đo thời gian frame

        // Xử lý sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    if (state == START && ui.isStartButtonClicked(mouseX, mouseY)) {
                        state = RUNNING;
                    }
                    else if ((state == GAMEOVER || state == VICTORY) && ui.isGameOverButtonClicked(mouseX, mouseY)) {
                        // Khởi động lại game
                        state = RUNNING;
                        player.reset(100, WINDOW_HEIGHT - 350);
                        object.reset(WINDOW_WIDTH);
                        object.setSpeed(5);
                        enemy.reset();
                        enemy.active = false;
                        bulletTops.clear();
                        bulletBottoms.clear();
                        enemyBullets.clear();
                        bg1X = 0;
                        bg2X = bgWidth;
                        bgSpeed = 3;
                        lastScoreMilestone = 0;
                        lastEnemySpawnTime = 0;
                        enemySpawnCooldown = 750;
                        shootCooldown = defaultShootCooldown;
                        powerUpType = 0;
                        powerUpStartTime = 0;
                        enemyShootCooldown = defaultEnemyShootCooldown;
                        playerSpeed = defaultPlayerSpeed;
                        lastHealthRegenTime = 0;
                    }
                }
            }
            if (e.type == SDL_KEYDOWN) {
                if (state == START && e.key.keysym.sym == SDLK_s) {
                state = RUNNING;
                }
                else if (state == RUNNING && e.key.keysym.sym == SDLK_SPACE && !player.isJumping) {
                    player.isJumping = true;
                    player.jumpHeight = 15;
                }
                else if (state == FLYING && e.key.keysym.sym == SDLK_SPACE) {
                    Uint32 currentTime = SDL_GetTicks();
                    if (currentTime - lastShotTime >= shootCooldown) {
                        Bullet topBullet(renderer);
                        Bullet bottomBullet(renderer);
                        topBullet.shoot(player.x + 50, player.y);
                        bottomBullet.shoot(player.x + 50, player.y + player.flyingHeight - BULLET_SIZE);
                        bulletTops.push_back(topBullet);
                        bulletBottoms.push_back(bottomBullet);
                        lastShotTime = currentTime;
                    }
                }
                else if ((state == GAMEOVER || state == VICTORY) && e.key.keysym.sym == SDLK_RETURN) {
                    // Khởi động lại game khi nhấn Enter
                    state = RUNNING;
                    player.reset(100, WINDOW_HEIGHT - 350);
                    object.reset(WINDOW_WIDTH);
                    object.setSpeed(5);
                    enemy.reset();
                    enemy.active = false;
                    bulletTops.clear();
                    bulletBottoms.clear();
                    enemyBullets.clear();
                    bg1X = 0;
                    bg2X = bgWidth;
                    bgSpeed = 3;
                    lastScoreMilestone = 0;
                    lastEnemySpawnTime = 0;
                    enemySpawnCooldown = 1000;
                    shootCooldown = defaultShootCooldown;
                    powerUpType = 0;
                    powerUpStartTime = 0;
                    enemyShootCooldown = defaultEnemyShootCooldown;
                    playerSpeed = defaultPlayerSpeed;
                    lastHealthRegenTime = 0;
                }
            }
        }

        // Cập nhật logic game
        if (state == START || state == GAMEOVER || state == VICTORY) {
            // Không cập nhật trong trạng thái START, GAMEOVER hoặc VICTORY
        }
        else {
            // Kiểm tra điều kiện Victory
            if (player.score >= 1500) {
                powerUpType = 0;
                powerUpStartTime = 0;
                finalScore = player.score;
                state = VICTORY;
            }

            // Cập nhật vị trí nền
            bg1X -= bgSpeed;
            bg2X -= bgSpeed;
            if (bg1X <= -bgWidth) bg1X = bg2X + bgWidth;
            if (bg2X <= -bgWidth) bg2X = bg1X + bgWidth;

            if (state == RUNNING) {
                lastGameMode = RUNNING;
                object.update();
                if (object.getX() < -Object::getSize()) {
                    object.reset(WINDOW_WIDTH);
                    player.score += 10;
                }

                // Kiểm tra mốc điểm 50 để tăng tốc độ
                if (player.score >= lastScoreMilestone + 50) {
                    lastScoreMilestone += 50;
                    int newRunningSpeed = min(player.getRunningSpeed() + 1, 8);
                    bgSpeed = min(bgSpeed + 1, 8);
                    player.setRunningSpeed(newRunningSpeed);
                    object.setSpeed(object.getSpeed() + 1);
                }

                player.updateJump();
                player.updateAnimation();
                if (object.isActive() && player.x + 50 > object.getX() &&
                    player.x < object.getX() + Object::getSize() &&
                    player.y + 50 > object.getY()) {
                    finalScore = player.score;
                    powerUpType = 0;
                    powerUpStartTime = 0;
                    state = GAMEOVER;
                }
                if (player.score >= 30) {
                    state = FLYING;
                    player.x = 100;
                    player.y = WINDOW_HEIGHT / 2;
                    enemy.active = true;
                    bgSpeed = 3;
                }
            }
            else if (state == FLYING) {
                lastGameMode = FLYING;
                player.movePlane(SDL_GetKeyboardState(NULL), playerSpeed); // Truyền playerSpeed
                enemy.update();

                // Bắn đạn enemy với cooldown
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - lastEnemyShootTime >= enemyShootCooldown && enemy.active) {
                    enemy.shoot(enemyBullets, renderer);
                    lastEnemyShootTime = currentTime;
                }

                // Hồi máu nếu có bổ trợ hồi máu
                if (powerUpType == 3 && currentTime - lastHealthRegenTime >= 1000) {
                    player.hp = min(player.hp + 2, 100); // Hồi 2 HP, tối đa 100
                    lastHealthRegenTime = currentTime;
                }

                // Kiểm tra thời gian bổ trợ
                if (powerUpType != 0 && SDL_GetTicks() - powerUpStartTime > powerUpDuration) {
                    if (powerUpType == 1) {
                        shootCooldown = defaultShootCooldown;
                        playerSpeed = defaultPlayerSpeed; // Reset tốc độ người chơi
                    }
                    powerUpType = 0;
                    powerUpStartTime = 0;
                }

                // Cập nhật đạn người chơi (trên)
                for (int i = bulletTops.size() - 1; i >= 0; --i) {
                    bulletTops[i].update();
                    if (bulletTops[i].getX() > WINDOW_WIDTH) {
                        bulletTops.erase(bulletTops.begin() + i);
                    }
                }

                // Cập nhật đạn người chơi (dưới)
                for (int i = bulletBottoms.size() - 1; i >= 0; --i) {
                    bulletBottoms[i].update();
                    if (bulletBottoms[i].getX() > WINDOW_WIDTH) {
                        bulletBottoms.erase(bulletBottoms.begin() + i);
                    }
                }

                // Cập nhật đạn của enemy
                for (int i = enemyBullets.size() - 1; i >= 0; --i) {
                    enemyBullets[i].update();
                    if (enemyBullets[i].getX() < -BULLET_SIZE) {
                        enemyBullets.erase(enemyBullets.begin() + i);
                    }
                }

                // Kiểm tra va chạm giữa đạn người chơi và enemy
                SDL_Rect enemyRect = enemy.getRect();
                bool enemyKilledThisFrame = false;
                for (int i = bulletTops.size() - 1; i >= 0; --i) {
                    if (bulletTops[i].isActive()) {
                        SDL_Rect bulletRect = bulletTops[i].getRect();
                        if (SDL_HasIntersection(&bulletRect, &enemyRect) && enemy.active) {
                            enemy.triggerExplosion();
                            enemy.reset();
                            bulletTops.erase(bulletTops.begin() + i);
                            if (!enemyKilledThisFrame) {
                                player.score += 10;
                                enemyKilledThisFrame = true;
                            }
                        }
                    }
                }
                for (int i = bulletBottoms.size() - 1; i >= 0; --i) {
                    if (bulletBottoms[i].isActive()) {
                        SDL_Rect bulletRect = bulletBottoms[i].getRect();
                        if (SDL_HasIntersection(&bulletRect, &enemyRect) && enemy.active) {
                            enemy.triggerExplosion();
                            enemy.reset();
                            bulletBottoms.erase(bulletBottoms.begin() + i);
                            if (!enemyKilledThisFrame) {
                                player.score += 10;
                                enemyKilledThisFrame = true;
                            }
                        }
                    }
                }

                // Kiểm tra mốc điểm 50 để kích hoạt bổ trợ và giảm cooldown
                if (player.score >= lastScoreMilestone + 50) {
                    lastScoreMilestone += 50;
                    enemySpawnCooldown = max<Uint32>(50, enemySpawnCooldown - 100); // Giảm 100ms, tối thiểu 50ms
                    enemyShootCooldown = max<Uint32>(50, enemyShootCooldown - 50); // Giảm 50ms, tối thiểu 50ms

                    // Kích hoạt bổ trợ ngẫu nhiên
                    if (powerUpType == 0) { // Chỉ kích hoạt nếu không có bổ trợ
                        powerUpType = (rand() % 3) + 1; // 1: tăng tốc bắn, 2: bất tử, 3: hồi máu
                        powerUpStartTime = SDL_GetTicks();
                        if (powerUpType == 1) {
                            shootCooldown = fastShootCooldown;
                            playerSpeed = 12.0f; // Tăng tốc độ người chơi
                        }
                    }
                }

                // Kiểm tra va chạm giữa đạn enemy và người chơi
                SDL_Rect playerRect = player.getRect(state);
                if (powerUpType != 2) { // Chỉ nhận sát thương nếu không bất tử
                    for (int i = enemyBullets.size() - 1; i >= 0; --i) {
                        if (enemyBullets[i].isActive()) {
                            SDL_Rect bulletRect = enemyBullets[i].getRect();
                            if (SDL_HasIntersection(&bulletRect, &playerRect)) {
                                player.takeDamage(10);
                                enemyBullets.erase(enemyBullets.begin() + i);
                            }
                        }
                    }
                }

                // Kiểm tra va chạm trực tiếp giữa người chơi và enemy
                if (powerUpType != 2 && SDL_HasIntersection(&playerRect, &enemyRect)) {
                    finalScore = player.score;
                    powerUpType = 0;
                    powerUpStartTime = 0;
                    state = GAMEOVER;
                }

                // Kiểm tra enemy thoát màn hình
                if (enemy.getRect().x <= -enemy.getRect().w) {
                    finalScore = player.score;
                    powerUpType = 0;
                    powerUpStartTime = 0;
                    state = GAMEOVER;
                }

                // Kiểm tra HP người chơi
                if (!player.isAlive()) {
                    finalScore = player.score;
                    powerUpType = 0;
                    powerUpStartTime = 0;
                    state = GAMEOVER;
                }
            }

            // Cập nhật UI
            ui.update(player.score, player.hp, powerUpType, powerUpStartTime, powerUpDuration);
        }

        // Vẽ lên màn hình
        SDL_RenderClear(renderer);

        // Vẽ nền
        SDL_Rect bgRect1 = {bg1X, 0, bgWidth, WINDOW_HEIGHT};
        SDL_Rect bgRect2 = {bg2X, 0, bgWidth, WINDOW_HEIGHT};
        if (state == START || state == RUNNING || ((state == GAMEOVER || state == VICTORY) && lastGameMode == RUNNING)) {
            SDL_RenderCopy(renderer, background1Running, NULL, &bgRect1);
            SDL_RenderCopy(renderer, background2Running, NULL, &bgRect2);
        }
        else if (state == FLYING || ((state == GAMEOVER || state == VICTORY) && lastGameMode == FLYING)) {
            SDL_RenderCopy(renderer, background1Flying, NULL, &bgRect1);
            SDL_RenderCopy(renderer, background2Flying, NULL, &bgRect2);
        }

        // Vẽ các đối tượng trong trạng thái RUNNING hoặc FLYING
        if (state == RUNNING || state == FLYING) {
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

                for (const Bullet& bullet : enemyBullets) {
                    if (bullet.isActive()) {
                        SDL_Rect bulletRect = bullet.getRect();
                        SDL_RenderCopy(renderer, bullet.getTexture(), NULL, &bulletRect);
                    }
                }
                enemy.renderExplosion(renderer);
            }
        }

        // Vẽ UI
        ui.render(state, finalScore, powerUpType);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Duy trì ~60 FPS

    }

    // Giải phóng tài nguyên
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
