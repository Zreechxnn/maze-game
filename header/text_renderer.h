#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class TextRenderer
{
public:
    static SDL_Texture *RenderText(SDL_Renderer *renderer, TTF_Font *font,
                                   const std::string &text, SDL_Color color);
    static void DestroyTextTexture(SDL_Texture *texture);
};