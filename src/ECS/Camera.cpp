#include "ECS/Camera.h"

#include "Game.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#endif //!GLM_ENABLE_EXPERIMENTAL

Camera::Camera(OpenGLRenderer* renderer) {
    glRenderer = renderer;
}

void Camera::init() {
    transform = &entity->getComponent<TransformComponent>();

    entity->addGroup(camera);
    // std::cout << glm::to_string(transform->position) << std::endl;
    
    // ortho = true;
    updateProjection(0.f);
}

void Camera::update() {
	for (SDL_Event event : Game::events) {
		listen_input(event);
	}

    std::cout << "nearplane " << nearplane << "; farplane " << farplane << std::endl;
}

void Camera::listen_input(SDL_Event event) {
    switch (event.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            updateProjection((float)event.window.data1/(float)event.window.data2);
            break;
        default:
            break;
	}
}

void Camera::setPerspective() {
    projection = glm::perspective(glm::radians(fov_deg), aspectR, nearplane, farplane);
    ortho = false;
}

void Camera::setOrtho() {
    projection = glm::ortho(-width/2.f, width/2.f, -height/2.f, height/2.f, nearplane, farplane);
    ortho = true;
}

void Camera::updateProjection(float aspect) {
    //Pass aspect < 0 to just refresh the matrices
    if (aspect > 0.f) {
        aspectR = aspect;
        width = height*aspect;
    }

    if (ortho) {
        setOrtho();
    } else {
        setPerspective();
    }

}

void Camera::setCamera() {
    glRenderer->setMat4("projection", &projection[0][0]);
    glRenderer->setMat4("view", &view[0][0]);
}