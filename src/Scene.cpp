#include "Scene.h"
#include "Cube.h"
#include "Light.h"

Scene::Scene(Json::Value scene) {
  const Json::Value objectsJSON = scene["objects"];
  for (int i = 0; i < objectsJSON.size(); ++i) {
    std::string type = objectsJSON[i]["type"].asString();
    float x = objectsJSON[i]["position"]["x"].asFloat();
    float y = objectsJSON[i]["position"]["y"].asFloat();
    float z = objectsJSON[i]["position"]["z"].asFloat();

    if (type == "Cube") {
      Cube cube;
      cube.translate(x, y, z);
      objects.push_back(&cube);
    }
  }

  const Json::Value lightsJSON = scene["lights"];
  for (int i = 0; i < lights.size(); ++i) {
    float x = lightsJSON[i]["position"]["x"].asFloat();
    float y = lightsJSON[i]["position"]["y"].asFloat();
    float z = lightsJSON[i]["position"]["z"].asFloat();

    Light light;
    light.translate(x, y, z);
    lights.push_back(&light);
  }
}

/**
 * Retourne la couleur d'arrière plan.
 */
Color Scene::getBackground() const { return {1.f, 0.f, 1.f, 1.f}; };

/**
 * Retourne la valeur de lumière ambiante.
 */
Color Scene::getAmbiant() const { return {1.f, 0.f, 1.f, 1.f}; };

/**
 * Retourne le nombre de lumières dans la scène.
 */
int Scene::nbLights() const { return lights.size(); };

/**
 * Retourne un pointeur vers la nième lumière.
 */
const Light *Scene::getLight(int index) const { return lights.at(index); };

/**
 * Retourne un pointeur vers l'objet intersecté par le rayon passé en paramètre
 * le plus proche, et nullptr si il n'y en a pas. Met à jour le point d'impact
 * passé en paramètre par référence.
 */
Object *Scene::closer_intersected(const Ray &ray, Point &impact) const {
  for (Object *obj : objects) {
    if (obj->intersect(ray, impact)) {
      return obj; // TODO : faire le calcule de distance
    }
  }
  return nullptr;
};