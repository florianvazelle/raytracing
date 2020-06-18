#ifndef H_JSONHELPER
#define H_JSONHELPER

#include <json/json.h>

#include <rtx/Material.h>
#include <rtx/Vector.h>

class JsonHelper {
public:
  static rtx::Vector toVector(Json::Value vector);
  static rtx::Color toColor(Json::Value color);
  static rtx::Material toMaterial(Json::Value material);

private:
  JsonHelper() {}
};

#endif