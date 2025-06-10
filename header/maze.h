// maze.h
#pragma once
#include "constants.h"
#include <vector>
#include <stack>
#include <random>

struct Player
{
    int x, y;
};

class Maze
{
public:
    std::vector<std::vector<int>> grid;
    Player player;
    int startX, startY, endX, endY;

    Maze(int level);
    void GenerateMaze(int maze_size, int level);
    bool IsWalkable(int x, int y) const;

private:
    void InitializeGrid(int maze_size);
    void SetEndPosition(int maze_size, int level);
    void GenerateMazePath(int maze_size);
};