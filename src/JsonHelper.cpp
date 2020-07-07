#include "JsonHelper.h"

#include <rtx/Checkboard.h>

#include <stdexcept>

rtx::Point JsonHelper::toPoint(Json::Value point) {
  return rtx::Point(point["x"].asFloat(), point["y"].asFloat(),
                    point["z"].asFloat());
}

rtx::Color JsonHelper::toColor(Json::Value color) {
  return rtx::Color(color["r"].asFloat(), color["g"].asFloat(),
                    color["b"].asFloat());
}

rtx::Material *JsonHelper::toMaterial(Json::Value material) {
  const std::string type = material["type"].asString();

  const rtx::Color ka = toColor(material["ka"]);
  const rtx::Color kd = toColor(material["kd"]);
  const rtx::Color ks = toColor(material["ks"]);

  const int shininess = material["shininess"].asInt();
  const float reflectivity = material["reflectivity"].asFloat();
  const float refractivity = material["refractivity"].asFloat();

  rtx::Material *mat;

  if (!type.empty()) {
    if (type == "Checkboard") {
      mat = new rtx::Checkboard(ka, kd, ks, shininess, reflectivity,
                                refractivity);
    } else {
      throw std::invalid_argument("Type not supported");
    }
  } else {
    mat = new rtx::Material(ka, kd, ks, shininess, reflectivity, refractivity);
  }

  return mat;
}