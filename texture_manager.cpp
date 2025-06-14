#include "header/texture_manager.h"

SDL_Texture* TextureManager::LoadTexture(SDL_Renderer* renderer, const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        SDL_Log("Failed to load image: %s | IMG_Error: %s", path.c_str(), IMG_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        SDL_Log("Failed to create texture from %s | SDL_Error: %s", path.c_str(), SDL_GetError());
    }
    return texture;
}

void TextureManager::DestroyTexture(SDL_Texture* texture)
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}