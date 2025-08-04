#include "ECS/MeshComponent.h"

MeshComponent::MeshComponent() {}

MeshComponent::MeshComponent(OpenGLRenderer* renderer, Mesh* imesh, glm::vec3 icolor) {
	glRenderer = renderer;
	color = icolor;
	mesh = imesh;
}

MeshComponent::~MeshComponent() {
}


void MeshComponent::init() {
	if (entity->hasComponent<TransformComponent>()) {
		transform = &entity->getComponent<TransformComponent>();
	} else {
		std::cout << "Could not find TransformComponent for entity " << entity->tag << std::endl;
	}
}

void MeshComponent::render() {
	if (mesh && transform) {
		GLint uniformLoc = glGetUniformLocation(glRenderer->shaderProgram, "color");
		
		mesh->bind();
		// std::cout << "Drawing entity at " << transform->position << std::endl;
		//Set position
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.f, 1.f);
		
		glm::mat4 model = glm::scale(glm::mat4(1.f), scale);
		model = glm::translate(model, glm::vec3((float)transform->position.x, (float)transform->position.y, (float)transform->position.z));
		glRenderer->setMat4("model", &model[0][0]);

		//Set color
		glUniform3f(uniformLoc, color.r, color.g, color.b);

		glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());

		//Draw edges
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glEnable(GL_LINE_SMOOTH);
			glLineWidth(5.f);
			glDisable(GL_POLYGON_OFFSET_FILL);
			glUniform3f(uniformLoc, edgecolor.r, edgecolor.g, edgecolor.b);
			glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
		}
	}
}