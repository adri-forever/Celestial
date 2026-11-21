#include "ECS/Camera.h"

#include "Game.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#endif //!GLM_ENABLE_EXPERIMENTAL

Camera::Camera(Renderer* renderer_i) {
    renderer = renderer_i;
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

    // std::cout << "nearplane " << nearplane << "; farplane " << farplane << std::endl;
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
	glm::mat4 skyboxview = glm::mat4(glm::mat3(view)); // remove translations from view for skybox

    // std::cout << "skyboxview =" << glm::to_string(skyboxview) << std::endl;
    // std::cout << "projection =" << glm::to_string(projection) << std::endl;
    // std::cout << "main shader: " << renderer->mainShader << std::endl;
    // std::cout << "skybox shader: " << renderer->skyboxShader << std::endl;

    // renderer->use(renderer->skyboxShader);
    // renderer->setMat4("projection", &projection[0][0]);
    // renderer->setMat4("view", &skyboxview[0][0]);

    renderer->use(renderer->mainShader);
    renderer->setMat4("projection", &projection[0][0]);
    renderer->setMat4("view", &view[0][0]);
}