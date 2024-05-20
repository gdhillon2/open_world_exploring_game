#pragma once

#include "SDL_ttf.h"
#include <SDL.h>
#include <SDL_image.h>

const SDL_Color black = {0, 0, 0, 0};
const SDL_Color red = {255, 0, 0, 0};

SDL_Texture *CreateGraphicTexture(SDL_Renderer *renderer, const char *file);
SDL_Texture *CreateFontTexture(SDL_Renderer *renderer, TTF_Font *font,
                               const char *text, SDL_Color color);
