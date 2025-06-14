#pragma once

#include "constants.h"
#include <vector>
#include <stack>
#include <random>

struct Player
{
    int x = 0;
    int y = 0;
};

class Maze
{
public:
    std::vector<std::vector<int>> grid;

    Player player1;
    Player player2;

    int startX, startY;
    int endX, endY;

    Maze(int level);

    bool IsWalkable(int x, int y) const;

private:
    void InitializeGrid(int maze_size);
    void SetEndPosition(int maze_size, int level);
    void GenerateMazePath(int maze_size);
    void GenerateMaze(int maze_size, int level);
};