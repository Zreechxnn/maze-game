#include "header/level.h"
#include "header/constants.h"
#include "header/text_renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

Level::Level(int startLevel)
    : level(startLevel),
      player1Score(0),
      player2Score(0),
      maze(level),
      timeLimit(TIME_LIMIT * 1000) {
    levelStartTime = SDL_GetTicks();
    timeUp = false;
}

void Level::update(bool timeUp) {
    this->timeUp = timeUp;
    
    if (!timeUp) {
        if (checkWinCondition()) {
            updateScores();
            nextLevel();
        }
    }
}

void Level::render(SDL_Renderer* renderer, 
                   SDL_Texture* wallTexture, 
                   SDL_Texture* floorTexture, 
                   SDL_Texture* doorTexture,
                   SDL_Texture* player1Texture,
                   SDL_Texture* player2Texture,
                   TTF_Font* font) {
    // Draw maze
    for (int x = 0; x < MAZE_WIDTH; ++x) {
        for (int y = 0; y < MAZE_HEIGHT; ++y) {
            SDL_Rect tileRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_Texture* texture = (maze.grid[x][y] == 1) ? wallTexture : floorTexture;
            SDL_RenderCopy(renderer, texture, nullptr, &tileRect);
        }
    }

    // Draw door
    SDL_Rect doorRect = {maze.endX * TILE_SIZE, maze.endY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, doorTexture, nullptr, &doorRect);

    // Draw players
    SDL_Rect player1Rect = {maze.player1.x * TILE_SIZE, maze.player1.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, player1Texture, nullptr, &player1Rect);

    SDL_Rect player2Rect = {maze.player2.x * TILE_SIZE, maze.player2.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, player2Texture, nullptr, &player2Rect);

    // Draw game info
    if (font) {
        std::string levelText = "Level: " + std::to_string(level);
        SDL_Texture* levelTexture = TextRenderer::RenderText(renderer, font, levelText, {255, 255, 255, 255});
        if (levelTexture) {
            SDL_Rect rect = {10, 10, 120, 30};
            SDL_RenderCopy(renderer, levelTexture, nullptr, &rect);
            TextRenderer::DestroyTextTexture(levelTexture);
        }

        int remainingTime = (timeLimit - getElapsedTime()) / 1000;
        if (remainingTime < 0) remainingTime = 0;
        std::string timerText = "Time: " + std::to_string(remainingTime);
        SDL_Texture* timerTexture = TextRenderer::RenderText(renderer, font, timerText, {255, 255, 255, 255});
        if (timerTexture) {
            SDL_Rect rect = {10, 50, 120, 30};
            SDL_RenderCopy(renderer, timerTexture, nullptr, &rect);
            TextRenderer::DestroyTextTexture(timerTexture);
        }

        // Tampilkan skor
        std::string scoreText = "P1: " + std::to_string(player1Score) + " | P2: " + std::to_string(player2Score);
        SDL_Texture* scoreTexture = TextRenderer::RenderText(renderer, font, scoreText, {255, 255, 0, 255});
        if (scoreTexture) {
            SDL_Rect rect = {SCREEN_WIDTH/2 - 80, 10, 160, 30};
            SDL_RenderCopy(renderer, scoreTexture, nullptr, &rect);
            TextRenderer::DestroyTextTexture(scoreTexture);
        }
    }
}

void Level::handlePlayerInput(bool timeUp, const Uint8* keyboardState) {
    if (timeUp) return;

    // Player 1 Controls (WASD)
    if (keyboardState[SDL_SCANCODE_W] && maze.IsWalkable(maze.player1.x, maze.player1.y - 1)) 
        maze.player1.y--;
    if (keyboardState[SDL_SCANCODE_S] && maze.IsWalkable(maze.player1.x, maze.player1.y + 1)) 
        maze.player1.y++;
    if (keyboardState[SDL_SCANCODE_A] && maze.IsWalkable(maze.player1.x - 1, maze.player1.y)) 
        maze.player1.x--;
    if (keyboardState[SDL_SCANCODE_D] && maze.IsWalkable(maze.player1.x + 1, maze.player1.y)) 
        maze.player1.x++;

    // Player 2 Controls (Arrow Keys)
    if (keyboardState[SDL_SCANCODE_UP] && maze.IsWalkable(maze.player2.x, maze.player2.y - 1)) 
        maze.player2.y--;
    if (keyboardState[SDL_SCANCODE_DOWN] && maze.IsWalkable(maze.player2.x, maze.player2.y + 1)) 
        maze.player2.y++;
    if (keyboardState[SDL_SCANCODE_LEFT] && maze.IsWalkable(maze.player2.x - 1, maze.player2.y)) 
        maze.player2.x--;
    if (keyboardState[SDL_SCANCODE_RIGHT] && maze.IsWalkable(maze.player2.x + 1, maze.player2.y)) 
        maze.player2.x++;
}

int Level::getLevel() const {
    return level;
}

int Level::getPlayer1Score() const {
    return player1Score;
}

int Level::getPlayer2Score() const {
    return player2Score;
}

Uint32 Level::getElapsedTime() const {
    return SDL_GetTicks() - levelStartTime;
}

Uint32 Level::getTimeLimit() const {
    return timeLimit;
}

bool Level::isTimeUp() const {
    return getElapsedTime() > timeLimit;
}

bool Level::player1WinsLevel() const {
    return maze.player1.x == maze.endX && maze.player1.y == maze.endY;
}

bool Level::player2WinsLevel() const {
    return maze.player2.x == maze.endX && maze.player2.y == maze.endY;
}

void Level::nextLevel() {
    level++;
    resetLevel();
}

void Level::resetTimer() {
    levelStartTime = SDL_GetTicks();
}

void Level::resetLevel() {
    maze = Maze(level);
    resetTimer();
}

bool Level::checkWinCondition() {
    return player1WinsLevel() || player2WinsLevel();
}

void Level::updateScores() {
    if (player1WinsLevel()) player1Score++;
    if (player2WinsLevel()) player2Score++;
}

Maze& Level::getMaze() {
    return maze;
}