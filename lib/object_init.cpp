#include "object_init.h"

SDL_Surface *InitSurface(const char *file) {
  SDL_Surface *surface = IMG_Load(file);
  if (surface == nullptr) {
    printf("failed to init %s\n", file);
    return nullptr;
  }

  return surface;
}

SDL_Texture *InitTexture(SDL_Renderer *renderer, SDL_Surface *surface) {
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  if (texture == nullptr) {
    printf("failed to init texture\n");
    return nullptr;
  }
  return texture;
}

SDL_Texture *CreateGraphicTexture(SDL_Renderer *renderer, const char *file) {
  SDL_Surface *surface = InitSurface(file);
  SDL_Texture *texture = InitTexture(renderer, surface);
  return texture;
}

SDL_Texture *CreateFontTexture(SDL_Renderer *renderer, TTF_Font *font,
                               const char *text, SDL_Color color) {
  SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
  SDL_Texture *texture = InitTexture(renderer, surface);
  return texture;
}
