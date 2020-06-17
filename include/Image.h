#ifndef H_IMAGE
#define H_IMAGE

#include <vector>

#include <array>
#include <iostream>
#include <string>
#include <vector>

class Image {
public:
  Image(char const *filename, const std::string &type);
  Image(int w, int h);

  auto operator[](int r) { return _pixels.data() + r * w(); }
  auto operator[](int r) const { return _pixels.data() + r * w(); }

  void save_png(char const *filename);
  auto pixels() { return _pixels.data(); }
  auto pixels() const { return _pixels.data(); }
  int w() const { return _w; }
  int h() const { return _h; }

private:
  std::vector<std::array<float, 3>> _pixels;
  int _w;
  int _h;
};

#endif