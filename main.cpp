// main.cpp
#include "header/maze.h"
#include "header/texture_manager.h"
#include "header/text_renderer.h"
#include "header/constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1)
    {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Maze Game", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Load assets
    SDL_Texture *wallTexture = TextureManager::LoadTexture(renderer, "/assets/images/wall.png");
    SDL_Texture *floorTexture = TextureManager::LoadTexture(renderer, "/assets/images/floor.png");
    SDL_Texture *playerTexture = TextureManager::LoadTexture(renderer, "/assets/images/player.png");
    SDL_Texture *doorTexture = TextureManager::LoadTexture(renderer, "/assets/images/door.png");

    TTF_Font *font = TTF_OpenFont("/assets/fonts/lonely.ttf", 24);
    if (!font)
    {
        std::cerr << "Font loading failed: " << TTF_GetError() << std::endl;
    }

    SDL_Color textColor = {255, 255, 255, 255};

    // Game state
    int level = 1;
    Maze maze(level);
    bool running = true;
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    Uint32 levelStartTime = SDL_GetTicks();
    const Uint32 timeLimit = TIME_LIMIT * 1000;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - levelStartTime;
        if (elapsedTime > timeLimit)
        {
            std::cout << "Time's up! Game Over!" << std::endl;
            running = false;
        }

        // Handle player movement
        if (keyboardState[SDL_SCANCODE_UP] && maze.IsWalkable(maze.player.x, maze.player.y - 1))
        {
            maze.player.y--;
        }
        if (keyboardState[SDL_SCANCODE_DOWN] && maze.IsWalkable(maze.player.x, maze.player.y + 1))
        {
            maze.player.y++;
        }
        if (keyboardState[SDL_SCANCODE_LEFT] && maze.IsWalkable(maze.player.x - 1, maze.player.y))
        {
            maze.player.x--;
        }
        if (keyboardState[SDL_SCANCODE_RIGHT] && maze.IsWalkable(maze.player.x + 1, maze.player.y))
        {
            maze.player.x++;
        }

        if (maze.player.x == maze.endX && maze.player.y == maze.endY)
        {
            std::cout << "Level " << level << " completed! Moving to level " << level + 1 << std::endl;
            level++;
            maze = Maze(level);
            levelStartTime = SDL_GetTicks();
        }

        SDL_RenderClear(renderer);

        for (int x = 0; x < MAZE_WIDTH; ++x)
        {
            for (int y = 0; y < MAZE_HEIGHT; ++y)
            {
                SDL_Rect tileRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_Texture *texture = (maze.grid[x][y] == 1) ? wallTexture : floorTexture;
                SDL_RenderCopy(renderer, texture, nullptr, &tileRect);
            }
        }

        SDL_Rect doorRect = {maze.endX * TILE_SIZE, maze.endY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, doorTexture, nullptr, &doorRect);

        SDL_Rect playerRect = {maze.player.x * TILE_SIZE, maze.player.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        double rotation = 0;
        if (keyboardState[SDL_SCANCODE_UP])
            rotation = 90;
        else if (keyboardState[SDL_SCANCODE_DOWN])
            rotation = -90;
        else if (keyboardState[SDL_SCANCODE_LEFT])
            rotation = 180;
        else if (keyboardState[SDL_SCANCODE_RIGHT])
            rotation = 0;

        SDL_RenderCopyEx(renderer, playerTexture, nullptr, &playerRect, rotation, nullptr, SDL_FLIP_NONE);

        // Render UI text
        if (font)
        {
            // Level indicator
            std::string levelText = "Level: " + std::to_string(level);
            SDL_Texture *levelTexture = TextRenderer::RenderText(renderer, font, levelText, textColor);
            if (levelTexture)
            {
                SDL_Rect levelRect = {10, 10, 100, 30};
                SDL_RenderCopy(renderer, levelTexture, nullptr, &levelRect);
                TextRenderer::DestroyTextTexture(levelTexture);
            }

            // Timer
            std::string timerText = "Time: " + std::to_string((timeLimit - elapsedTime) / 1000);
            SDL_Texture *timerTexture = TextRenderer::RenderText(renderer, font, timerText, textColor);
            if (timerTexture)
            {
                SDL_Rect timerRect = {10, 50, 100, 30};
                SDL_RenderCopy(renderer, timerTexture, nullptr, &timerRect);
                TextRenderer::DestroyTextTexture(timerTexture);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(120);
    }

    // Cleanup
    TextureManager::DestroyTexture(wallTexture);
    TextureManager::DestroyTexture(floorTexture);
    TextureManager::DestroyTexture(playerTexture);
    TextureManager::DestroyTexture(doorTexture);

    if (font)
        TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}