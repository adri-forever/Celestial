#include "ECS/CamKeyboardController.h"

#include "Game.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#endif //!GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/quaternion.hpp>
#include "glm/gtx/quaternion.hpp"

void CamKeyboardController::init() {
	transform = &entity->getComponent<TransformComponent>();
	camera = &entity->getComponent<Camera>();

	// person = "first";
}

void CamKeyboardController::update() {
	listen_input();

	angles += sensitivity*rotspeed;
	angles.x = glm::clamp(angles.x, glm::radians(-89.9f), glm::radians(89.9f));
	angles.y = glm::mod(angles.y, 2*glm::pi<float>());

	glm::quat yaw = glm::angleAxis(angles.y, glm::vec3(0.f, 1.f, 0.f));
	glm::quat pitch = glm::angleAxis(angles.x, glm::vec3(1.f, 0.f, 0.f));

	if (person=="first") {
		update_firstperson(yaw, pitch);
	} else {
		update_thirdperson(yaw, pitch);
	}
}

void CamKeyboardController::listen_input() {
	switch (Game::event.type) {
		case SDL_EVENT_KEY_DOWN:
			switch (Game::event.key.key) {
				case SDLK_Z:
					movspeed.z = -1.f;
					break;
				case SDLK_S:
					movspeed.z = 1.f;
					break;
				case SDLK_Q:
					movspeed.x = -1.f;
					break;
				case SDLK_D:
					movspeed.x = 1.f;
					break;
				case SDLK_F5:
					switchPerson();
					break;
				default:
					break;
			}
			break;
		case SDL_EVENT_KEY_UP:
			switch (Game::event.key.key) {
				case SDLK_Z:
					movspeed.z = 0.f;
					break;
				case SDLK_S:
					movspeed.z = 0.f;
					break;
				case SDLK_Q:
					movspeed.x = 0.f;
					break;
				case SDLK_D:
					movspeed.x = 0.f;
					break;
				default:
					break;
			}
			break;
		case SDL_EVENT_MOUSE_MOTION:
			// std::cout << Game::event.motion.xrel << ", " << Game::event.motion.yrel << std::endl;
			angles += glm::vec3(sensitivity*(float)Game::event.motion.yrel, sensitivity*(float)Game::event.motion.xrel, 0.f);
			break;
		default:
			break;
	}
}

void CamKeyboardController::update_firstperson(glm::quat yaw, glm::quat pitch) {
	//POSITION
	transform->position += glm::mat3_cast(yaw)*speed*movspeed;

	//ROTATION
	transform->rotation = glm::normalize(pitch*yaw); //apply yaw before pitch
	//because the yaw axis is the absolute "up", while the pitch axis is local x
	
	glm::mat4 trans = glm::translate(glm::mat4(1.f), -transform->position);
	glm::mat4 rot = glm::mat4_cast(transform->rotation);

	camera->view = rot*trans;
}

void CamKeyboardController::update_thirdperson(glm::quat yaw, glm::quat pitch) {
	//POSITION
	cameraTarget += glm::mat3_cast(yaw)*speed*movspeed;

	//ROTATION
	transform->rotation = glm::normalize(yaw*pitch); //for some reason its the other way around in this case
	//surely because we are rotating our camera position around a point instead of orienting our camera on itself

	glm::mat3 rot = glm::mat3_cast(transform->rotation);
	transform->position = rot*glm::vec3(0.f, 0.f, distance) + cameraTarget;

	camera->view = glm::lookAt(transform->position, cameraTarget, up);
}

void CamKeyboardController::switchPerson() {
	person = person=="third" ? "first" : "third";
	angles = -angles; //seamless uwu
}