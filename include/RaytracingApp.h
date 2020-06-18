#ifndef H_RAYTRACINGAPP
#define H_RAYTRACINGAPP

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include <json/json.h>
#include <nanogui/nanogui.h>

#include <rtx/Camera.h>
#include <rtx/Cube.h>
#include <rtx/Object.h>
#include <rtx/Scene.h>
#include <rtx/Vector.h>

#include "GLTexture.h"

class RaytracingApp : public nanogui::Screen {
public:
  RaytracingApp()
      : nanogui::Screen(Eigen::Vector2i(800, 600), "NanoGUI Test", true) {
    using namespace nanogui;

    // On crée une fenetre "action"
    auto actionWindow = new Window(this, "Actions");
    actionWindow->setPosition(Vector2i(15, 15));
    actionWindow->setLayout(new GroupLayout());

    std::vector<std::pair<int, std::string>> icons =
        loadImageDirectory(mNVGContext, "assets/png");
    std::string resourcesFolderPath("assets/png/");

    // On crée une fenetre "image"
    auto imageWindow = new Window(this, "Selected image");
    imageWindow->setPosition(Vector2i(75, 75));
    imageWindow->setLayout(new GroupLayout());

    // Load all of the images by creating a GLTexture object and saving the
    // pixel data.
    for (auto &icon : icons) {
      GLTexture texture(icon.second);
      auto data = texture.load(icon.second + ".png");
      mImagesData.emplace_back(std::move(texture), std::move(data));
    }

    // Set the first texture
    auto imageView = new ImageView(imageWindow, mImagesData[0].first.texture());
    imageView->setSize(Vector2i(75, 75));

    /* Actions */
    Widget *tools = new Widget(actionWindow);
    tools->setLayout(
        new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 5));

    new CheckBox(actionWindow, "Multithreading");
    Button *b = new Button(tools, "Lance les rayons !");
    b->setCallback([this]() {});

    /* Import & export file */

    new Label(actionWindow, "File dialog", "sans-bold");
    tools = new Widget(actionWindow);
    tools->setLayout(
        new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 6));
    b = new Button(tools, "Open");
    b->setCallback([this, imageView] {
      std::string jsonPath;
      jsonPath = file_dialog(
          {
              {"json", "JavaScript Object Notation"},
          },
          true);
      std::cout << "File dialog result: " << jsonPath << std::endl;
      std::string pngPath = this->openScene(jsonPath);

      GLTexture texture(pngPath);
      auto data = texture.load(pngPath);
      mImagesData.emplace_back(std::move(texture), std::move(data));

      imageView->bindImage(mImagesData.back().first.texture());
    });

    b = new Button(tools, "Save");
    b->setCallback([&] {
      std::cout << "File dialog result: "
                << file_dialog(
                       {
                           {"json", "JavaScript Object Notation"},
                       },
                       true)
                << std::endl;
    });

    performLayout();
  }

  virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) {
    if (Screen::keyboardEvent(key, scancode, action, modifiers))
      return true;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      setVisible(false);
      return true;
    }
    return false;
  }

  virtual void draw(NVGcontext *ctx) {
    /* Draw the user interface */
    Screen::draw(ctx);
  }

  std::string openScene(std::string);
  std::string raytracing(rtx::Scene scene);

  rtx::Color getImpactColor(const rtx::Ray &ray, const rtx::Object &obj,
                            const rtx::Point &impact,
                            const rtx::Scene &scene) const;

private:
  using imagesDataType =
      std::vector<std::pair<GLTexture, GLTexture::handleType>>;
  imagesDataType mImagesData;

  std::vector<rtx::Scene> scenes;
};

#endif