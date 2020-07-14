#include "GLTexture.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdexcept>

GLTexture::GLTexture(GLTexture &&other) noexcept {
  mTextureId = other.mTextureId;
  textureData = std::move(other.textureData);
  width = other.width;
  height = other.height;

  other.mTextureId = 0;
  other.textureData.clear();
  other.width = 0;
  other.height = 0;
}

GLTexture &GLTexture::operator=(GLTexture &&other) noexcept {
  mTextureId = other.mTextureId;
  textureData = std::move(other.textureData);
  width = other.width;
  height = other.height;

  other.mTextureId = 0;
  other.textureData.clear();
  other.width = 0;
  other.height = 0;
  return *this;
}

GLTexture::~GLTexture() noexcept {
  if (mTextureId)
    glDeleteTextures(1, &mTextureId);
}

const char *GLTexture::get_filename_ext(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return "";
  return dot + 1;
}

void GLTexture::save(char const *filename) {
  SDL_Surface *out_surface;
  out_surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
  out_surface =
      SDL_ConvertSurfaceFormat(out_surface, SDL_PIXELFORMAT_RGBA8888, 0);

  SDL_LockSurface(out_surface);
  auto pixels = static_cast<uint8_t *>(out_surface->pixels);
  for (int i = 0; i < size(); ++i) {
    int j = 4 * i;
    pixels[j] = 255.0f;
    pixels[j + 1] = textureData[i].b * 255.0f;
    pixels[j + 2] = textureData[i].g * 255.0f;
    pixels[j + 3] = textureData[i].r * 255.0f;
  }
  SDL_UnlockSurface(out_surface);

  const char *extension = get_filename_ext(filename);
  if (strcmp(extension, "png"))
    IMG_SavePNG(out_surface, filename);
  else if (strcmp(extension, "jpg"))
    IMG_SaveJPG(out_surface, filename, 100);

  SDL_FreeSurface(out_surface);
}