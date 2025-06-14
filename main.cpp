#include "header/level.h"
#include "header/button.h"
#include "header/texture_manager.h"
#include "header/text_renderer.h"
#include "header/constants.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>

const int BUTTON_SIZE = 60;
const int BUTTON_MARGIN = 10;
const int CENTER_BUTTON_SIZE = 40;

void ShowPopup(SDL_Renderer* renderer, TTF_Font* font, const std::string& message, int duration = 2000) {
    SDL_Color popupColor = {255, 255, 255, 255};
    SDL_Color bgColor = {0, 0, 0, 200};

    SDL_Texture* msgTexture = TextRenderer::RenderText(renderer, font, message, popupColor);
    if (msgTexture) {
        int texW, texH;
        SDL_QueryTexture(msgTexture, nullptr, nullptr, &texW, &texH);

        SDL_Rect bgRect = {
            SCREEN_WIDTH / 2 - texW / 2 - 20,
            SCREEN_HEIGHT / 2 - texH / 2 - 20,
            texW + 40,
            texH + 40
        };

        SDL_Rect textRect = {
            SCREEN_WIDTH / 2 - texW / 2,
            SCREEN_HEIGHT / 2 - texH / 2,
            texW,
            texH
        };

        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(renderer, &bgRect);
        SDL_RenderCopy(renderer, msgTexture, nullptr, &textRect);

        TextRenderer::DestroyTextTexture(msgTexture);
        SDL_RenderPresent(renderer);
        SDL_Delay(duration);
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window* window = SDL_CreateWindow("Maze Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* wallTexture = TextureManager::LoadTexture(renderer, "assets/images/wall.png");
    SDL_Texture* floorTexture = TextureManager::LoadTexture(renderer, "assets/images/floor.png");
    SDL_Texture* doorTexture = TextureManager::LoadTexture(renderer, "assets/images/door.png");
    SDL_Texture* player1Texture = TextureManager::LoadTexture(renderer, "assets/images/player1.png");
    SDL_Texture* player2Texture = TextureManager::LoadTexture(renderer, "assets/images/player2.png");
    SDL_Texture* arrowUp = TextureManager::LoadTexture(renderer, "assets/images/arrow_up.png");
    SDL_Texture* arrowDown = TextureManager::LoadTexture(renderer, "assets/images/arrow_down.png");
    SDL_Texture* arrowLeft = TextureManager::LoadTexture(renderer, "assets/images/arrow_left.png");
    SDL_Texture* arrowRight = TextureManager::LoadTexture(renderer, "assets/images/arrow_right.png");
    SDL_Texture* centerTexture = TextureManager::LoadTexture(renderer, "assets/images/center.png");

    TTF_Font* font = TTF_OpenFont("assets/fonts/lonely.ttf", 24);
    TTF_Font* titleFont = TTF_OpenFont("assets/fonts/lonely.ttf", 36);
    Mix_Music* bgMusic = Mix_LoadMUS("assets/sound/sounds.ogg");

    Level gameLevel(1);
    bool running = true;
    bool musicStarted = false;
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    std::vector<Button> buttons;

    // ---- PLAYER 1 BUTTONS ----
    const int P1_CENTER_X = SCREEN_WIDTH / 4;
    const int P1_CENTER_Y = SCREEN_HEIGHT + 60 + BUTTON_SIZE + BUTTON_MARGIN;

    buttons.emplace_back(P1_CENTER_X - BUTTON_SIZE / 2, P1_CENTER_Y - (BUTTON_SIZE + BUTTON_MARGIN), BUTTON_SIZE, BUTTON_SIZE, [&]() {
        if (gameLevel.getMaze().IsWalkable(gameLevel.getMaze().player1.x, gameLevel.getMaze().player1.y - 1))
            gameLevel.getMaze().player1.y--;
    }); // Up
    buttons.emplace_back(P1_CENTER_X - (BUTTON_SIZE + BUTTON_MARGIN), P1_CENTER_Y, BUTTON_SIZE, BUTTON_SIZE, [&]() {
        if (gameLevel.getMaze().IsWalkable(gameLevel.getMaze().player1.x - 1, gameLevel.getMaze().player1.y))
            gameLevel.getMaze().player1.x--;
    }); // Left
    buttons.emplace_back(P1_CENTER_X - BUTTON_SIZE / 2, P1_CENTER_Y + (BUTTON_SIZE + BUTTON_MARGIN), BUTTON_SIZE, BUTTON_SIZE, [&]() {
        if (gameLevel.getMaze().IsWalkable(gameLevel.getMaze().player1.x, gameLevel.getMaze().player1.y + 1))
            gameLevel.getMaze().player1.y++;
    }); // Down
    buttons.emplace_back(P1_CENTER_X + BUTTON_MARGIN, P1_CENTER_Y, BUTTON_SIZE, BUTTON_SIZE, [&]() {
        if (gameLevel.getMaze().IsWalkable(gameLevel.getMaze().player1.x + 1, gameLevel.getMaze().player1.y))
            gameLevel.getMaze().player1.x++;
    }); // Right
    buttons.emplace_back(P1_CENTER_X - CENTER_BUTTON_SIZE / 2, P1_CENTER_Y, CENTER_BUTTON_SIZE, CENTER_BUTTON_SIZE, []() {});

    // ---- PLAYER 2 BUTTONS ----
    const int P2_CENTER_X = 3 * SCREEN_WIDTH / 4;
    const int P2_CENTER_Y = SCREEN_HEIGHT + 60 + BUTTON_SIZE + BUTTON_MARGIN;

    buttons.emplace_back(P2_CENTER_X - BUTTON_SIZE / 2, P2_CENTER_Y - (BUTTON_SIZE + BUTTON_MARGIN), BUTTON_SIZE, BUTTON_SIZE, [&]() {
        if (gameLevel.getMaze().IsWalkable(gameLevel.getMaze().player2.x, gameLevel.getMaze().player2.y - 1))
            gameLevel.getMaze().player2.y--;
    }); // Up
    buttons.emplace_back(P2_CENTER_X - (BUTTON_SIZE + BUTTON_MARGIN), P2_CENTER_Y, BUTTON_SIZE, BUTTON_SIZE, [&]() {
        if (gameLevel.getMaze().IsWalkable(gameLevel.getMaze().player2.x - 1, gameLevel.getMaze().player2.y))
            gameLevel.getMaze().player2.x--;
    }); // Left
    buttons.emplace_back(P2_CENTER_X - BUTTON_SIZE / 2, P2_CENTER_Y + (BUTTON_SIZE + BUTTON_MARGIN), BUTTON_SIZE, BUTTON_SIZE, [&]() {
        if (gameLevel.getMaze().IsWalkable(gameLevel.getMaze().player2.x, gameLevel.getMaze().player2.y + 1))
            gameLevel.getMaze().player2.y++;
    }); // Down
    buttons.emplace_back(P2_CENTER_X + BUTTON_MARGIN, P2_CENTER_Y, BUTTON_SIZE, BUTTON_SIZE, [&]() {
        if (gameLevel.getMaze().IsWalkable(gameLevel.getMaze().player2.x + 1, gameLevel.getMaze().player2.y))
            gameLevel.getMaze().player2.x++;
    }); // Right
    buttons.emplace_back(P2_CENTER_X - CENTER_BUTTON_SIZE / 2, P2_CENTER_Y, CENTER_BUTTON_SIZE, CENTER_BUTTON_SIZE, []() {});

    // ---- MAIN LOOP ----
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            if (!musicStarted && (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN)) {
                if (bgMusic) Mix_PlayMusic(bgMusic, -1);
                musicStarted = true;
            }
            for (auto& btn : buttons) btn.handleEvent(&event);
        }

        bool timeUp = gameLevel.isTimeUp();
        gameLevel.update(timeUp);
        gameLevel.handlePlayerInput(timeUp, keyboardState);

        if (timeUp) {
            std::string winner = (gameLevel.getPlayer1Score() > gameLevel.getPlayer2Score())
                ? "Player 1 Wins!"
                : (gameLevel.getPlayer2Score() > gameLevel.getPlayer1Score())
                ? "Player 2 Wins!"
                : "Draw!";
            std::string result = winner + " Score: " + std::to_string(gameLevel.getPlayer1Score()) + " - " + std::to_string(gameLevel.getPlayer2Score());
            ShowPopup(renderer, font, result, 5000);
            running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        gameLevel.render(renderer, wallTexture, floorTexture, doorTexture, player1Texture, player2Texture, font);

        SDL_Rect panelRect = {0, SCREEN_HEIGHT, SCREEN_WIDTH, PANEL_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 40, 40, 50, 255);
        SDL_RenderFillRect(renderer, &panelRect);

        // Judul Kontrol
        if (titleFont) {
            SDL_Texture* p1 = TextRenderer::RenderText(renderer, titleFont, "Player 1 (WASD)", {255, 200, 100, 255});
            SDL_Texture* p2 = TextRenderer::RenderText(renderer, titleFont, "Player 2 (Arrow Keys)", {100, 200, 255, 255});
            if (p1) {
                SDL_Rect r = {SCREEN_WIDTH / 4 - 120, SCREEN_HEIGHT + 20, 240, 40};
                SDL_RenderCopy(renderer, p1, nullptr, &r);
                TextRenderer::DestroyTextTexture(p1);
            }
            if (p2) {
                SDL_Rect r = {3 * SCREEN_WIDTH / 4 - 120, SCREEN_HEIGHT + 20, 240, 40};
                SDL_RenderCopy(renderer, p2, nullptr, &r);
                TextRenderer::DestroyTextTexture(p2);
            }
        }

        for (auto& btn : buttons) btn.render(renderer);

        // Draw arrows
            SDL_Rect arrowRects[] = {
                // Player 1
                {P1_CENTER_X - BUTTON_SIZE / 2, P1_CENTER_Y - (BUTTON_SIZE + BUTTON_MARGIN), BUTTON_SIZE, BUTTON_SIZE}, // Up
                {P1_CENTER_X - (BUTTON_SIZE + BUTTON_MARGIN), P1_CENTER_Y, BUTTON_SIZE, BUTTON_SIZE}, // Left
                {P1_CENTER_X - BUTTON_SIZE / 2, P1_CENTER_Y + (BUTTON_SIZE + BUTTON_MARGIN), BUTTON_SIZE, BUTTON_SIZE}, // Down
                {P1_CENTER_X + BUTTON_MARGIN, P1_CENTER_Y, BUTTON_SIZE, BUTTON_SIZE}, // Right
                {P1_CENTER_X - CENTER_BUTTON_SIZE / 2, P1_CENTER_Y, CENTER_BUTTON_SIZE, CENTER_BUTTON_SIZE}, // Center

                // Player 2
                {P2_CENTER_X - BUTTON_SIZE / 2, P2_CENTER_Y - (BUTTON_SIZE + BUTTON_MARGIN), BUTTON_SIZE, BUTTON_SIZE}, // Up
                {P2_CENTER_X - (BUTTON_SIZE + BUTTON_MARGIN), P2_CENTER_Y, BUTTON_SIZE, BUTTON_SIZE}, // Left
                {P2_CENTER_X - BUTTON_SIZE / 2, P2_CENTER_Y + (BUTTON_SIZE + BUTTON_MARGIN), BUTTON_SIZE, BUTTON_SIZE}, // Down
                {P2_CENTER_X + BUTTON_MARGIN, P2_CENTER_Y, BUTTON_SIZE, BUTTON_SIZE}, // Right
                {P2_CENTER_X - CENTER_BUTTON_SIZE / 2, P2_CENTER_Y, CENTER_BUTTON_SIZE, CENTER_BUTTON_SIZE}, // Center
            };

        SDL_Texture* textures[] = {arrowUp, arrowLeft, arrowDown, arrowRight, centerTexture,
                                   arrowUp, arrowLeft, arrowDown, arrowRight, centerTexture};
        for (int i = 0; i < 10; ++i) SDL_RenderCopy(renderer, textures[i], nullptr, &arrowRects[i]);

        if (font) {
            SDL_Texture* levelText = TextRenderer::RenderText(renderer, font, "Level: " + std::to_string(gameLevel.getLevel()), {255, 255, 255, 255});
            if (levelText) {
                SDL_Rect r = {SCREEN_WIDTH / 2 - 100, WINDOW_HEIGHT - 40, 200, 30};
                SDL_RenderCopy(renderer, levelText, nullptr, &r);
                TextRenderer::DestroyTextTexture(levelText);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    // CLEANUP
    TextureManager::DestroyTexture(wallTexture);
    TextureManager::DestroyTexture(floorTexture);
    TextureManager::DestroyTexture(doorTexture);
    TextureManager::DestroyTexture(player1Texture);
    TextureManager::DestroyTexture(player2Texture);
    TextureManager::DestroyTexture(arrowUp);
    TextureManager::DestroyTexture(arrowDown);
    TextureManager::DestroyTexture(arrowLeft);
    TextureManager::DestroyTexture(arrowRight);
    TextureManager::DestroyTexture(centerTexture);
    if (font) TTF_CloseFont(font);
    if (titleFont) TTF_CloseFont(titleFont);
    if (bgMusic) Mix_FreeMusic(bgMusic);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
