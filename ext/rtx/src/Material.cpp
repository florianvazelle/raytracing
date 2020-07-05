#include <rtx/Material.h>

using namespace rtx;

Material::Material() : shininess(0), reflectivity(0){};

Material::Material(Color ka, Color kd, Color ks, int shininess,
                   float reflectivity)
    : ka(ka), kd(kd), ks(ks), shininess(shininess),
      reflectivity(reflectivity){};