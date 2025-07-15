#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <string>

#include "glm/glm.hpp"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

glm::vec3 parseHexColor(std::string colorstr);
glm::vec3 parseHexColor_json(json colordata);

bool checkVector(json data, int size);
bool checkVector(json data);

glm::dvec3 parseDvec3(json data);

#endif // TOOLBOX_H