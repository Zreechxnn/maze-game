#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <algorithm>
#include <random>
#include <stack>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int MAZE_WIDTH = 25; 
const int TILE_SIZE = SCREEN_WIDTH / MAZE_WIDTH;
const int MAZE_HEIGHT = MAZE_WIDTH; 
const int INITIAL_LEVEL_SIZE = 20;
const int LEVEL_INCREASE = 5;
const int TIME_LIMIT = 25; 
const int DOOR_DISTANCE_FACTOR = 1;

struct Player {
    int x, y;
};

struct Maze {
    std::vector<std::vector<int>> grid;
    Player player;
    int startX, startY, endX, endY;

    Maze(int level) : grid(MAZE_WIDTH, std::vector<int>(MAZE_HEIGHT, 1)) {
        int maze_size = INITIAL_LEVEL_SIZE + level * LEVEL_INCREASE;
        generateMaze(maze_size, level);
        player.x = startX;
        player.y = startY;
    }

    void generateMaze(int maze_size, int level) {
        maze_size = std::min(maze_size, MAZE_WIDTH);
        maze_size = std::min(maze_size, MAZE_HEIGHT);

        for (int x = 0; x < maze_size; ++x) {
            for (int y = 0; y < maze_size; ++y) {
                grid[x][y] = 1;
            }
        }

        startX = 1;
        startY = 1;

        // Randomize endX and endY positions after every 10 levels
        if (level % 10 == 0) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(2, maze_size - 2);

            endX = dis(gen);
            endY = dis(gen);
        } else {
            // Keep the door in a fixed position for other levels
            endX = std::min(startX + (level * TILE_SIZE / 10), maze_size - 2);
            endY = std::min(startY + (level * TILE_SIZE / 10), maze_size - 2);
        }

        // Ensure the end position is not too close to the start position
        if (endX <= startX) endX = startX + 2;
        if (endY <= startY) endY = startY + 2;

        std::stack<std::pair<int, int>> stack;
        stack.push({startX, startY});
        grid[startX][startY] = 0;

        std::default_random_engine rng(static_cast<unsigned>(std::time(nullptr)));
        std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        while (!stack.empty()) {
            int x = stack.top().first;
            int y = stack.top().second;
            stack.pop();

            std::vector<std::pair<int, int>> validDirections;
            for (auto& dir : directions) {
                int nx = x + dir.first * 2;
                int ny = y + dir.second * 2;
                if (nx > 0 && ny > 0 && nx < maze_size - 1 && ny < maze_size - 1 && grid[nx][ny] == 1) {
                    validDirections.push_back(dir);
                }
            }

            if (!validDirections.empty()) {
                stack.push({x, y});
                std::shuffle(validDirections.begin(), validDirections.end(), rng);
                auto dir = validDirections.front();
                int nx = x + dir.first * 2;
                int ny = y + dir.second * 2;
                grid[x + dir.first][y + dir.second] = 0;
                grid[nx][ny] = 0;
                stack.push({nx, ny});
            }
        }

        grid[endX][endY] = 0;
    }

    bool isWalkable(int x, int y) const {
        return x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT && grid[x][y] == 0;
    }
};

SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image at " << path << "! IMG_Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Failed to create texture from " << path << "! SDL_Error: " << SDL_GetError() << std::endl;
    }
    return texture;
}

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to render text! TTF_Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Failed to create texture from text! SDL_Error: " << SDL_GetError() << std::endl;
    }
    return texture;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Maze Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* wallTexture = loadTexture(renderer, "assets/images/wall.png");
    SDL_Texture* floorTexture = loadTexture(renderer, "assets/images/floor.png");
    SDL_Texture* playerTexture = loadTexture(renderer, "assets/images/player.png");
    SDL_Texture* doorTexture = loadTexture(renderer, "assets/images/door.png");

    if (!wallTexture || !floorTexture || !playerTexture || !doorTexture) {
        SDL_DestroyTexture(wallTexture);
        SDL_DestroyTexture(floorTexture);
        SDL_DestroyTexture(playerTexture);
        SDL_DestroyTexture(doorTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("assets/fonts/lonely.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyTexture(wallTexture);
        SDL_DestroyTexture(floorTexture);
        SDL_DestroyTexture(playerTexture);
        SDL_DestroyTexture(doorTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Color textColor = {255, 255, 255, 255};

    int level = 1;
    Maze maze(level);
    bool running = true;
    SDL_Event event;
    const Uint8* state = SDL_GetKeyboardState(nullptr);

    Uint32 startTime = SDL_GetTicks();  // Waktu mulai level
    const Uint32 timeLimit = TIME_LIMIT * 1000;  // Batas waktu dalam milidetik

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;

        if (elapsedTime > timeLimit) {
            std::cout << "Game Over! Time's up!" << std::endl;
            running = false;
        }

        if (state[SDL_SCANCODE_UP] && maze.isWalkable(maze.player.x, maze.player.y - 1)) {
            maze.player.y--;
        }
        if (state[SDL_SCANCODE_DOWN] && maze.isWalkable(maze.player.x, maze.player.y + 1)) {
            maze.player.y++;
        }
        if (state[SDL_SCANCODE_LEFT] && maze.isWalkable(maze.player.x - 1, maze.player.y)) {
            maze.player.x--;
        }
        if (state[SDL_SCANCODE_RIGHT] && maze.isWalkable(maze.player.x + 1, maze.player.y)) {
            maze.player.x++;
        }

        if (maze.player.x == maze.endX && maze.player.y == maze.endY) {
            std::cout << "Congratulations! You have successfully reached level " << level + 1 << "!" << std::endl;
            level++;
            maze = Maze(level);
            startTime = SDL_GetTicks();
        }


        SDL_RenderClear(renderer);

        for (int x = 0; x < MAZE_WIDTH; ++x) {
            for (int y = 0; y < MAZE_HEIGHT; ++y) {
                SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_Texture* texture = (maze.grid[x][y] == 1) ? wallTexture : floorTexture;
                SDL_RenderCopy(renderer, texture, nullptr, &rect);
            }
        }

        SDL_Rect doorRect = {maze.endX * TILE_SIZE, maze.endY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, doorTexture, nullptr, &doorRect);

       SDL_Rect playerRect = {maze.player.x * TILE_SIZE, maze.player.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        double rotationAngle = 0;

        // Tentukan sudut rotasi berdasarkan arah gerakan
        if (state[SDL_SCANCODE_UP]) {
            rotationAngle = 90;
        } else if (state[SDL_SCANCODE_DOWN]) {
            rotationAngle = -90;
        } else if (state[SDL_SCANCODE_LEFT]) {
            rotationAngle = 360;
        } else if (state[SDL_SCANCODE_RIGHT]) {
            rotationAngle = 0;  // Default arah kanan
        }

        SDL_RenderCopyEx(renderer, playerTexture, nullptr, &playerRect, rotationAngle, nullptr, SDL_FLIP_NONE);

        // Render level text
        std::string levelText = "Level: " + std::to_string(level);
        SDL_Texture* levelTexture = renderText(renderer, font, levelText, textColor);
        if (levelTexture) {
            SDL_Rect textRect = {10, 10, 100, 30};
            SDL_RenderCopy(renderer, levelTexture, nullptr, &textRect);
            SDL_DestroyTexture(levelTexture);
        }

        // Render timer text
        std::string timerText = "Time: " + std::to_string((timeLimit - elapsedTime) / 1000);
        SDL_Texture* timerTexture = renderText(renderer, font, timerText, textColor);
        if (timerTexture) {
            SDL_Rect timerRect = {10, 50, 100, 30};
            SDL_RenderCopy(renderer, timerTexture, nullptr, &timerRect);
            SDL_DestroyTexture(timerTexture);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(120);  // Adjust delay as needed for game speed
    }

    TTF_CloseFont(font);
    SDL_DestroyTexture(wallTexture);
    SDL_DestroyTexture(floorTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(doorTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}