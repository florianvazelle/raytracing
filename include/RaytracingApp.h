#ifndef H_RAYTRACINGAPP
#define H_RAYTRACING

#include <fstream>
#include <iostream>
#include <json/json.h>
#include <memory>
#include <nanogui/nanogui.h>
#include <string>

#include "GLTexture.h"
#include <rtx/Camera.h>
#include <rtx/Cube.h>
#include <rtx/Object.h>
#include <rtx/Scene.h>
#include <rtx/Vector.h>

class RaytracingApp : public nanogui::Screen {
public:
  RaytracingApp()
      : nanogui::Screen(Eigen::Vector2i(800, 600), "NanoGUI Test", false) {
    using namespace nanogui;

    std::vector<rtx::Scene> scenes;

    // On crée une fenetre "action"
    auto actionWindow = new Window(this, "Actions");
    actionWindow->setPosition(Vector2i(15, 15));
    actionWindow->setLayout(new GroupLayout());

    std::vector<std::pair<int, std::string>> icons =
        loadImageDirectory(mNVGContext, "assets");
    std::string resourcesFolderPath("./assets/");

    PopupButton *imagePanelBtn = new PopupButton(actionWindow, "Image Panel");
    imagePanelBtn->setIcon(ENTYPO_ICON_FOLDER);
    Popup *popup = imagePanelBtn->popup();
    VScrollPanel *vscroll = new VScrollPanel(popup);
    ImagePanel *imgPanel = new ImagePanel(vscroll);
    imgPanel->setImages(icons);
    popup->setFixedSize(Vector2i(245, 150));

    // On crée une fenetre "image"
    auto imageWindow = new Window(this, "Selected image");
    imageWindow->setPosition(Vector2i(50, 50));
    imageWindow->setLayout(new GroupLayout());

    // Load all of the images by creating a GLTexture object and saving the
    // pixel data.
    // for (auto &icon : icons) {
    Json::Value root;

    std::ifstream file;
    file.open("assets/scene1.json");

    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, file, &root, &errs)) {
      std::cout << errs << std::endl;
    }

    rtx::Scene scene;

    const Json::Value objectsJSON = root["objects"];
    for (int i = 0; i < objectsJSON.size(); ++i) {
      std::string type = objectsJSON[i]["type"].asString();
      float x = objectsJSON[i]["position"]["x"].asFloat();
      float y = objectsJSON[i]["position"]["y"].asFloat();
      float z = objectsJSON[i]["position"]["z"].asFloat();

      if (type == "Cube") {
        rtx::Cube cube;
        cube.translate(x, y, z);
        scene.objects.push_back(&cube);
      }
    }

    const Json::Value lightsJSON = root["lights"];
    for (int i = 0; i < lightsJSON.size(); ++i) {
      float x = lightsJSON[i]["position"]["x"].asFloat();
      float y = lightsJSON[i]["position"]["y"].asFloat();
      float z = lightsJSON[i]["position"]["z"].asFloat();

      rtx::Light light;
      light.translate(x, y, z);
      scene.lights.push_back(&light);
    }

    scenes.push_back(scene);
    // }

    // // Set the first texture
    // auto imageView = new ImageView(imageWindow,
    // mImagesData[0].first.texture()); mCurrentImage = 0;
    // // Change the active textures.
    // imgPanel->setCallback([this, imageView](int i) {
    //   imageView->bindImage(mImagesData[i].first.texture());
    //   mCurrentImage = i;
    //   std::cout << "Selected item " << i << '\n';
    // });

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
    b->setCallback([&] {
      std::cout << "File dialog result: "
                << file_dialog(
                       {
                           {"png", "Portable Network Graphics"},
                       },
                       false)
                << std::endl;
    });
    b = new Button(tools, "Save");
    b->setCallback([&] {
      std::cout << "File dialog result: "
                << file_dialog(
                       {
                           {"png", "Portable Network Graphics"},
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

  void raytracing(rtx::Scene scene) {
    const float width = 800.f;
    const float height = 600.f;

    rtx::Camera cam;
    std::vector<rtx::Color> colors;

    for (float i = 0.f; i < width; i++) {
      for (float j = 0.f; j < height; j++) {
        rtx::Point impact;
        rtx::Ray ray = cam.getRay(i, j);
        rtx::Object *obj = scene.closer_intersected(ray, impact);

        if (obj) {
          // colors.push_back(getImpactColor(ray, obj, impact, scene));
        } else {
          colors.push_back(scene.getBackground());
        }
      }
    }
  }

private:
  using imagesDataType =
      std::vector<std::pair<GLTexture, GLTexture::handleType>>;
  imagesDataType mImagesData;
  int mCurrentImage;
};

#endif