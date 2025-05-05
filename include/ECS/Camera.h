#ifndef CAMERA_H
#define CAMERA_H

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ECS.h"
#include "TransformComponent.h"
#include "OpenGLRenderer.h"

//Let us not delve into 3d graphics by hand, at least not now
 
//2d orthographic projection camera, it completely ignores the z axis (and does not handle depth)

class Camera : public Component {
private:

public:
	Camera() {}
	Camera(OpenGLRenderer* renderer);
	~Camera() {}

	bool active = true;

	bool ortho = false;
	std::string person = "third"; // first person, third person
	bool invX = true;
	bool invY = true;

	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);

	float aspectR = 16.f/9.f;
	float nearplane = 0.1f, farplane = 100.f;
	float fov_deg = 90.f;
	
	float height = 5.f, width = height*aspectR;

	glm::vec3 cameraTarget = glm::vec3(0.f);

	glm::mat4 projection;
	glm::mat4 view = glm::mat4(1.f);

	TransformComponent* transform;

	OpenGLRenderer* glRenderer;

	void init() override;

	void setPerspective();
	void setOrtho();
	void setViewTarget();

	void updateProjection(float aspect);

	void setCamera();
};

#endif //CAMERA_H