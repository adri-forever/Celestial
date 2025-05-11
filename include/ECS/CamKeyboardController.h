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

	float sensitivity = glm::pi<float>()/(60.f*4.f);
	float speed = .2f;
	std::string person = "third"; // first person, third person
	bool invX = true;
	bool invY = true;

	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	float distance = 5.f;
	glm::vec3 movspeed;
	glm::vec3 rotspeed = glm::vec3(0.f);
	glm::vec3 angles; // pitch: assiette/tangage - yaw: lacet - roll: roulis
	
	glm::vec3 cameraTarget = glm::vec3(0.f);

	void init() override;

	void update() override;

	void listen_input(SDL_Event event);

	void update_firstperson(glm::quat yaw, glm::quat pitch);
	void update_thirdperson(glm::quat yaw, glm::quat pitch);

	void switchPerson();

};

#endif // !CamKeyboardController_h