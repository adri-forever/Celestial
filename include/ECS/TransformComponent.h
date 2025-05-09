#ifndef TransformComponent_h
#define TransformComponent_h

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ECS.h"
#include "utils/Vector2_double.h"

class TransformComponent : public Component {
protected:

public:
	TransformComponent() = default;
	TransformComponent(double x, double y, double z) : position{ x, y, z } {};

	glm::vec3 position{ 0.f };
	glm::quat rotation{ 1.f, 0.f, 0.f, 0.f };

};

#endif // !TransformComponent_h
