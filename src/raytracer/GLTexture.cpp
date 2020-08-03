#include "GLTexture.h"

#include <iostream>
#include <stdexcept>

#define STB_IMAGE_WRITE_IMPLEMENTATION
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

const char* GLTexture::get_filename_ext(const char* filename) const {
  const char* dot = strrchr(filename, '.');
  if (!dot || dot == filename) return "";
  return dot + 1;
}

void GLTexture::update() const {
  glBindTexture(GL_TEXTURE_2D, mTextureId);

  std::vector<uint8_t> data;
  auto pixels = textureData.data();
  for (int j = 0; j < size(); j++) {
    data.push_back(pixels[j].r * 255.0f);
    data.push_back(pixels[j].g * 255.0f);
    data.push_back(pixels[j].b * 255.0f);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::save(char const* filename) const {
  int s = size();
  uint8_t pixels[s * 4];
  for (int i = 0; i < s; ++i) {
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