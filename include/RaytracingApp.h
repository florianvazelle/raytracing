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
        mCurrentScene = scenes.size() - 1;

        Image img = raytracing(scene, (useMultithreading) ? 2 : 1);

        char buff[100];
        sprintf(buff, "assets/png/scene%d.json.png", mCurrentScene);
        img.save_png(buff);
      }
    }

    // On récupère toute les images
    std::vector<std::pair<int, std::string>> icons =
        loadImageDirectory(mNVGContext, "assets/png");

    std::sort(icons.begin(), icons.end(),
              [](const std::pair<int, std::string> &lhs,
                 const std::pair<int, std::string> &rhs) {
                return lhs.second < rhs.second;
              });
    icons.resize(scenes.size());

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
    ImageView *imageView =
        new ImageView(imageWindow, mImagesData[mCurrentScene].first.texture());
    imageView->setScaleCentered(0.5);
    imageView->setFixedSize(Vector2i(500, 500));

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
      mCurrentScene = i;
      std::cout << "Selected item " << i << '\n';
    });

    /* Actions */
    Widget *tools = new Widget(actionWindow);
    tools->setLayout(
        new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 5));

    CheckBox *c = new CheckBox(actionWindow, "Multithreading");
    c->setChecked(useMultithreading);
    c->setCallback([this](bool isChecked) { useMultithreading = isChecked; });

    /* Super sample slider */
    {
      new Label(actionWindow, "Sample per pixels", "sans-bold");
      tools = new Widget(actionWindow);
      tools->setLayout(
          new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 6));

      Slider *slider = new Slider(tools);
      slider->setValue(spp / 10);
      slider->setFixedWidth(80);

      TextBox *textBox = new TextBox(tools);
      textBox->setFixedSize(Vector2i(60, 25));
      textBox->setValue(std::to_string((int)spp));
      slider->setCallback([textBox](float value) {
        textBox->setValue(std::to_string((int)(value * 10)));
      });
      slider->setFinalCallback([this, imageView, imgPanel](float value) {
        spp = value * 10;

        updateView(scenes[mCurrentScene], imageView, imgPanel);
      });
      textBox->setFixedSize(Vector2i(60, 25));
      textBox->setFontSize(20);
      textBox->setAlignment(TextBox::Alignment::Right);
    }

    /* FOV slider */
    // {
    //   new Label(actionWindow, "Field of view", "sans-bold");
    //   tools = new Widget(actionWindow);
    //   tools->setLayout(
    //       new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 6));

    //   Slider *slider = new Slider(tools);
    //   slider->setValue(0.5f);
    //   slider->setFixedWidth(80);

    //   TextBox *textBox = new TextBox(tools);
    //   textBox->setFixedSize(Vector2i(60, 25));
    //   textBox->setValue("90");
    //   slider->setCallback([textBox](float value) {
    //     float i = value * 3;
    //     textBox->setValue(std::to_string((int)(60 + (i * 30))));
    //   });
    //   slider->setFinalCallback([this, imageView, imgPanel](float value) {
    //     float i = value * 3;
    //     fov = 60 + (i * 30);

    //     updateView(scenes[mCurrentScene], imageView, imgPanel);
    //   });
    //   textBox->setFixedSize(Vector2i(60, 25));
    //   textBox->setFontSize(20);
    //   textBox->setAlignment(TextBox::Alignment::Right);
    // }

    /* Positive integer widget */
    {
      new Label(actionWindow, "Width :", "sans-bold");
      auto intBox = new IntBox<int>(actionWindow);
      intBox->setEditable(true);
      intBox->setFixedSize(Vector2i(100, 20));
      intBox->setValue(_width);
      intBox->setDefaultValue(std::to_string(_width));
      intBox->setFontSize(16);
      intBox->setFormat("[1-9][0-9]*");
      intBox->setSpinnable(true);
      intBox->setMinValue(1);
      intBox->setCallback([this, imageView, imgPanel](float value) {
        _width = value;

        updateView(scenes[mCurrentScene], imageView, imgPanel);
      });

      new Label(actionWindow, "Height :", "sans-bold");
      intBox = new IntBox<int>(actionWindow);
      intBox->setEditable(true);
      intBox->setFixedSize(Vector2i(100, 20));
      intBox->setValue(_height);
      intBox->setDefaultValue(std::to_string(_height));
      intBox->setFontSize(16);
      intBox->setFormat("[1-9][0-9]*");
      intBox->setSpinnable(true);
      intBox->setMinValue(1);
      intBox->setCallback([this, imageView, imgPanel](float value) {
        _height = value;

        updateView(scenes[mCurrentScene], imageView, imgPanel);
      });
    }

    /* Import & export file */

    new Label(actionWindow, "File dialog", "sans-bold");
    tools = new Widget(actionWindow);
    tools->setLayout(
        new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 6));
    Button *b = new Button(tools, "Open");
    b->setCallback([this, imageView, imgPanel] {
      std::string jsonPath;
      jsonPath = file_dialog(
          {
              {"json", "JavaScript Object Notation"},
          },
          true);
      std::cout << "File dialog result: " << jsonPath << std::endl;
      if (jsonPath.size() > 1) {
        rtx::Scene scene = openScene(jsonPath);
        scenes.push_back(scene);
        mCurrentScene = scenes.size() - 1;

        updateView(scene, imageView, imgPanel);
      }
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

  void updateView(const rtx::Scene &scene, nanogui::ImageView *imageView,
                  nanogui::ImagePanel *imgPanel) {
    // Execute les lancers de rayon pour determiner l'image grace a la scene
    // passé en parametre
    Image img = raytracing(scene, (useMultithreading) ? 8 : 1);

    // Défini le nom du fichier
    char buff[100];
    sprintf(buff, "assets/png/scene%d.json.png", mCurrentScene);
    // Enregistre l'image en PNG
    img.save_png(buff);

    // Lit l'image en OpenGL et la stocke
    GLTexture texture(buff);
    auto data = texture.load(buff);
    mImagesData.emplace_back(std::move(texture), std::move(data));

    // On affiche l'image que l'on vient de calculer
    imageView->bindImage(mImagesData.back().first.texture());

    // On met a jour la popup des miniatures des images
    std::vector<std::pair<int, std::string>> icons =
        nanogui::loadImageDirectory(mNVGContext, "assets/png");

    std::sort(icons.begin(), icons.end(),
              [](const std::pair<int, std::string> &lhs,
                 const std::pair<int, std::string> &rhs) {
                return lhs.second < rhs.second;
              });
    icons.resize(scenes.size());

    imgPanel->setImages(icons);
  }

  rtx::Scene openScene(std::string);

  rtx::Color castRayForPixel(const rtx::Scene &scene, const rtx::Camera &cam,
                             float i, float j) const;
  void traceRays(Image::View view, const rtx::Scene &scene,
                 const rtx::Camera &cam) const;
  Image raytracing(const rtx::Scene &scene, int threadsCount) const;

  rtx::Color getImpactColor(const rtx::Ray &ray, const rtx::Object &obj,
                            const rtx::Point &impact,
                            const rtx::Scene &scene) const;

private:
  using imagesDataType =
      std::vector<std::pair<GLTexture, GLTexture::handleType>>;
  imagesDataType mImagesData;

  std::vector<rtx::Scene> scenes;
  int mCurrentScene;

  float _width = 800.0f;
  float _height = 800.0f;
  float fov = 90.0f;
  float spp = 3.f; // sample par pixels
  bool useMultithreading = true;
};

#endif