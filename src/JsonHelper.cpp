#include "JsonHelper.h"

rtx::Point JsonHelper::toPoint(Json::Value point) {
  return rtx::Point(point["x"].asFloat(), point["y"].asFloat(),
                    point["z"].asFloat());
}

rtx::Color JsonHelper::toColor(Json::Value color) {
  return rtx::Color(color["r"].asFloat(), color["g"].asFloat(),
                    color["b"].asFloat());
}

rtx::Material JsonHelper::toMaterial(Json::Value material) {
  const rtx::Color ka = toColor(material["ka"]);
  const rtx::Color kd = toColor(material["kd"]);
  const rtx::Color ks = toColor(material["ks"]);

  const int shininess = material["shininess"].asInt();

  return rtx::Material(ka, kd, ks, shininess);
}