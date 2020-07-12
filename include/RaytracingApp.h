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

namespace fs = std::filesystem; // g++ -v >= 9

class RaytracingApp : public nanogui::Screen {
public:
  RaytracingApp()
      : nanogui::Screen(Eigen::Vector2i(800, 600), "NanoGUI Test", true) {
    using namespace nanogui;

    /**
     * Pre-settings
     */

    // Convertie en image toute les scenes de assets/scenes/
    for (const fs::directory_entry &p :
         fs::directory_iterator("assets/scenes/")) {
      fs::path path = p.path();
      fs::path filename = path.filename();
      fs::path ext = path.extension();

      // On ne selectionne que des image
      if (ext == ".json") {
        std::string path_str = path.string();

        rtx::Scene scene = openScene(path_str);
        scenes.push_back(scene);
        mCurrentScene = scenes.size() - 1;

        // On crée la texture
        GLTexture texture(_width, _height);
        glGenTextures(1, &(texture.texture()));

        updateTexture(scene, texture);

        char buff[100];
        sprintf(buff, "assets/samples/scene%d.png", mCurrentScene);
        texture.save(buff);

        mImagesData.push_back(std::move(texture));
      }
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
        new ImageView(imageWindow, mImagesData.at(mCurrentScene).texture());
    // imageView->setScaleCentered(0.5);
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
    imgPanel->setCallback([this, imageView](int i) {
      imageView->bindImage(mImagesData.at(i).texture());
      mCurrentScene = i;
      std::cout << "Selected item " << i << '\n';
    });

    updateIcons(imgPanel);

    /* Actions */
    Widget *tools = new Widget(actionWindow);
    tools->setLayout(
        new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 5));

    CheckBox *c = new CheckBox(actionWindow, "Multithreading");
    c->setChecked(useMultithreading);
    c->setCallback([this](bool isChecked) { useMultithreading = isChecked; });

    c = new CheckBox(actionWindow, "Shadow");
    c->setChecked(useShadow);
    c->setCallback([this](bool isChecked) { useShadow = isChecked; });

    c = new CheckBox(actionWindow, "Occlusion ambiante");
    c->setChecked(useAmbiantOcclusion);
    c->setCallback([this](bool isChecked) { useAmbiantOcclusion = isChecked; });

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
        int newSpp = (int)(value * 10);
        if (spp != newSpp) {
          spp = newSpp;
          updateView(scenes.at(mCurrentScene), imgPanel,
                     mImagesData.at(mCurrentScene));
          imageView->bindImage(mImagesData.at(mCurrentScene).texture());
        }
      });
      textBox->setFixedSize(Vector2i(60, 25));
      textBox->setFontSize(20);
      textBox->setAlignment(TextBox::Alignment::Right);
    }

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

        updateView(scenes.at(mCurrentScene), imgPanel,
                   mImagesData.at(mCurrentScene));
        imageView->bindImage(mImagesData.at(mCurrentScene).texture());
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

        updateView(scenes.at(mCurrentScene), imgPanel,
                   mImagesData.at(mCurrentScene));
        imageView->bindImage(mImagesData.at(mCurrentScene).texture());
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
          false);
      std::cout << "File dialog result: " << jsonPath << std::endl;
      if (jsonPath.size() > 1) {
        rtx::Scene scene = openScene(jsonPath);
        scenes.push_back(scene);
        mCurrentScene = scenes.size() - 1;

        // On crée la texture
        GLTexture texture(_width, _height);
        updateView(scene, imgPanel, texture);
        mImagesData.push_back(std::move(texture));
        imageView->bindImage(texture.texture());
      }
    });

    b = new Button(tools, "Save");
    b->setCallback([&] {
      std::string jpgPath = file_dialog(
          {
              {"jpg", "Joint Photographic Experts Group"},
          },
          true);

      std::cout << "File dialog result: " << jpgPath << std::endl;

      if (jpgPath.size() > 1) {
        char buff[255];
        sprintf(buff, "%s.jpg", jpgPath.c_str());
        mImagesData.at(mCurrentScene).save(buff);
      }
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

  void updateView(rtx::Scene &scene, nanogui::ImagePanel *imgPanel,
                  GLTexture &texture) {
    updateTexture(scene, texture);

    char buff[100];
    sprintf(buff, "assets/samples/scene%d.png", mCurrentScene);
    texture.save(buff);

    updateIcons(imgPanel);
  }

  void updateTexture(rtx::Scene &scene, GLTexture &texture) const {
    texture.size(_width, _height);

    raytracing(scene, (useMultithreading) ? 12 : 1, texture);

    glBindTexture(GL_TEXTURE_2D, texture.texture());

    std::vector<uint8_t> data;
    auto pixels = texture.pixels();
    for (int j = 0; j < texture.size(); j++) {
      data.push_back(pixels[j].r * 255.0f);
      data.push_back(pixels[j].g * 255.0f);
      data.push_back(pixels[j].b * 255.0f);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _width, _height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void updateIcons(nanogui::ImagePanel *imgPanel) const {
    // On met a jour la popup des miniatures des images
    std::vector<std::pair<int, std::string>> icons =
        nanogui::loadImageDirectory(mNVGContext, "assets/samples");

    std::sort(icons.begin(), icons.end(),
              [](const std::pair<int, std::string> &lhs,
                 const std::pair<int, std::string> &rhs) {
                return lhs.second < rhs.second;
              });
    icons.resize(scenes.size());

    imgPanel->setImages(icons);
  }

  /**
   *  Convert a Json::Value into a Scene object.
   */
  rtx::Scene openScene(std::string);

  /**
   *  Trace all ray & apply super sampling & correction gamma.
   */
  void traceRays(GLTexture::View view, const rtx::Scene &scene,
                 const rtx::Camera &cam) const;

  /**
   *  Split image & launch thread.
   */
  void raytracing(rtx::Scene &scene, int threadsCount,
                  GLTexture &texture) const;

private:
  std::vector<GLTexture> mImagesData;

  std::vector<rtx::Scene> scenes;
  int mCurrentScene;

  float _width = 800.0f;
  float _height = 800.0f;
  float fov = 90.0f;
  float spp = 1.f; // sample par pixels
  bool useMultithreading = true;
  bool useShadow = true;
  bool useAmbiantOcclusion = false;
};

#endif