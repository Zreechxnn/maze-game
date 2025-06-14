#ifndef LEVEL_H
#define LEVEL_H

#include "maze.h"
#include "text_renderer.h"
#include <SDL2/SDL.h>
#include <string>

class Level {
public:
    Level(int startLevel = 1);
    
    void update(bool timeUp);
    void render(SDL_Renderer* renderer, 
                SDL_Texture* wallTexture, 
                SDL_Texture* floorTexture, 
                SDL_Texture* doorTexture,
                SDL_Texture* player1Texture,
                SDL_Texture* player2Texture,
                TTF_Font* font);
    
    void handlePlayerInput(bool timeUp, const Uint8* keyboardState);
    
    int getLevel() const;
    int getPlayer1Score() const;
    int getPlayer2Score() const;
    Uint32 getElapsedTime() const;
    Uint32 getTimeLimit() const;
    bool isTimeUp() const;
    
    bool player1WinsLevel() const;
    bool player2WinsLevel() const;
    void nextLevel();
    
    void resetTimer();
    void resetLevel();
    
    Maze& getMaze();

private:
    int level;
    int player1Score;
    int player2Score;
    Maze maze;
    Uint32 levelStartTime;
    const Uint32 timeLimit;
    bool timeUp;
    
    bool checkWinCondition();
    void updateScores();
};

#endif