#include "ECS/PointMass.h"

#include "ECS/TransformComponent.h"

PointMass::PointMass(double imass, glm::dvec3 p0, glm::dvec3 v0) {
    setMass(imass);
    dposition = p0;
    dspeed = v0;
}

void PointMass::init() {
	transform = &entity->getComponent<TransformComponent>();
}

void PointMass::update() {
    transform->position = dposition; //apparently they implemented implicit conversion from dvec3 to vec3
}