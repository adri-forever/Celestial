#include "ECS/Camera.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#endif //!GLM_ENABLE_EXPERIMENTAL

Camera::Camera(OpenGLRenderer* renderer) {
    glRenderer = renderer;
}

void Camera::init() {
    transform = &entity->getComponent<TransformComponent>();

    // std::cout << glm::to_string(transform->position) << std::endl;
    
    // ortho = true;
    updateProjection(0.f);
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
    //In case of invalid aspect ratio, don t change it
    if (aspect <= .01f) {
        aspect = aspectR;
    }

    aspectR = aspect;
    width = height*aspect;

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