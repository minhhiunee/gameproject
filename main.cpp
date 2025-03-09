#include <SDL.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PLAYER_SIZE = 50;
const int OBSTACLE_SIZE = 50;
const int ENEMY_SIZE = 50;
const int BULLET_SIZE = 10;

// Trạng thái trò chơi
enum GameState {
    RUNNING,  // Giai đoạn chạy trốn
    FLYING    // Giai đoạn lái máy bay
};

struct Player {
    int x, y;
    int speed;
    int score;
    bool isJumping;
    int jumpHeight;
};

struct Obstacle {
    int x, y;
    bool active;
};

struct Enemy {
    int x, y;
    bool active;
};

struct Bullet {
    int x, y;
    bool active;
};

int main(int argc, char* argv[]) {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Escape and Fly", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Khởi tạo nhân vật
    Player player = {100, WINDOW_HEIGHT - PLAYER_SIZE - 50, 5, 0, false, 0};
    Obstacle obstacle = {WINDOW_WIDTH, WINDOW_HEIGHT - OBSTACLE_SIZE - 50, true};
    Enemy enemy = {WINDOW_WIDTH, 100, false};
    Bullet bullet = {0, 0, false};
    GameState state = RUNNING;

    bool running = true;
    SDL_Event e;

    while (running) {
        // Xử lý sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) {
                if (state == RUNNING && e.key.keysym.sym == SDLK_SPACE && !player.isJumping) {
                    player.isJumping = true;
                    player.jumpHeight = 15;
                }
                if (state == FLYING && e.key.keysym.sym == SDLK_SPACE && !bullet.active) {
                    bullet.x = player.x + PLAYER_SIZE;
                    bullet.y = player.y;
                    bullet.active = true;
                }
            }
        }

        // Cập nhật logic trò chơi
        if (state == RUNNING) {
            // Di chuyển chướng ngại vật
            obstacle.x -= 5;
            if (obstacle.x < -OBSTACLE_SIZE) {
                obstacle.x = WINDOW_WIDTH;
                player.score += 10;
            }

            // Nhảy
            if (player.isJumping) {
                player.y -= player.jumpHeight;
                player.jumpHeight -= 1;
                if (player.y >= WINDOW_HEIGHT - PLAYER_SIZE - 50) {
                    player.y = WINDOW_HEIGHT - PLAYER_SIZE - 50;
                    player.isJumping = false;
                }
            }

            // Va chạm với chướng ngại vật
            if (obstacle.active && player.x + PLAYER_SIZE > obstacle.x && player.x < obstacle.x + OBSTACLE_SIZE &&
                player.y + PLAYER_SIZE > obstacle.y) {
                std::cout << "Game Over! Score: " << player.score << std::endl;
                running = false;
            }

            // Chuyển sang giai  đoạn máy bay khi đủ điểm
            if (player.score >= 2) {
                state = FLYING;
                player.y = WINDOW_HEIGHT / 2; // Đưa máy bay lên giữa màn hình
                enemy.active = true;
            }
        } else if (state == FLYING) {
            // Điều khiển máy bay
            const Uint8* keystates = SDL_GetKeyboardState(NULL);
            if (keystates[SDL_SCANCODE_UP]) player.y -= player.speed;
            if (keystates[SDL_SCANCODE_DOWN]) player.y += player.speed;
            if (player.y < 0) player.y = 0;
            if (player.y > WINDOW_HEIGHT - PLAYER_SIZE) player.y = WINDOW_HEIGHT - PLAYER_SIZE;

            // Di chuyển kẻ địch
            enemy.x -= 5;
            if (enemy.x < -ENEMY_SIZE) {
                enemy.x = WINDOW_WIDTH;
                enemy.y = rand() % (WINDOW_HEIGHT - ENEMY_SIZE);
            }

            // Di chuyển đạn
            if (bullet.active) {
                bullet.x += 10;
                if (bullet.x > WINDOW_WIDTH) bullet.active = false;

                // Va chạm với kẻ địch
                if (bullet.x + BULLET_SIZE > enemy.x && bullet.x < enemy.x + ENEMY_SIZE &&
                    bullet.y + BULLET_SIZE > enemy.y && bullet.y < enemy.y + ENEMY_SIZE) {
                    enemy.x = WINDOW_WIDTH;
                    enemy.y = rand() % (WINDOW_HEIGHT - ENEMY_SIZE);
                    bullet.active = false;
                    player.score += 20;
                }
            }

            // Va chạm với kẻ địch
            if (player.x + PLAYER_SIZE > enemy.x && player.x < enemy.x + ENEMY_SIZE &&
                player.y + PLAYER_SIZE > enemy.y && player.y < enemy.y + ENEMY_SIZE) {
                std::cout << "Game Over! Score: " << player.score << std::endl;
                running = false;
            }
        }

        // Vẽ lên màn hình
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ nhân vật
        SDL_Rect playerRect = {player.x, player.y, PLAYER_SIZE, PLAYER_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ
        SDL_RenderFillRect(renderer, &playerRect);

        if (state == RUNNING) {
            // Vẽ chướng ngại vật
            SDL_Rect obstacleRect = {obstacle.x, obstacle.y, OBSTACLE_SIZE, OBSTACLE_SIZE};
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Màu xanh lá
            SDL_RenderFillRect(renderer, &obstacleRect);
        } else if (state == FLYING) {
            // Vẽ kẻ địch
            SDL_Rect enemyRect = {enemy.x, enemy.y, ENEMY_SIZE, ENEMY_SIZE};
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Màu xanh dương
            SDL_RenderFillRect(renderer, &enemyRect);

            // Vẽ đạn
            if (bullet.active) {
                SDL_Rect bulletRect = {bullet.x, bullet.y, BULLET_SIZE, BULLET_SIZE};
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Màu vàng
                SDL_RenderFillRect(renderer, &bulletRect);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Dọn dẹp
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
