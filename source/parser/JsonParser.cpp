#include <rtx/parser/JsonParser.h>

using namespace rtx;
using namespace rtx::parser;

Point JsonParser::toPoint(Json::Value point) {
  return Point(point["x"].asFloat(), point["y"].asFloat(), point["z"].asFloat());
}

Color JsonParser::toColor(Json::Value color) {
  return Color(color["r"].asFloat(), color["g"].asFloat(), color["b"].asFloat());
}

std::shared_ptr<Material> JsonParser::toMaterial(Json::Value material) {
  const std::string type = material["type"].asString();

  const Color ka = JsonParser::toColor(material["ka"]);
  const Color kd = JsonParser::toColor(material["kd"]);
  const Color ks = JsonParser::toColor(material["ks"]);

  const int shininess = material["shininess"].asInt();
  const float reflectivity = material["reflectivity"].asFloat();
  const float refractivity = material["refractivity"].asFloat();

  std::shared_ptr<Material> mat;

  if (!type.empty()) {
    const float scale = material["scale"].asFloat();
    if (type == "Checkboard") {
      mat = std::make_shared<Checkboard>(ka, kd, ks, shininess, reflectivity, refractivity, scale);
    } else if (type == "Line") {
      mat = std::make_shared<Line>(ka, kd, ks, shininess, reflectivity, refractivity, scale);
    } else {
      throw std::invalid_argument("Type not supported");
    }
  } else {
    mat = std::make_shared<Material>(ka, kd, ks, shininess, reflectivity, refractivity);
  }

  return mat;
}

Scene rtx::parser::parse_json(const char* path) {
  Json::Value root;

  std::ifstream file;
  file.open(path);

  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, file, &root, &errs)) {
    std::cout << errs << std::endl;
  }

  Scene scene;

  const Json::Value objs = root["objects"];
  for (unsigned int i = 0; i < objs.size(); ++i) {
    const Json::Value obj = objs[i];

    std::string type = obj["type"].asString();
    Point pos = JsonParser::toPoint(obj["position"]);
    Point rot = JsonParser::toPoint(obj["rotation"]);
    float scale = obj["scale"].asFloat();
    std::shared_ptr<Material> mat = JsonParser::toMaterial(obj["material"]);

    std::shared_ptr<Object> object;

    if (type == "Cube") {
      object = std::make_shared<Cube>(mat);
    } else if (type == "Plan") {
      object = std::make_shared<Plan>(mat);
    } else if (type == "Square") {
      object = std::make_shared<Square>(mat);
    } else if (type == "Sphere") {
      object = std::make_shared<Sphere>(mat);
    } else if (type == "InfiniteCylinder") {
      object = std::make_shared<InfiniteCylinder>(mat);
    } else if (type == "Triangle") {
      object = std::make_shared<Triangle>(mat);
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

    Point pos = JsonParser::toPoint(lig["position"]);
    Color id = JsonParser::toColor(lig["id"]);
    Color is = JsonParser::toColor(lig["is"]);

    std::shared_ptr<Light> light = std::make_shared<Light>(id, is);
    light->translate(pos.x, pos.y, pos.z);
    scene.lights.push_back(light);
  }

  return scene;
}