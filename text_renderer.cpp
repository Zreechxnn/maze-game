#include "header/text_renderer.h"
#include <iostream>

SDL_Texture* TextRenderer::RenderText(SDL_Renderer* renderer, TTF_Font* font,
                                      const std::string& text, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface)
    {
        SDL_Log("Text rendering failed: %s", TTF_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        SDL_Log("Texture creation failed: %s", SDL_GetError());
    }
    return texture;
}

void TextRenderer::DestroyTextTexture(SDL_Texture* texture)
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}