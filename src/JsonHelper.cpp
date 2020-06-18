#include "JsonHelper.h"

rtx::Vector JsonHelper::toVector(Json::Value vector) {
  return {vector["x"].asFloat(), vector["y"].asFloat(), vector["z"].asFloat()};
}

rtx::Color JsonHelper::toColor(Json::Value color) {
  return {color["r"].asFloat(), color["g"].asFloat(), color["b"].asFloat()};
}

rtx::Material JsonHelper::toMaterial(Json::Value material) {
  const rtx::Color ka = toColor(material["ka"]);
  const rtx::Color kd = toColor(material["kd"]);
  const rtx::Color ks = toColor(material["ks"]);

  const int shininess = material["shininess"].asInt();

  return {ka, kd, ks, shininess};
}