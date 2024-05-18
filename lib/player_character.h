#pragma once

#include <SDL.h>
#include <SDL_image.h>

extern SDL_Rect main_char_rect; // Declare main_char_rect as an extern variable
extern SDL_Rect src_rect;       // Declare src_rect as an extern variable
extern bool left_facing;        // Declare left_facing as an extern variable

void InitializeCharacterRectangles(int window_width, int window_height);
