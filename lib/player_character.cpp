#include "player_character.h"

// Define the variables declared in the header file
SDL_Rect main_char_rect;
SDL_Rect src_rect;
bool left_facing = false;

void InitializeCharacterRectangles(int window_width, int window_height) {
  // size of the rectangle on the screen
  main_char_rect.w = 148;
  main_char_rect.h = 194;

  // rectangle that defines the part of the texture that we want to draw
  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.w = 148;
  src_rect.h = 194;

  float main_char_scale = 0.3f;
  main_char_rect.w = static_cast<int>(main_char_rect.w * main_char_scale);
  main_char_rect.h = static_cast<int>(main_char_rect.h * main_char_scale);
  main_char_rect.x = window_width / 2 - main_char_rect.w / 2;
  main_char_rect.y = window_height / 2 - main_char_rect.h / 2;
}
