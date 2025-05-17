#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include <vector>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ECS.h"
#include "TransformComponent.h"
#include "OpenGLRenderer.h"
#include "MeshManager.h"

class MeshComponent : public Component {
private:
	//once the conversion to mesh manager has been done, maybe move some things in here

public:
	MeshComponent();
	MeshComponent(OpenGLRenderer* renderer, Mesh* imesh, glm::vec3 icolor);
	~MeshComponent();

	bool wireframe = true;
	glm::vec3 scale = glm::vec3(1.f);

	glm::vec3 color;
	glm::vec3 edgecolor = glm::vec3(0.f);

	Mesh* mesh { nullptr };

	OpenGLRenderer* glRenderer { nullptr };
	TransformComponent* transform { nullptr };

	void init() override;
	void render();
};
#endif // MESHCOMPONENT_H