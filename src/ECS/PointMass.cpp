#include "ECS/PointMass.h"

#include "ECS/TransformComponent.h"

#include "Physics.h"

PointMass::PointMass(double imass) {
    setMass(imass);
}

PointMass::PointMass(double imass, glm::dvec3 p0, glm::dvec3 v0) {
    setMass(imass);
    dposition = p0;
    dpositionm1 = p0;
    dspeed = v0;
}

void PointMass::rkinit(int klevel) {
    kv.reserve(klevel);
    kp.reserve(klevel);
    pkp.reserve(klevel);

    kv.resize(klevel);
    kp.resize(klevel);
    pkp.resize(klevel);
}

void PointMass::init() {
    entity->addGroup(Physics::physical);
	transform = &entity->getComponent<TransformComponent>();

    if (!entity->hasComponent<TransformComponent>()) {
        entity->addComponent<TransformComponent>(dposition.x, dposition.y, dposition.z);
	    transform = &entity->getComponent<TransformComponent>(); //test if this works
    }

    if (dposition==glm::dvec3(0.)) {
        dposition = transform->position;
    }
}

void PointMass::update() {
    transform->position = dposition; //apparently they implemented implicit conversion from dvec3 to vec3
}