#include "main.h"
#include "lib/collision.h"
#include "lib/player_character.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <filesystem>
#include <iostream>
#include <unistd.h>

int main() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Initialize SDL_image
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  // Initialize SDL_ttf
  if (TTF_Init() == -1) {
    std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  // movement speeds
  bool sprint_toggle = false;
  float walk = 0.2f;
  float sprint = 2 * walk;

  // Create window
  SDL_Window *window = SDL_CreateWindow(
      "moving rectangle game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      window_width, window_height, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  // Create renderer
  // THE COMMENTED OUT RENDERER RENDERS WITH VSYNC ENABLED
  //  SDL_Renderer *renderer = SDL_CreateRenderer(
  //      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  // Load font
  TTF_Font *font = TTF_OpenFont("fonts/Anonymous.ttf", 24);
  if (font == nullptr) {
    std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  // sprinting/walking text
  SDL_Color black = {0, 0, 0, 0};
  SDL_Color red = {255, 0, 0, 0};

  SDL_Surface *no_sprint_surface = TTF_RenderText_Solid(font, "walking", black);
  SDL_Surface *sprint_surface = TTF_RenderText_Solid(font, "sprinting", red);

  SDL_Texture *no_sprint_texture =
      SDL_CreateTextureFromSurface(renderer, no_sprint_surface);
  SDL_Texture *sprint_texture =
      SDL_CreateTextureFromSurface(renderer, sprint_surface);

  SDL_Rect text_rect = {0, 0, no_sprint_surface->w, no_sprint_surface->h};
  SDL_Texture *movement_type_texture = no_sprint_texture;

  SDL_FreeSurface(no_sprint_surface);
  SDL_FreeSurface(sprint_surface);

  // Load main character texture
  // SDL_Surface *main_char_surface = IMG_Load("sprites/main_char_border.png");
  SDL_Surface *main_char_surface = IMG_Load("sprites/example_sprite_sheet.jpg");
  if (main_char_surface == nullptr) {
    std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
    SDL_DestroyTexture(no_sprint_texture);
    SDL_DestroyTexture(sprint_texture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  SDL_Texture *main_char_texture =
      SDL_CreateTextureFromSurface(renderer, main_char_surface);
  SDL_FreeSurface(main_char_surface);

  InitializeCharacterRectangles(window_width, window_height);

  bool quit = false;
  SDL_Event event;

  Uint32 startTime = SDL_GetTicks();

  // animation variables
  Uint32 animation_frame_length = 100; // time in ms per frame
  bool moving = false;

  while (!quit) {
    // get the time for the initial start of the frame
    Uint32 currentTime = SDL_GetTicks();

    Uint32 elapsedTime = currentTime - startTime;

    startTime = currentTime;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      } else if (event.type == SDL_KEYUP) {
        // Toggle sprint with shift
        if (event.key.keysym.sym == SDLK_LSHIFT ||
            event.key.keysym.sym == SDLK_RSHIFT) {
          sprint_toggle = !sprint_toggle;
          movement_type_texture =
              sprint_toggle ? sprint_texture : no_sprint_texture;
          if (sprint_toggle) {
            animation_frame_length = 75;
          } else {
            animation_frame_length = 100;
          }
        }
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_d) {
          left_facing = false;
        }
        if (event.key.keysym.sym == SDLK_a) {
          left_facing = true;
        }
      }
    }

    // Closing game when escape is pressed
    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
    if (keystate[SDL_SCANCODE_ESCAPE]) {
      quit = true;
    }

    float currentSprint = sprint * elapsedTime;
    float currentWalk = walk * elapsedTime;

    // Main character movement
    moving = false;
    if (keystate[SDL_SCANCODE_W] && main_char_rect.y > 0) {
      main_char_rect.y -= sprint_toggle ? currentSprint : currentWalk;
      moving = true;
    }
    if (keystate[SDL_SCANCODE_A] && main_char_rect.x > 0) {
      main_char_rect.x -= sprint_toggle ? currentSprint : currentWalk;
      moving = true;
    }
    if (keystate[SDL_SCANCODE_S] &&
        main_char_rect.y < window_height - main_char_rect.h) {
      main_char_rect.y += sprint_toggle ? currentSprint : currentWalk;
      moving = true;
    }
    if (keystate[SDL_SCANCODE_D] &&
        main_char_rect.x < window_width - main_char_rect.w) {
      main_char_rect.x += sprint_toggle ? currentSprint : currentWalk;
      moving = true;
    }

    CheckSpriteBorders(window_height, window_width, &main_char_rect.y,
                       &main_char_rect.x, main_char_rect.h, main_char_rect.w);

    AnimateCharacter(currentTime, animation_frame_length, moving);

    // Clear the window
    SDL_SetRenderDrawColor(renderer, 235, 235, 223, 255);
    SDL_RenderClear(renderer);

    // Render main character
    SDL_RenderCopyEx(renderer, main_char_texture, &src_rect, &main_char_rect, 0,
                     nullptr,
                     left_facing ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

    // Render text
    SDL_RenderCopy(renderer, movement_type_texture, nullptr, &text_rect);

    // Present the back buffer
    SDL_RenderPresent(renderer);

    Uint32 frameDelay = SDL_GetTicks() - currentTime;
    // if the current frame took less time than the allotted frame time,
    // delay the next frame until the allotted time has occured
    if (frameDelay < frameTime) {
      SDL_Delay(frameTime - frameDelay);
    }

    // printf("x: %d\ty: %d\n", main_char_rect.x, main_char_rect.y);
    printf("fps: %.2f\n", 1000.f / (SDL_GetTicks() - startTime));
  }
  // Clean up
  SDL_DestroyTexture(main_char_texture);
  SDL_DestroyTexture(no_sprint_texture);
  SDL_DestroyTexture(sprint_texture);
  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
