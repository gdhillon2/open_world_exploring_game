#include "player_character.h"
// Define the variables declared in the header file
SDL_Rect main_char_rect;
SDL_Rect src_rect;
bool left_facing = false;
bool moving = false;
int player_animation_frame_length = 100;

// size of the rectangle shown on the screen
int char_w = 148;
int char_h = 194;

void InitializeCharacterRectangles(int window_width, int window_height) {
  // size of the rectangle on the screen
  main_char_rect.w = char_w;
  main_char_rect.h = char_h;

  // rectangle that defines the part of the texture that we want to draw
  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.w = char_w;
  src_rect.h = char_h;

  float main_char_scale = 0.4f;
  main_char_rect.w = static_cast<int>(main_char_rect.w * main_char_scale);
  main_char_rect.h = static_cast<int>(main_char_rect.h * main_char_scale);
  main_char_rect.x = window_width / 2 - main_char_rect.w / 2;
  main_char_rect.y = window_height / 2 - main_char_rect.h / 2;
}

void AnimateCharacter(Uint32 currentTime, Uint32 animation_speed, bool moving) {
  static int frame = 0;
  static int num_frames = 7;
  static int increasing = 1;
  static Uint32 last_frame_time = currentTime;

  if (currentTime - last_frame_time >= animation_speed) {
    if (moving) {
      if (increasing) {
        frame++;
        if (frame >= num_frames - 1) {
          frame = num_frames - 1;
          increasing = 0;
        }
      } else {
        frame--;
        if (frame <= 0) {
          frame = 0;
          increasing = 1;
        }
      }
    } else if (frame > 0) {
      frame--;
    }
    src_rect.x = frame * src_rect.w;
    last_frame_time = currentTime;
  }
}
