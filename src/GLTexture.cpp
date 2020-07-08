#include "GLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

/**
 *  Load a file in memory and create an OpenGL texture.
 *  Returns a handle type (an std::unique_ptr) to the loaded pixels.
 */
void GLTexture::load(const std::string &fileName) {
  if (mTextureId) {
    glDeleteTextures(1, &mTextureId);
    mTextureId = 0;
  }
  int force_channels = 0;
  int n;
  uint8_t *data =
      stbi_load(fileName.c_str(), &width, &height, &n, force_channels);
  if (!data)
    throw std::invalid_argument("Could not load texture data from file " +
                                fileName);
  glGenTextures(1, &mTextureId);
  glBindTexture(GL_TEXTURE_2D, mTextureId);
  GLint internalFormat;
  GLint format;
  switch (n) {
  case 1:
    internalFormat = GL_R8;
    format = GL_RED;
    break;
  case 2:
    internalFormat = GL_RG8;
    format = GL_RG;
    break;
  case 3:
    internalFormat = GL_RGB8;
    format = GL_RGB;
    break;
  case 4:
    internalFormat = GL_RGBA8;
    format = GL_RGBA;
    break;
  default:
    internalFormat = 0;
    format = 0;
    break;
  }
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  textureData.resize(height * width);
  for (int i = 0; i < size(); ++i) {
    int j = 3 * i;
    textureData[i].b = data[j] / 255.0f;
    textureData[i].g = data[j + 1] / 255.0f;
    textureData[i].r = data[j + 2] / 255.0f;
  }

  stbi_image_free(data);
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