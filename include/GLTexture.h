#ifndef H_GLTEXTURE
#define H_GLTEXTURE

#include <nanogui/nanogui.h>
#include <rtx/Vector.h>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

class GLTexture {
 public:
  GLTexture() = default;
  GLTexture(const int& w, const int& h) : mTextureId(0), width(w), height(h) {
    textureData.resize(w * h);
  }

  GLTexture(const GLTexture& other) = delete;
  GLTexture(GLTexture&& other) noexcept;

  GLTexture& operator=(const GLTexture& other) = delete;
  GLTexture& operator=(GLTexture&& other) noexcept;
  ~GLTexture() noexcept;

  GLuint texture() const { return mTextureId; }
  GLuint& texture() { return mTextureId; }

  /**
   *  Save a PNG or JPG image with SDL.
   */
  void save(char const* filename);

  auto operator[](int r) { return textureData.data() + r * w(); }
  auto operator[](int r) const { return textureData.data() + r * w(); }

  auto pixels() { return textureData.data(); }
  auto pixels() const { return textureData.data(); }

  int w() const { return width; }
  int h() const { return height; }

  void size(const int& w, const int& h) {
    width = w;
    height = h;
    textureData.resize(width * height);
  }
  int size() const { return width * height; }

  class View {
   public:
    auto operator[](int r) { return image.pixels() + (y + r) * image.w() + x; }

    int x;
    int y;
    int w;
    int h;
    GLTexture& image;
  };

  auto view(int x, int y, int w, int h) { return View{x, y, w, h, *this}; }

 private:
  GLuint mTextureId;
  std::vector<rtx::Color> textureData;
  int width, height;

  const char* get_filename_ext(const char* filename);
};

#endif