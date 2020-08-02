#include "JsonParser.h"

rtx::Point JsonParser::toPoint(Json::Value point) {
  return rtx::Point(point["x"].asFloat(), point["y"].asFloat(), point["z"].asFloat());
}

rtx::Color JsonParser::toColor(Json::Value color) {
  return rtx::Color(color["r"].asFloat(), color["g"].asFloat(), color["b"].asFloat());
}

rtx::Material* JsonParser::toMaterial(Json::Value material) {
  const std::string type = material["type"].asString();

  const rtx::Color ka = JsonParser::toColor(material["ka"]);
  const rtx::Color kd = JsonParser::toColor(material["kd"]);
  const rtx::Color ks = JsonParser::toColor(material["ks"]);

  const int shininess = material["shininess"].asInt();
  const float reflectivity = material["reflectivity"].asFloat();
  const float refractivity = material["refractivity"].asFloat();

  rtx::Material* mat;

  if (!type.empty()) {
    const float scale = material["scale"].asFloat();
    if (type == "Checkboard") {
      mat = new rtx::Checkboard(ka, kd, ks, shininess, reflectivity, refractivity, scale);
    } else if (type == "Line") {
      mat = new rtx::Line(ka, kd, ks, shininess, reflectivity, refractivity, scale);
    } else {
      throw std::invalid_argument("Type not supported");
    }
  } else {
    mat = new rtx::Material(ka, kd, ks, shininess, reflectivity, refractivity);
  }

  return mat;
}

rtx::Scene JsonParser::openScene(const char* path) {
  Json::Value root;

  std::ifstream file;
  file.open(path);

  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, file, &root, &errs)) {
    std::cout << errs << std::endl;
  }

  rtx::Scene scene;

  const Json::Value objs = root["objects"];
  for (unsigned int i = 0; i < objs.size(); ++i) {
    const Json::Value obj = objs[i];

    std::string type = obj["type"].asString();
    rtx::Point pos = JsonParser::toPoint(obj["position"]);
    rtx::Point rot = JsonParser::toPoint(obj["rotation"]);
    float scale = obj["scale"].asFloat();
    rtx::Material* mat = JsonParser::toMaterial(obj["material"]);

    std::shared_ptr<rtx::Object> object;

    if (type == "Cube") {
      object = std::make_shared<rtx::Cube>(mat);
    } else if (type == "Plan") {
      object = std::make_shared<rtx::Plan>(mat);
    } else if (type == "Square") {
      object = std::make_shared<rtx::Square>(mat);
    } else if (type == "Sphere") {
      object = std::make_shared<rtx::Sphere>(mat);
    } else if (type == "InfiniteCylinder") {
      object = std::make_shared<rtx::InfiniteCylinder>(mat);
    } else if (type == "Triangle") {
      object = std::make_shared<rtx::Triangle>(mat);
    } else {
      throw std::invalid_argument("Type not supported");
    }

    object->scale(scale);
    object->rotateX(rot.x);
    object->rotateY(rot.y);
    object->rotateZ(rot.z);
    object->translate(pos.x, pos.y, pos.z);
    scene.objects.push_back(object);
  }

  const Json::Value ligs = root["lights"];
  for (unsigned int i = 0; i < ligs.size(); ++i) {
    const Json::Value lig = ligs[i];

    rtx::Point pos = JsonParser::toPoint(lig["position"]);
    rtx::Color id = JsonParser::toColor(lig["id"]);
    rtx::Color is = JsonParser::toColor(lig["is"]);

    std::shared_ptr<rtx::Light> light = std::make_shared<rtx::Light>(id, is);
    light->translate(pos.x, pos.y, pos.z);
    scene.lights.push_back(light);
  }

  return scene;
}