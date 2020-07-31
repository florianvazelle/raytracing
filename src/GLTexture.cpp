#include "GLTexture.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <stdexcept>

#include "stb_image_write.h"

GLTexture::GLTexture(GLTexture&& other) noexcept {
  mTextureId = other.mTextureId;
  textureData = std::move(other.textureData);
  width = other.width;
  height = other.height;

  other.mTextureId = 0;
  other.textureData.clear();
  other.width = 0;
  other.height = 0;
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept {
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
  if (mTextureId) glDeleteTextures(1, &mTextureId);
}

const char* GLTexture::get_filename_ext(const char* filename) {
  const char* dot = strrchr(filename, '.');
  if (!dot || dot == filename) return "";
  return dot + 1;
}

void GLTexture::save(char const* filename) {
  uint8_t pixels[size() * 4];
  for (int i = 0; i < size(); ++i) {
    int j = 4 * i;
    pixels[j + 0] = textureData[i].r * 255.0f;
    pixels[j + 1] = textureData[i].g * 255.0f;
    pixels[j + 2] = textureData[i].b * 255.0f;
    pixels[j + 3] = 255.0f;
  }

  const char* extension = get_filename_ext(filename);
  if (strcmp(extension, "png")) {
    stbi_write_png(filename, width, height, 4, pixels, 4 * width);
  } else if (strcmp(extension, "jpg")) {
    stbi_write_jpg(filename, width, height, 4, pixels, 100);
  }
}