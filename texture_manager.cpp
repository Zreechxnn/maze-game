// texture_manager.cpp
#include "header/texture_manager.h"
#include <iostream>

SDL_Texture *TextureManager::LoadTexture(SDL_Renderer *renderer, const std::string &path)
{
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface)
    {
        std::cerr << "Failed to load image: " << path << "! IMG_Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        std::cerr << "Failed to create texture: " << path << "! SDL_Error: " << SDL_GetError() << std::endl;
    }
    return texture;
}

void TextureManager::DestroyTexture(SDL_Texture *texture)
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}