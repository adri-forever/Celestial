#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <ECS/ECS.h>

#include "glm/glm.hpp"

namespace Physics {
    enum groups : std::size_t {
        physical
    };

    void echo();

    glm::dvec3 eulerex(glm::dvec3 dv, glm::dvec3 v, double dt); // Euler explicite
    glm::dvec3 rk2(glm::dvec3 dv, glm::dvec3 v, double dt); // Runge Kutta 2
    glm::dvec3 rk4(glm::dvec3 dv, glm::dvec3 v, double dt); // 4
    glm::dvec3 rk4d2(glm::dvec3 dv, glm::dvec3 v, double dt); // 4 avec derivee seconde
    glm::dvec3 rk8(glm::dvec3 dv, glm::dvec3 v, double dt); // 8
    glm::dvec3 cranknic(glm::dvec3 dv, glm::dvec3 v, double dt); // Methode de Crank-Nicolson
    

    void compute(EntityManager* emanager, double dt);
}

#endif //PHYSICS_H