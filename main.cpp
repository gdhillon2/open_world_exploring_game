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

  // window dimensions
  int window_height = 880;
  int window_width = 1470;

  // movement speeds
  bool sprint_toggle = false;
  float walk = 2.0f;
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
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  // Load font
  TTF_Font *font = TTF_OpenFont("Anonymous.ttf", 24);
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
  SDL_Color white = {255, 255, 255, 0};
  SDL_Color red = {255, 0, 0, 0};

  SDL_Surface *no_sprint_surface = TTF_RenderText_Solid(font, "walking", white);
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
  SDL_Surface *main_char_surface = IMG_Load("sprites/main_char_border.png");
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

  SDL_Rect main_char_rect;
  SDL_QueryTexture(main_char_texture, nullptr, nullptr, &main_char_rect.w,
                   &main_char_rect.h);

  float main_char_scale = 0.3f;
  main_char_rect.w = static_cast<int>(main_char_rect.w * main_char_scale);
  main_char_rect.h = static_cast<int>(main_char_rect.h * main_char_scale);
  main_char_rect.x = window_width / 2 - main_char_rect.w / 2;
  main_char_rect.y = window_height / 2 - main_char_rect.h / 2;

  // Main character movement directions
  bool left_facing = true;

  bool quit = false;
  SDL_Event event;

  while (!quit) {
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
        }
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_d) {
          left_facing = true;
        }
        if (event.key.keysym.sym == SDLK_a) {
          left_facing = false;
        }
      }
    }

    // Closing game when escape is pressed
    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
    if (keystate[SDL_SCANCODE_ESCAPE]) {
      quit = true;
    }

    // Main character movement
    if (keystate[SDL_SCANCODE_W] && main_char_rect.y > 0) {
      main_char_rect.y -= sprint_toggle ? sprint : walk;
    }
    if (keystate[SDL_SCANCODE_A] && main_char_rect.x > 0) {
      main_char_rect.x -= sprint_toggle ? sprint : walk;
    }
    if (keystate[SDL_SCANCODE_S] &&
        main_char_rect.y < window_height - main_char_rect.h) {
      main_char_rect.y += sprint_toggle ? sprint : walk;
    }
    if (keystate[SDL_SCANCODE_D] &&
        main_char_rect.x < window_width - main_char_rect.w) {
      main_char_rect.x += sprint_toggle ? sprint : walk;
    }

    // Clear the window
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    // Render main character
    SDL_RenderCopyEx(renderer, main_char_texture, nullptr, &main_char_rect, 0,
                     nullptr,
                     left_facing ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

    // Render text
    SDL_RenderCopy(renderer, movement_type_texture, nullptr, &text_rect);

    // Present the back buffer
    SDL_RenderPresent(renderer);
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

