#ifndef H_RAYTRACINGAPP
#define H_RAYTRACINGAPP

#include <cstdio>
#include <filesystem>
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
#include "Image.h"

namespace fs = std::filesystem; // g++ -v >= 9

class RaytracingApp : public nanogui::Screen {
public:
  RaytracingApp()
      : nanogui::Screen(Eigen::Vector2i(800, 600), "NanoGUI Test", true) {
    using namespace nanogui;

    /**
     * Pre-settings
     */

    // Convertie en image toute les scenes de assets/json/
    for (const fs::directory_entry &p :
         fs::directory_iterator("assets/json/")) {
      fs::path path = p.path();
      fs::path filename = path.filename();
      fs::path ext = path.extension();

      // On ne selectionne que des image
      if (ext == ".json") {
        std::string path_str = path.string();

        rtx::Scene scene = openScene(path_str);
        scenes.push_back(scene);

        Image img = raytracing(scene);

        char buff[100];
        sprintf(buff, "assets/png/%s.png", filename.c_str());
        img.save_png(buff);
      }
    }

    // On récupère toute les images
    std::vector<std::pair<int, std::string>> icons =
        loadImageDirectory(mNVGContext, "assets/png");

    // On les charges et les stock
    for (auto &icon : icons) {
      GLTexture texture(icon.second);
      auto data = texture.load(icon.second + ".png");
      mImagesData.emplace_back(std::move(texture), std::move(data));
    }

    /**
     * Image Window
     */

    // On crée une fenetre "image" pour afficher une image
    auto imageWindow = new Window(this, "Selected image");
    imageWindow->setPosition(Vector2i(250, 15));
    imageWindow->setLayout(new GroupLayout());

    // On définie l'image a affiché
    auto imageView = new ImageView(imageWindow, mImagesData[0].first.texture());
    imageView->setScaleCentered(0.5);
    imageView->setFixedSize(Vector2i(500, 500));
    mCurrentImage = 0;

    /**
     * Action Window
     */

    // On crée une fenetre "action" pour avoir l'ensemble des actions
    auto actionWindow = new Window(this, "Actions");
    actionWindow->setPosition(Vector2i(15, 15));
    actionWindow->setLayout(new GroupLayout());

    // Popup bouton pour selectionner une scene
    PopupButton *imagePanelBtn = new PopupButton(actionWindow, "Image Panel");
    imagePanelBtn->setIcon(ENTYPO_ICON_FOLDER);
    Popup *popup = imagePanelBtn->popup();
    VScrollPanel *vscroll = new VScrollPanel(popup);
    popup->setFixedSize(Vector2i(245, 150));
    ImagePanel *imgPanel = new ImagePanel(vscroll);
    imgPanel->setImages(icons);
    imgPanel->setCallback([this, imageView](int i) {
      imageView->bindImage(mImagesData[i].first.texture());
      mCurrentImage = i;
      std::cout << "Selected item " << i << '\n';
    });

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
      rtx::Scene scene = openScene(jsonPath);
      scenes.push_back(scene);

      Image img = raytracing(scene);
      img.save_png("assets/png/t2.png");

      GLTexture texture("assets/png/t2.png");
      auto data = texture.load("assets/png/t2.png");
      mImagesData.emplace_back(std::move(texture), std::move(data));

      mCurrentImage = mImagesData.size() - 1;
      imageView->bindImage(mImagesData.back().first.texture());
    });

    b = new Button(tools, "Save");
    b->setCallback([&] {
      std::cout << "File dialog result: "
                << file_dialog(
                       {
                           {"jpg", "JavaScript Object Notation"},
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

  rtx::Scene openScene(std::string);
  Image raytracing(rtx::Scene scene);

  rtx::Color getImpactColor(const rtx::Ray &ray, const rtx::Object &obj,
                            const rtx::Point &impact,
                            const rtx::Scene &scene) const;

private:
  using imagesDataType =
      std::vector<std::pair<GLTexture, GLTexture::handleType>>;
  imagesDataType mImagesData;
  int mCurrentImage;

  std::vector<rtx::Scene> scenes;
  int mCurrentScene;
};

#endif