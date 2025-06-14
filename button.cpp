#include "header/button.h"
#include <SDL2/SDL.h>

Button::Button(int x, int y, int width, int height, std::function<void()> onClick)
    : onClick(onClick) {
    rect = {x, y, width, height};
}

void Button::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 80, 80, 100, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 180, 180, 200, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void Button::handleEvent(SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x;
        int y = event->button.y;
        
        if (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h) {
            if (onClick) {
                onClick();
            }
        }
    }
}

void Button::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
}

void Button::setSize(int width, int height) {
    rect.w = width;
    rect.h = height;
}

void Button::setOnClick(std::function<void()> onClick) {
    this->onClick = onClick;
}