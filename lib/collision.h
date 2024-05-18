#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

void CheckSpriteBorders(int maxHeight, int maxWidth, int *currentY,
                        int *currentX, int spriteHeight, int spriteWidth);
