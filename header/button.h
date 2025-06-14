#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <functional>

class Button {
public:
    Button(int x, int y, int width, int height, std::function<void()> onClick);
    
    void render(SDL_Renderer* renderer);
    void handleEvent(SDL_Event* event);
    void setPosition(int x, int y);
    void setSize(int width, int height);
    void setOnClick(std::function<void()> onClick);

private:
    SDL_Rect rect;
    std::function<void()> onClick;
};

#endif