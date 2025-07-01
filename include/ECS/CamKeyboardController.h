#ifndef CamKeyboardController_h
#define CamKeyboardController_h

#include "ECS.h"

#include <glm/glm.hpp>
#include "glm/gtc/constants.hpp"

#include "SDL3/SDL.h"

class TransformComponent;
class Camera;

class CamKeyboardController : public Component {
protected:

public:
	TransformComponent* transform;
	Camera* camera;

	bool listencontrols = true;

	//Translation parameters
	float hspeed = .2f;
	float vspeed = .15f;
	float step = 0.1f; //amount to multiply the movspeed each frame. 1 / number of frames to reach max speed
	bool slide = false; //do we keep going forward after releasing the keys

	glm::vec3 speed = glm::vec3(hspeed, vspeed, hspeed);
	glm::ivec3 direction; //no need for that to be float
	glm::vec3 movspeed = glm::vec3(0.f); //actual movement speed in unit/frame

	//Rotation parameters
	float sensitivity = glm::pi<float>()/(60.f*4.f);
	bool invX = true;
	bool invY = true;

	glm::vec3 rotspeed = glm::vec3(0.f);

	//Camera parameters
	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	std::string person = "third"; // first person, third person
	float distance = 5.f;
	
	glm::vec3 angles; // pitch: assiette/tangage - yaw: lacet - roll: roulis
	glm::vec3 cameraTarget = glm::vec3(0.f);

	void init() override;

	void update() override;

	void listen_input(SDL_Event event);
	void smart_direction();

	void update_firstperson(glm::quat yaw, glm::quat pitch);
	void update_thirdperson(glm::quat yaw, glm::quat pitch);

	void switchPerson();

};

#endif // !CamKeyboardController_h