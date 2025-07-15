#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <ECS/ECS.h>

#include "glm/glm.hpp"


class PointMass;
class OpenGLRenderer;
class MeshManager;

namespace Physics {
    enum groups : std::size_t {
        physical
    };

    static int rklevel = 4;

    void echo();

    void import(std::string scriptpath, EntityManager* emanager, OpenGLRenderer* glRenderer, MeshManager* meshManager);
    
    void rkinit(EntityManager* emanager, int klevel);
    void rkinit(EntityManager* emanager);

    void calc_acc(std::vector<PointMass*> pms, int klevel);

    glm::dvec3 eulerex(glm::dvec3 dv, glm::dvec3 v, double dt); // Euler explicite
    glm::dvec3 rk2(glm::dvec3 dv, glm::dvec3 v, double dt); // Runge Kutta 2
    glm::dvec3 rk4(glm::dvec3 dv, glm::dvec3 v, double dt); // 4
    glm::dvec3 rk4d2(glm::dvec3 dv, glm::dvec3 v, double dt); // 4 avec derivee seconde
    glm::dvec3 rk8(glm::dvec3 dv, glm::dvec3 v, double dt); // 8
    glm::dvec3 cranknic(glm::dvec3 dv, glm::dvec3 v, double dt); // Methode de Crank-Nicolson

    glm::dvec3 verlet(glm::dvec3 acc, glm::dvec3 pos, glm::dvec3 posm1, double dt);
    
    void compute_rk2(EntityManager* emanager, double dt);
    void compute(EntityManager* emanager, double dt);
}

#endif //PHYSICS_H