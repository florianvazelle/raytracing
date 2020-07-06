#include "Image.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdexcept>

Image::Image(char const *filename) {
  SDL_Surface *surface = IMG_Load(filename);
  if (surface) {
    _h = surface->h;
    _w = surface->w;
    _pixels.resize(_h * _w);

    SDL_Surface *in_surface =
        SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(surface);

    SDL_LockSurface(in_surface);
    auto pixels = static_cast<uint8_t *>(in_surface->pixels);
    for (int i = 0; i < _h * _w; ++i) {
      int j = 4 * i;
      _pixels[i].b = pixels[j + 1] / 255.0f;
      _pixels[i].g = pixels[j + 2] / 255.0f;
      _pixels[i].r = pixels[j + 3] / 255.0f;
    }
    SDL_UnlockSurface(in_surface);

    SDL_FreeSurface(in_surface);
  } else {
    throw std::runtime_error("Cannot load image");
  }
}

Image::Image(int w, int h) {
  _h = h;
  _w = w;
  _pixels.resize(w * h);
}

const char *Image::get_filename_ext(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return "";
  return dot + 1;
}

void Image::save(char const *filename) {
  SDL_Surface *out_surface;
  out_surface = SDL_CreateRGBSurface(0, _w, _h, 32, 0, 0, 0, 0);
  out_surface =
      SDL_ConvertSurfaceFormat(out_surface, SDL_PIXELFORMAT_RGBA8888, 0);

  SDL_LockSurface(out_surface);
  auto pixels = static_cast<uint8_t *>(out_surface->pixels);
  for (int i = 0; i < _h * _w; ++i) {
    int j = 4 * i;
    pixels[j] = 255.0f;
    pixels[j + 1] = _pixels[i].b * 255.0f;
    pixels[j + 2] = _pixels[i].g * 255.0f;
    pixels[j + 3] = _pixels[i].r * 255.0f;
  }
  SDL_UnlockSurface(out_surface);

  const char *extension = get_filename_ext(filename);
  if (strcmp(extension, "png"))
    IMG_SavePNG(out_surface, filename);
  else if (strcmp(extension, "jpg"))
    IMG_SaveJPG(out_surface, filename, 100);
  SDL_FreeSurface(out_surface);
}