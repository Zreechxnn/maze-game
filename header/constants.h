#pragma once

#include <cstdint>

constexpr int SCREEN_WIDTH = 600;
constexpr int SCREEN_HEIGHT = 600;
constexpr int PANEL_HEIGHT = 275;
constexpr int WINDOW_HEIGHT = SCREEN_HEIGHT + PANEL_HEIGHT;

constexpr int MAZE_WIDTH = 25;
constexpr int MAZE_HEIGHT = MAZE_WIDTH;
constexpr int TILE_SIZE = SCREEN_WIDTH / MAZE_WIDTH;

constexpr int INITIAL_LEVEL_SIZE = 20;
constexpr int LEVEL_INCREASE = 5;

constexpr int TIME_LIMIT = 40;

constexpr int PLAYER_COUNT = 2;
constexpr int DOOR_DISTANCE_FACTOR = 1;