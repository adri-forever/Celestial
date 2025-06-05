#ifndef POINTMASS_H
#define POINTMASS_H

#include "ECS.h"

#include "glm/glm.hpp"

#define BIG_G 6.6743e-11

class TransformComponent;

class PointMass : public Component {
private:
    double mass = 1.;
    double gravComp = mass*BIG_G;

public:
    PointMass() {}
    PointMass(double imass);
    PointMass(double imass, glm::dvec3 p0, glm::dvec3 v0);
    ~PointMass() {}

    glm::dvec3 dposition = glm::dvec3(0.);
    glm::dvec3 dpositionm1 = glm::dvec3(0.); //last position, used for verlet
    glm::dvec3 dspeed = glm::dvec3(0.);
    glm::dvec3 dacc = glm::dvec3(0.);

    std::vector<glm::dvec3> kv; //k coefficients for speed -- rk methods
    std::vector<glm::dvec3> kp; //k coefficients for position
    std::vector<glm::dvec3> pkp; //position to evaluate for each k level depending on our butcher table

    TransformComponent* transform;

    void setMass(double imass) { mass = imass; gravComp = mass*BIG_G; }
    double getMass() { return mass; }
    double getGravComp() { return gravComp; }

    void init() override;
    void update() override;
};

#endif //POINTMASS_H