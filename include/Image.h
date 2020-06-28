#ifndef H_IMAGE
#define H_IMAGE

#include <array>
#include <iostream>
#include <rtx/Vector.h>
#include <string>
#include <vector>

class Image {
public:
  Image(char const *filename);
  Image(int w, int h);

  auto operator[](int r) { return _pixels.data() + r * w(); }
  auto operator[](int r) const { return _pixels.data() + r * w(); }

  void save_png(char const *filename);
  auto pixels() { return _pixels.data(); }
  auto pixels() const { return _pixels.data(); }
  int w() const { return _w; }
  int h() const { return _h; }

  class View {
  public:
    auto operator[](int r) { return image.pixels() + (y + r) * image.w() + x; }

    int x;
    int y;
    int w;
    int h;
    Image &image;
  };

  auto view(int x, int y, int w, int h) { return View{x, y, w, h, *this}; }

private:
  std::vector<rtx::Color> _pixels;
  int _w;
  int _h;
};

#endif