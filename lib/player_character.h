#pragma once

#include <SDL.h>
#include <SDL_image.h>

extern SDL_Rect main_char_rect; // Declare main_char_rect as an extern variable
extern SDL_Rect src_rect;       // Declare src_rect as an extern variable
extern bool left_facing;        // Declare left_facing as an extern variable
extern bool moving;
extern int player_animation_frame_length; // length of animation frame in ms
void InitializeCharacterRectangles(int window_width, int window_height);
void AnimateCharacter(Uint32 currentTime, Uint32 animation_speed, bool moving);
