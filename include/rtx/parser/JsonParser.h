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
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace rtx {
    namespace parser {

        Scene parse_json(const char* path);
            
        struct JsonParser {
            static Point toPoint(Json::Value point);
            static Color toColor(Json::Value color);
            static std::shared_ptr<Material> toMaterial(Json::Value material);
        };

    }
}


#endif