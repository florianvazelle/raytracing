#include "Image.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>

Image::Image(char const *filename, const std::string &type) {
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
      _pixels[i][0] = pixels[j + 1];
      _pixels[i][1] = pixels[j + 2];
      _pixels[i][2] = pixels[j + 3];
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

void Image::save_png(char const *filename) {
  SDL_Surface *out_surface;
  out_surface = SDL_CreateRGBSurface(0, _w, _h, 32, 0, 0, 0, 0);
  out_surface =
      SDL_ConvertSurfaceFormat(out_surface, SDL_PIXELFORMAT_RGBA8888, 0);

  SDL_LockSurface(out_surface);
  auto pixels = static_cast<uint8_t *>(out_surface->pixels);
  for (int i = 0; i < _h * _w; ++i) {
    int j = 4 * i;
    pixels[j] = 255.0f;
    pixels[j + 1] = _pixels[i][0];
    pixels[j + 2] = _pixels[i][1];
    pixels[j + 3] = _pixels[i][2];
  }
  SDL_UnlockSurface(out_surface);

  IMG_SavePNG(out_surface, filename);
  SDL_FreeSurface(out_surface);
}