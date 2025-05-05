#ifndef CamKeyboardController
#define CamKeyboardController

#include "ECS.h"

class TransformComponent;

class KeyboardController : public Component {
protected:

public:
	TransformComponent* transform;
	Camera* camera;

	void init() override;

	void update() override;

};

#endif // !CamKeyboardController