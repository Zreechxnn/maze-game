// texture_manager.h
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class TextureManager
{
public:
    static SDL_Texture *LoadTexture(SDL_Renderer *renderer, const std::string &path);
    static void DestroyTexture(SDL_Texture *texture);
};