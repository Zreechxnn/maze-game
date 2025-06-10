// maze.cpp
#include "header/maze.h"
#include <algorithm>
#include <ctime>
#include <iostream>

Maze::Maze(int level) : grid(MAZE_WIDTH, std::vector<int>(MAZE_HEIGHT, 1))
{
    int maze_size = std::min(INITIAL_LEVEL_SIZE + level * LEVEL_INCREASE, MAZE_WIDTH);
    GenerateMaze(maze_size, level);
    player.x = startX;
    player.y = startY;
}

void Maze::InitializeGrid(int maze_size)
{
    for (int x = 0; x < maze_size; ++x)
    {
        for (int y = 0; y < maze_size; ++y)
        {
            grid[x][y] = 1;
        }
    }
}

void Maze::SetEndPosition(int maze_size, int level)
{
    startX = 1;
    startY = 1;

    if (level % 10 == 0)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(2, maze_size - 2);
        endX = dis(gen);
        endY = dis(gen);
    }
    else
    {
        endX = std::min(startX + (level * TILE_SIZE / 10), maze_size - 2);
        endY = std::min(startY + (level * TILE_SIZE / 10), maze_size - 2);
    }

    if (endX <= startX)
        endX = startX + 2;
    if (endY <= startY)
        endY = startY + 2;
}

void Maze::GenerateMazePath(int maze_size)
{
    std::stack<std::pair<int, int>> stack;
    stack.push({startX, startY});
    grid[startX][startY] = 0;

    std::default_random_engine rng(static_cast<unsigned>(std::time(nullptr)));
    std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    while (!stack.empty())
    {
        auto [x, y] = stack.top();
        stack.pop();

        std::vector<std::pair<int, int>> validDirections;
        for (const auto &[dx, dy] : directions)
        {
            int nx = x + dx * 2;
            int ny = y + dy * 2;
            if (nx > 0 && ny > 0 && nx < maze_size - 1 && ny < maze_size - 1 && grid[nx][ny] == 1)
            {
                validDirections.emplace_back(dx, dy);
            }
        }

        if (!validDirections.empty())
        {
            stack.push({x, y});
            std::shuffle(validDirections.begin(), validDirections.end(), rng);
            auto [dx, dy] = validDirections.front();
            int nx = x + dx * 2;
            int ny = y + dy * 2;
            grid[x + dx][y + dy] = 0;
            grid[nx][ny] = 0;
            stack.push({nx, ny});
        }
    }
}

void Maze::GenerateMaze(int maze_size, int level)
{
    InitializeGrid(maze_size);
    SetEndPosition(maze_size, level);
    GenerateMazePath(maze_size);
    grid[endX][endY] = 0;
}

bool Maze::IsWalkable(int x, int y) const
{
    return x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT && grid[x][y] == 0;
}