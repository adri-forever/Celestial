#ifndef CamKeyboardController_h
#define CamKeyboardController_h

#include "ECS.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/quaternion.hpp>

class TransformComponent;
class Camera;

class CamKeyboardController : public Component {
protected:

public:
	TransformComponent* transform;
	Camera* camera;

	float rotspeed = glm::pi<float>()/60.f;

	std::string person = "third"; // first person, third person
	bool invX = true;
	bool invY = true;

	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 direction; // The direction the rotation is going, not actually the camera direction
	//im just recording where i should be rotating
	glm::vec3 angles; 
	/* pitch: assiette/tangage
	yaw: lacet
	roll: roulis
	*/
	
	glm::vec3 cameraTarget = glm::vec3(0.f);

	void init() override;

	void update() override;

	void update_firstperson();
	void update_thirdperson();

	void setViewTarget();

};

#endif // !CamKeyboardController_h