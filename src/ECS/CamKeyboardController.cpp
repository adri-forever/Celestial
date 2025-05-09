#include "ECS/CamKeyboardController.h"

#include "Game.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#endif //!GLM_ENABLE_EXPERIMENTAL

void CamKeyboardController::init() {
	transform = &entity->getComponent<TransformComponent>();
	camera = &entity->getComponent<Camera>();

	// if(transform) {
	// 	std::cout << "Transform found" << std::endl;
	// } else {
	// 	std::cout << "Transform not found" << std::endl;
	// }
	// if(camera) {
	// 	std::cout << "Camera found" << std::endl;
	// } else {
	// 	std::cout << "Camera not found" << std::endl;
	// }
}

void CamKeyboardController::update() {
	if (Game::event.type == SDL_EVENT_KEY_DOWN) {
		switch (Game::event.key.key) {
		case SDLK_Z:
			direction.x = -1.f;
			break;
		case SDLK_S:
			direction.x = 1.f;
			break;
		case SDLK_Q:
			direction.y = 1.f;
			break;
		case SDLK_D:
			direction.y = -1.f;
			break;
		}
	}
	if (Game::event.type == SDL_EVENT_KEY_UP) {
		switch (Game::event.key.key) {
		case SDLK_Z:
			direction.x = 0.f;
			break;
		case SDLK_S:
			direction.x = 0.f;
			break;
		case SDLK_Q:
			direction.y = 0.f;
			break;
		case SDLK_D:
			direction.y = 0.f;
			break;
		}
	}

	if (person=="first") {
		update_firstperson();
	} else {
		update_thirdperson();
	}
}

void CamKeyboardController::update_firstperson() {
	glm::quat rot_quat = transform->rotation; //convenience
	
	//Full quat approach
	glm::vec3 vert = glm::normalize(rot_quat*glm::vec3(0.f, 1.f, 0.f));
	glm::quat yaw = glm::angleAxis(rotspeed*direction.y, vert);
	rot_quat = glm::normalize(yaw*rot_quat);

	// glm::vec3 normalized = glm::normalize(rot_quat*glm::vec3(1.f, 0.f, 0.f));
	glm::quat pitch = glm::angleAxis(rotspeed*direction.x, glm::vec3(1.f, 0.f, 0.f));
	rot_quat = glm::normalize(pitch*rot_quat);
	transform->rotation = rot_quat;

	glm::mat4 trans = glm::translate(glm::mat4(1.f), -transform->position);
	glm::mat4 rot = glm::mat4_cast(transform->rotation);

	angles = glm::eulerAngles(rot_quat);
	camera->view = rot*trans;
}

void CamKeyboardController::update_thirdperson() {
	
}

void CamKeyboardController::setViewTarget() {
    // glm::vec3 cameraDirection = glm::normalize(transform->position - cameraTarget);
    // glm::vec3 cameraPos = transform->position;
    camera->view = glm::lookAt(transform->position, cameraTarget, up);

	// std::cout << "Projection : " << glm::to_string(projection) << std::endl;
	// std::cout << "View : " << glm::to_string(view) << std::endl;
}