#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ECS.h"
#include "TransformComponent.h"
#include "OpenGLRenderer.h"
#include "utils/glUtils.h"

class Mesh : public Component {
public:
	Mesh();
	Mesh(OpenGLRenderer* renderer, std::string ifilepath, glm::vec3 icolor);
	~Mesh();

	bool wireframe = true;
	glm::vec3 scale = glm::vec3(1.f);

	std::string filepath;
	glm::vec3 color;
	glm::vec3 edgecolor = glm::vec3(0.f);

	TransformComponent* transform;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	GLuint VAO=0, VBO=0;
	OpenGLRenderer* glRenderer;

	bool loadObj(std::string& filepath);

	void init() override;
	void bind() const;
	void render();
};
#endif // MESH_H