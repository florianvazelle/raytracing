#ifndef H_GLTEXTURE
#define H_GLTEXTURE

#include <memory>
#include <nanogui/nanogui.h>
#include <string>

class GLTexture {
public:
  using handleType = std::unique_ptr<uint8_t[], void (*)(void *)>;

  GLTexture() = default;
  GLTexture(const std::string &textureName)
      : mTextureName(textureName), mTextureId(0) {}

  GLTexture(const std::string &textureName, GLint textureId)
      : mTextureName(textureName), mTextureId(textureId) {}

  GLTexture(const GLTexture &other) = delete;
  GLTexture(GLTexture &&other) noexcept;

  GLTexture &operator=(const GLTexture &other) = delete;
  GLTexture &operator=(GLTexture &&other) noexcept;
  ~GLTexture() noexcept;

  GLuint texture() const { return mTextureId; }
  const std::string &textureName() const { return mTextureName; }

  /**
   *  Load a file in memory and create an OpenGL texture.
   *  Returns a handle type (an std::unique_ptr) to the loaded pixels.
   */
  handleType load(const std::string &fileName);

private:
  std::string mTextureName;
  GLuint mTextureId;
};

#endif