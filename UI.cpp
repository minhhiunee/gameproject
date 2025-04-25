#include "UI.h"
#include <string>
using namespace std;

UI::UI(SDL_Renderer* renderer) : renderer(renderer), font(nullptr), largeFont(nullptr), mediumFont(nullptr),
    scoreTexture(nullptr), hpTexture(nullptr), startTitleTexture(nullptr),
    startButtonTexture(nullptr), gameOverTitleTexture(nullptr),
    victoryTitleTexture(nullptr), gameOverScoreTexture(nullptr),
    gameOverButtonTexture(nullptr), powerUpTexture(nullptr),
    powerUpType(0), powerUpStartTime(0), powerUpDuration(0) {
    // Khởi tạo SDL_ttf
    if (TTF_Init() < 0) {
        SDL_Log("Failed to initialize SDL_ttf: %s", TTF_GetError());
        return;
    }

    // Load font mặc định
    font = TTF_OpenFont("arial.ttf", 30);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return;
    }

    // Load font lớn cho "GAME OVER" và "Victory"
    largeFont = TTF_OpenFont("arial.ttf", 150);
    if (!largeFont) {
        SDL_Log("Failed to load large font: %s", TTF_GetError());
        return;
    }

    // Load font vừa cho "GAME START"
    mediumFont = TTF_OpenFont("arial.ttf", 85);
    if (!mediumFont) {
        SDL_Log("Failed to load medium font: %s", TTF_GetError());
        return;
    }

    // Khởi tạo vị trí các thành phần UI
    scoreRect = {10, 10, 0, 0};
    hpRect = {1280 - 110, 10, 0, 0};
    hpBarBackground = {1280 - 110, 40, 100, 20};
    hpBarForeground = {1280 - 110, 40, 100, 20};
    startTitleRect = {1280 / 2 - 450, 720 / 2 - 250, 0, 0};
    startButtonRect = {1280 / 2 - 100, 720 / 2 + 10, 200, 50};
    startButtonTextRect = {1280 / 2 - 40, 720 / 2 + 20, 0, 0};
    gameOverTitleRect = {1280 / 2 - 150, 100, 0, 0};
    victoryTitleRect = {1280 / 2 - 150, 100, 0, 0};
    gameOverScoreRect = {1280 / 2 - 50, 720 / 2 + 50, 0, 0};
    gameOverButtonRect = {1280 / 2 - 100, 720 - 150, 200, 50};
    gameOverButtonTextRect = {1280 / 2 - 50, 720 - 140, 0, 0};
    powerUpRect = {10, 40, 150, 30}; // Text bổ trợ ở góc trên trái, dưới score

    // Tạo texture cho Start
    createStartScreenTextures();
}

UI::~UI() {
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    if (hpTexture) SDL_DestroyTexture(hpTexture);
    if (startTitleTexture) SDL_DestroyTexture(startTitleTexture);
    if (startButtonTexture) SDL_DestroyTexture(startButtonTexture);
    if (gameOverTitleTexture) SDL_DestroyTexture(gameOverTitleTexture);
    if (victoryTitleTexture) SDL_DestroyTexture(victoryTitleTexture);
    if (gameOverScoreTexture) SDL_DestroyTexture(gameOverScoreTexture);
    if (gameOverButtonTexture) SDL_DestroyTexture(gameOverButtonTexture);
    if (powerUpTexture) SDL_DestroyTexture(powerUpTexture);
    if (font) TTF_CloseFont(font);
    if (largeFont) TTF_CloseFont(largeFont);
    if (mediumFont) TTF_CloseFont(mediumFont);
    TTF_Quit();
}

void UI::createScoreTexture(int score) {
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    string scoreText = "Score: " + to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), {255, 255, 255, 255});
    if (!surface) {
        SDL_Log("Failed to create score surface: %s", TTF_GetError());
        return;
    }
    scoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
    scoreRect.w = surface->w;
    scoreRect.h = surface->h;
    SDL_FreeSurface(surface);
}

void UI::createHPTexture(int hp) {
    if (hpTexture) SDL_DestroyTexture(hpTexture);
    string hpText = "HP: " + to_string(hp);
    SDL_Surface* surface = TTF_RenderText_Solid(font, hpText.c_str(), {255, 255, 255, 255});
    if (!surface) {
        SDL_Log("Failed to create HP surface: %s", TTF_GetError());
        return;
    }
    hpTexture = SDL_CreateTextureFromSurface(renderer, surface);
    hpRect.w = surface->w;
    hpRect.h = surface->h;
    SDL_FreeSurface(surface);
    hpBarForeground.w = (hp * 100) / 100;
}

void UI::createStartScreenTextures() {
    if (startTitleTexture) SDL_DestroyTexture(startTitleTexture);
    SDL_Surface* titleSurface = TTF_RenderText_Solid(mediumFont, "Escape from the planet !", {255, 255, 255, 255});
    if (!titleSurface) {
        SDL_Log("Failed to create start title surface: %s", TTF_GetError());
        return;
    }
    startTitleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    startTitleRect.w = titleSurface->w;
    startTitleRect.h = titleSurface->h;
    startTitleRect.x = (1280 - startTitleRect.w) / 2;
    SDL_FreeSurface(titleSurface);

    if (startButtonTexture) SDL_DestroyTexture(startButtonTexture);
    SDL_Surface* buttonSurface = TTF_RenderText_Solid(font, "Start", {255, 255, 255, 255});
    if (!buttonSurface) {
        SDL_Log("Failed to create start button surface: %s", TTF_GetError());
        return;
    }
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    startButtonTextRect.w = buttonSurface->w;
    startButtonTextRect.h = buttonSurface->h;
    SDL_FreeSurface(buttonSurface);
}

void UI::createEndScreenTextures(int finalScore, GameState state) {
    if (state == GAMEOVER) {
        if (gameOverTitleTexture) SDL_DestroyTexture(gameOverTitleTexture);
        SDL_Surface* titleSurface = TTF_RenderText_Solid(largeFont, "Game Over", {255, 255, 255, 255});
        if (!titleSurface) {
            SDL_Log("Failed to create game over title surface: %s", TTF_GetError());
            return;
        }
        gameOverTitleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        gameOverTitleRect.w = titleSurface->w;
        gameOverTitleRect.h = titleSurface->h;
        gameOverTitleRect.x = (1280 - gameOverTitleRect.w) / 2;
        SDL_FreeSurface(titleSurface);
    } else if (state == VICTORY) {
        if (victoryTitleTexture) SDL_DestroyTexture(victoryTitleTexture);
        SDL_Surface* titleSurface = TTF_RenderText_Solid(largeFont, "Victory", {255, 215, 0, 255});
        if (!titleSurface) {
            SDL_Log("Failed to create victory title surface: %s", TTF_GetError());
            return;
        }
        victoryTitleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        victoryTitleRect.w = titleSurface->w;
        victoryTitleRect.h = titleSurface->h;
        victoryTitleRect.x = (1280 - victoryTitleRect.w) / 2;
        SDL_FreeSurface(titleSurface);
    }

    if (gameOverScoreTexture) SDL_DestroyTexture(gameOverScoreTexture);
    string scoreText = "Score: " + to_string(finalScore);
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), {255, 215, 0, 255});
    if (!scoreSurface) {
        SDL_Log("Failed to create end screen score surface: %s", TTF_GetError());
        return;
    }
    gameOverScoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    gameOverScoreRect.w = scoreSurface->w;
    gameOverScoreRect.h = scoreSurface->h;
    gameOverScoreRect.x = (1280 - gameOverScoreRect.w) / 2;
    SDL_FreeSurface(scoreSurface);

    if (gameOverButtonTexture) SDL_DestroyTexture(gameOverButtonTexture);
    SDL_Surface* buttonSurface = TTF_RenderText_Solid(font, "Restart", {255, 255, 255, 255});
    if (!buttonSurface) {
        SDL_Log("Failed to create end screen button surface: %s", TTF_GetError());
        return;
    }
    gameOverButtonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    gameOverButtonTextRect.w = buttonSurface->w;
    gameOverButtonTextRect.h = buttonSurface->h;
    gameOverButtonTextRect.x = (1280 - gameOverButtonTextRect.w) / 2;
    gameOverButtonRect.x = (1280 - gameOverButtonRect.w) / 2;
    SDL_FreeSurface(buttonSurface);
}

void UI::update(int score, int hp, int powerUpType, Uint32 powerUpStartTime, Uint32 powerUpDuration) {
    createScoreTexture(score);
    createHPTexture(hp);
    this->powerUpType = powerUpType;
    this->powerUpStartTime = powerUpStartTime;
    this->powerUpDuration = powerUpDuration;

    // Cập nhật text bổ trợ
    if (powerUpType != 0 && powerUpTexture) {
        SDL_DestroyTexture(powerUpTexture);
        powerUpTexture = nullptr;
    }
    if (powerUpType == 1) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, "Fast Shooting", {255, 255, 255, 255});
        if (!surface) {
            SDL_Log("Failed to create power-up surface: %s", TTF_GetError());
            return;
        }
        powerUpTexture = SDL_CreateTextureFromSurface(renderer, surface);
        powerUpRect.w = surface->w;
        powerUpRect.h = surface->h;
        SDL_FreeSurface(surface);
    } else if (powerUpType == 2) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, "Invincibility", {255, 255, 255, 255});
        if (!surface) {
            SDL_Log("Failed to create power-up surface: %s", TTF_GetError());
            return;
        }
        powerUpTexture = SDL_CreateTextureFromSurface(renderer, surface);
        powerUpRect.w = surface->w;
        powerUpRect.h = surface->h;
        SDL_FreeSurface(surface);
    } else if (powerUpType == 3) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, "Health Regen", {255, 255, 255, 255});
        if (!surface) {
            SDL_Log("Failed to create power-up surface: %s", TTF_GetError());
            return;
        }
        powerUpTexture = SDL_CreateTextureFromSurface(renderer, surface);
        powerUpRect.w = surface->w;
        powerUpRect.h = surface->h;
        SDL_FreeSurface(surface);
    }
}

void UI::render(GameState state, int finalScore, int powerUpType) {
    if (state == START) {
        if (startTitleTexture) SDL_RenderCopy(renderer, startTitleTexture, nullptr, &startTitleRect);
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &startButtonRect);
        if (startButtonTexture) SDL_RenderCopy(renderer, startButtonTexture, nullptr, &startButtonTextRect);
    } else if (state == GAMEOVER || state == VICTORY) {
        createEndScreenTextures(finalScore, state);
        if (state == GAMEOVER && gameOverTitleTexture) {
            SDL_RenderCopy(renderer, gameOverTitleTexture, nullptr, &gameOverTitleRect);
        } else if (state == VICTORY && victoryTitleTexture) {
            SDL_RenderCopy(renderer, victoryTitleTexture, nullptr, &victoryTitleRect);
        }
        if (gameOverScoreTexture) SDL_RenderCopy(renderer, gameOverScoreTexture, nullptr, &gameOverScoreRect);
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &gameOverButtonRect);
        if (gameOverButtonTexture) SDL_RenderCopy(renderer, gameOverButtonTexture, nullptr, &gameOverButtonTextRect);
    } else {
        if (scoreTexture) SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
        if (state == FLYING) {
            if (hpTexture) SDL_RenderCopy(renderer, hpTexture, nullptr, &hpRect);
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderFillRect(renderer, &hpBarBackground);
            SDL_SetRenderDrawColor(renderer, hpBarForeground.w > 50 ? 0 : 255, hpBarForeground.w > 50 ? 255 : 0, 0, 255);
            SDL_RenderFillRect(renderer, &hpBarForeground);
        }
    }

    // Vẽ text bổ trợ nếu còn hiệu lực
    if (powerUpType != 0 && powerUpTexture && SDL_GetTicks() - powerUpStartTime <= powerUpDuration) {
        SDL_RenderCopy(renderer, powerUpTexture, nullptr, &powerUpRect);
    }
}

bool UI::isStartButtonClicked(int x, int y) const {
    return (x >= startButtonRect.x && x <= startButtonRect.x + startButtonRect.w &&
            y >= startButtonRect.y && y <= startButtonRect.y + startButtonRect.h);
}

bool UI::isGameOverButtonClicked(int x, int y) const {
    return (x >= gameOverButtonRect.x && x <= gameOverButtonRect.x + gameOverButtonRect.w &&
            y >= gameOverButtonRect.y && y <= gameOverButtonRect.y + gameOverButtonRect.h);
}
