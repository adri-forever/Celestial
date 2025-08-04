#include "ECS/PointMass.h"

#include "ECS/TransformComponent.h"

#include "Physics.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#endif //!GLM_ENABLE_EXPERIMENTAL

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

    if (entity->hasComponent<TransformComponent>()) {
	    transform = &entity->getComponent<TransformComponent>();
    } else {
		std::cout << "Could not find TransformComponent for entity " << entity->tag << std::endl;
	}

    if (dposition==glm::dvec3(0.)) {
        dposition = transform->position;
    }
}

void PointMass::update() {
    if (transform) {
        transform->position = dposition; //apparently they implemented implicit conversion from dvec3 to vec3
    }

    if (entity->tag=="moon") {
        std::cout << glm::to_string(dposition) << std::endl;
    }
}