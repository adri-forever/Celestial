#include "Physics.h"

//std
#include <fstream>
#include <sstream>

// personal includes
#include "MeshManager.h"
#include "ECS/PointMass.h"
#include "ECS/MeshComponent.h"
#include "ECS/Camera.h"
#include "ECS/CamKeyboardController.h"
#include "utils/toolbox.h"

// thirdparty
#include "nlohmann/json.hpp"

    // GL bazar
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#endif //!GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/norm.hpp"

using json = nlohmann::json;

void Physics::echo() { std::cout << "It's physickin time" << std::endl; }

void Physics::import(std::string scriptpath, EntityManager* emanager, OpenGLRenderer* glRenderer, MeshManager* meshManager) {
    // Read input file
    std::ifstream f {scriptpath};
    json data = json::parse(f);

    // Load entities
    std::string name;
    glm::vec3 color;
    glm::dvec3 p0, v0;
    double radius, mass;

    Entity* e = nullptr;
    Camera* cam = nullptr;
    CamKeyboardController* ckc = nullptr;
    TransformComponent* tc = nullptr;

    if (data["objects"].is_array()) {
        for (int i=0; i < data["objects"].size(); i++) {
            if (data["objects"][i]["name"].is_string()) {
                name = data["objects"][i]["name"];
            } else {
                name = std::to_string(i);
            }
            std::cout << "Importing entity " << name << std::endl;
            // std::cout << data["objects"][i] << std::endl;

            if (data["objects"][i]["radius"].is_number()) {
                radius = 1000.*(double)data["objects"][i]["radius"];
            }
            if (data["objects"][i]["mass"].is_number()) {
                mass = data["objects"][i]["mass"];
            }

            e = &emanager->addEntity();
            e->tag = name;
            e->addComponent<TransformComponent>();
            e->addComponent<MeshComponent>(glRenderer, meshManager->getMesh_index(0), parseHexColor_json(data["objects"][i]["color"]));
            e->getComponent<MeshComponent>().scale = glm::vec3(radius);
            e->addComponent<PointMass>(mass, parseDvec3(data["objects"][i]["p0"]), parseDvec3(data["objects"][i]["v0"]));
            e->getComponent<PointMass>().rkinit(Physics::rklevel);
        }
    }

    if(data["camera"].is_object()) {
        for (auto& e : emanager->getGroup(camera)) {
            cam = &e->getComponent<Camera>();
            if (e->hasComponent<CamKeyboardController>()) {
                ckc = &e->getComponent<CamKeyboardController>();
            }
            if (e->hasComponent<TransformComponent>()) {
                tc = &e->getComponent<TransformComponent>();
            }

            if (e->getComponent<Camera>().active) {
                std::cout << "Updating " << e->tag << std::endl;
                
                if (data["camera"]["farplane"].is_number()) {
                    cam->farplane = data["camera"]["farplane"];
                }
                if (data["camera"]["nearplane"].is_number()) {
                    cam->nearplane = data["camera"]["nearplane"];
                }

                if (ckc) {   
                    if (data["camera"]["person"].is_string()) {
                        if (data["camera"]["person"] == "third") {
                            ckc->person = "third";
                        } else {
                            ckc->person = "first";
                        }
                    }
                    if (data["camera"]["distance"].is_number()) {
                        ckc->distance = data["camera"]["distance"];
                    }
                    if (data["camera"]["speedfactor"].is_number()) {
                        ckc->speed *= data["camera"]["speedfactor"];
                    }
                    if (data["camera"]["position"].is_array()) {
                        ckc->cameraTarget = (glm::vec3)parseDvec3(data["camera"]["position"]);
                    }
                }

                cam->updateProjection();
                
                break; //only update the active camera
            }
        }
    }
}

void Physics::rkinit(EntityManager* emanager, int klevel) {
    std::vector<Entity*> entities = emanager->getGroup(Physics::physical);
    for (auto& e : entities) {
        if (e->hasComponent<PointMass>()) {
            e->getComponent<PointMass>().rkinit(klevel); //init rk
        }
    }
    
}

void Physics::rkinit(EntityManager* emanager) {
    Physics::rkinit(emanager, Physics::rklevel);
}

glm::dvec3 Physics::eulerex(glm::dvec3 dv, glm::dvec3 v, double dt) {
    return v+dv*dt;
}

glm::dvec3 Physics::rk2(glm::dvec3 dv, glm::dvec3 v, double dt) {
    glm::dvec3 k1 = dv
    , k2;
    return v + .5*dt*(k1+k2);
}

glm::dvec3 Physics::verlet(glm::dvec3 acc, glm::dvec3 pos, glm::dvec3 posm1, double dt) {
    return 2.*pos - posm1 + dt*dt*acc;
}

void Physics::calc_acc(std::vector<PointMass*> pms, int klevel) {
    //pms is a list of all the interacting PointMasses objects
    //klevel decides which "position" is used to compute the coefficients :
    //klevel -1 is used to compute k1 (index 0 in the k array), and takes "real" position
    //klevel >= 0 is used to compute k(klevel+2) from k(klevel+1) position
    //that way the positions we use are just kp[klevel] 

    PointMass* e1,* e2;
    glm::dvec3 dp1, dp2;

    for (auto& e : pms) {
        e->force = glm::dvec3(0.);
    }

    for (int i=0; i<pms.size()-1; i++) {
        e1 = pms[i];
        dp1 = klevel < 0 ? e1->dposition : e1->pkp[klevel];

        // std::cout << std::endl << "Computing entity number " << i << std::endl;
        for (int j=i+1; j<pms.size(); j++) {
            e2 = pms[j];
            dp2 = klevel < 0 ? e2->dposition : e2->pkp[klevel];
            
            // std::cout << "Against entity number " << j << std::endl;

            //      vect a           Gm1                 m2                          vect e1e2                                  r2
            glm::dvec3 f = e1->getGravComp() * e2->getMass() * glm::normalize(dp2 - dp1) / glm::distance2(dp1, dp2); //this is force actually

            // std::cout << "Acceleration of object " << i << " from object " << j << ": " << glm::to_string(f) << std::endl;

            e1->force += f;
            e2->force -= f;
        }
    }

    for (auto& e : pms) {
        e->dacc = e->force / e->getMass();
    }
}

void printpos(PointMass* pm) {
    std::cout << "Definite : " << std::endl;
    std::cout << "\t" << "V = " << glm::to_string(pm->dspeed) << std::endl;
    std::cout << "\t" << "P = " << glm::to_string(pm->dposition) << std::endl << std::endl;
}

void printms(PointMass* pm, int klevel) {
    std::cout << "Speed K coefficients : " << std::endl;
    for (int i=0; i<klevel; i++) {
        std::cout << "\t" << "kv" << i+1 << " = " << glm::to_string(pm->kv[i]) << std::endl;
    }
    
    std::cout << "Position K coefficients : " << std::endl;
    for (int i=0; i<klevel; i++) {
        std::cout << "\t" << "kp" << i+1 << " = " << glm::to_string(pm->kp[i]) << std::endl;
    }

    std::cout << "Estimated positions : " << std::endl;
    for (int i=0; i<klevel-1; i++) {
        std::cout << "\t" << "pkp" << i+1 << " = " << glm::to_string(pm->pkp[i]) << std::endl;
    }

    printpos(pm);
}


void Physics::compute_rk2(EntityManager* emanager, double dt) {
    std::vector<Entity*> entities = emanager->getGroup(Physics::physical);
    std::vector<PointMass*> pms;
    for (auto& e : entities) {
        if (e->hasComponent<PointMass>()) {
            pms.emplace_back(&e->getComponent<PointMass>()); //add to our vector
        }
    }

    //k1
    Physics::calc_acc(pms, -1); //acc
    for (auto& e : pms) { //k1
        e->kv[0] = e->dacc;
        e->kp[0] = e->dspeed;

        e->pkp[0] = e->dposition + .5*dt*e->kp[0];
    }

    //k2
    Physics::calc_acc(pms, 0); //acc
    for (auto& e : pms) { //k2
        e->kv[1] = e->dacc;
        e->kp[1] = e->kv[0];
    }

    //... kn 

    //calcule pos en abaissant les k selon la formule
    for (auto& e : pms) {
        e->dposition += 0.5*dt*(e->kp[0] + e->kp[1]);
        e->dspeed += 0.5*dt*(e->kv[0] + e->kv[1]); 
    }

    // printms(pms[pms.size()-1], 2);
    printpos(pms[pms.size()-1]);
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

    Physics::calc_acc(pms, -1);

    for (int i=0; i<pms.size(); i++) {
        PointMass* e = pms[i];
        
        //eulerex
        e->dspeed = Physics::eulerex(e->dacc, e->dspeed, dt);
        e->dposition = Physics::eulerex(e->dspeed, e->dposition, dt);

        //verlet
        //works like shite
        // {
        //     glm::dvec3 npos = Physics::verlet(e->dacc, e->dposition, e->dpositionm1, dt);

        //     e->dpositionm1 = e->dposition;
        //     e->dposition = npos;
        //     // speed is not needed here
        // }

        if (i==2) {
            std::cout << "Object " << i << " : "<<std::endl;
            std::cout << "Acc : " << glm::to_string(e->dacc) << std::endl;
            std::cout << "Spe : " << glm::to_string(e->dspeed) << std::endl;
            std::cout << "Pos : " << glm::to_string(e->dposition) << std::endl << std::endl;
        }
    }

}