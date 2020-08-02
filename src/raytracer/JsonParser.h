#ifndef H_JSONPARSER
#define H_JSONPARSER

#include <json/json.h>
#include <rtx/Checkboard.h>
#include <rtx/Cube.h>
#include <rtx/InfiniteCylinder.h>
#include <rtx/Line.h>
#include <rtx/Plan.h>
#include <rtx/Scene.h>
#include <rtx/Sphere.h>
#include <rtx/Square.h>
#include <rtx/Triangle.h>

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

struct JsonParser {
  static rtx::Point toPoint(Json::Value point);
  static rtx::Color toColor(Json::Value color);
  static rtx::Material* toMaterial(Json::Value material);
  static rtx::Scene openScene(const char* path);
};

#endif