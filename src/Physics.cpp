#include "Physics.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "glm/gtx/norm.hpp"

#endif //!GLM_ENABLE_EXPERIMENTAL

#include "ECS/PointMass.h"

void Physics::echo() { std::cout << "It's physickin time" << std::endl; }

glm::dvec3 Physics::eulerex(glm::dvec3 dv, glm::dvec3 v, double dt) {
    return v+dv*dt;
}

void Physics::compute(EntityManager* emanager, double dt) {
    std::vector<Entity*> entities = emanager->getGroup(Physics::physical);
    std::vector<PointMass*> pms;
    for (auto& e : entities) {
        if (e->hasComponent<PointMass>()) {
            pms.emplace_back(&e->getComponent<PointMass>());
        }
    }
    //est ce que cest vraiment une bonne idee de creer un vecteur avec les trucs a chaque iteration
    //soit on fait confiance au getgroup, soit on fait ce vecteur...

    // std::cout << std::endl << "Counting " << pms.size() << " objects" << std::endl;

    //Reset acceleration
    for (auto& e : pms) {
        e->dacc = glm::dvec3(0.);
    } //optimize later

    // Acceleration
    for (int i=0; i<pms.size()-1; i++) {
        PointMass* e1 = pms[i];
        // std::cout << std::endl << "Computing entity number " << i << std::endl;
        for (int j=i+1; j<pms.size(); j++) {
            PointMass* e2 = pms[j];
            
            // std::cout << "Against entity number " << j << std::endl;

            double e1gc = e1->getGravComp();
            double e2m = e2->getMass();
            glm::dvec3 e1pos = e1->dposition;
            glm::dvec3 e2pos = e2->dposition;
            glm::dvec3 e1vel = e1->dspeed;
            glm::dvec3 e2vel = e2->dspeed;
            glm::dvec3 dir = glm::normalize(e2->dposition - e1->dposition);
            double dist = glm::distance2(e1->dposition, e2->dposition);

            //      vect a           Gm1                 m2                          vect e1e2                                  r2
            glm::dvec3 lacc = e1->getGravComp() * e2->getMass() * glm::normalize(e2->dposition - e1->dposition) / glm::distance2(e1->dposition, e2->dposition);

            // std::cout << "Acceleration of object " << i << " from object " << j << ": " << glm::to_string(lacc) << std::endl;

            e1->dacc += lacc;
            e2->dacc -= lacc;
        }
    }

    for (int i=0; i<pms.size(); i++) {
        PointMass* e = pms[i];
        e->dspeed = Physics::eulerex(e->dacc, e->dspeed, dt);
        e->dposition = Physics::eulerex(e->dspeed, e->dposition, dt);

        if (i==0) {
            std::cout << "Object " << i << " : "<<std::endl;
            std::cout << "Acc : " << glm::to_string(e->dacc) << std::endl;
            std::cout << "Spe : " << glm::to_string(e->dspeed) << std::endl;
            std::cout << "Pos : " << glm::to_string(e->dposition) << std::endl << std::endl;
        }
    }

}