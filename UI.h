#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "main.h"

class UI {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* largeFont;
    TTF_Font* mediumFont;
    SDL_Texture* scoreTexture;
    SDL_Texture* hpTexture;
    SDL_Texture* startTitleTexture;     // Văn bản tiêu đề cho Start
    SDL_Texture* startButtonTexture;    // Văn bản cho nút Start
    SDL_Texture* gameOverTitleTexture;  // Văn bản "Game Over"
    SDL_Texture* gameOverScoreTexture;  // Văn bản điểm số cuối cùng
    SDL_Texture* gameOverButtonTexture; // Văn bản cho nút Restart
    SDL_Rect scoreRect;
    SDL_Rect hpRect;
    SDL_Rect hpBarBackground;
    SDL_Rect hpBarForeground;
    SDL_Rect startTitleRect;     // Vị trí tiêu đề Start
    SDL_Rect startButtonRect;    // Vị trí nút Start
    SDL_Rect startButtonTextRect; // Vị trí văn bản trên nút Start
    SDL_Rect gameOverTitleRect;  // Vị trí "Game Over"
    SDL_Rect gameOverScoreRect;  // Vị trí điểm số Game Over
    SDL_Rect gameOverButtonRect; // Vị trí nút Restart
    SDL_Rect gameOverButtonTextRect; // Vị trí văn bản trên nút Restart

    void createScoreTexture(int score);
    void createHPTexture(int hp);
    void createStartScreenTextures();         // Tạo texture cho Start
    void createGameOverScreenTextures(int finalScore); // Tạo texture cho Game Over

public:
    UI(SDL_Renderer* renderer);
    ~UI();
    void update(int score, int hp);
    void render(GameState state, int finalScore = 0);
    bool isStartButtonClicked(int x, int y) const; // Kiểm tra nhấn nút Start
    bool isGameOverButtonClicked(int x, int y) const; // Kiểm tra nhấn nút Restart
};

#endif
