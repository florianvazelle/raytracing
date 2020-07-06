#include <rtx/Material.h>

using namespace rtx;

Material::Material() : shininess(0), reflectivity(0), refractivity(0){};

Material::Material(const Color &ka, const Color &kd, const Color &ks,
                   const int &shininess, const float &reflectivity,
                   const float &refractivity)
    : ka(ka), kd(kd), ks(ks), shininess(shininess), reflectivity(reflectivity),
      refractivity(refractivity){};