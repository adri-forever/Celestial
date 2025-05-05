#include "ECS/CamKeyboardController.h"

#include "Game.h"

void KeyboardController::init() {
	transform = &entity->getComponent<TransformComponent>();
	camera = &entity->getComponent<Camera>();
}

void KeyboardController::update() {
	/*if (Game::event.type == SDL_EVENT_KEY_DOWN) {
		switch (Game::event.key.key) {
		case SDLK_Z:
			transform->setVy(-1);
			break;
		case SDLK_S:
			transform->setVy(1);
			break;
		case SDLK_Q:
			transform->setVx(-1);
			break;
		case SDLK_D:
			transform->setVx(1);
			break;
		}
	}

	if (Game::event.type == SDL_EVENT_KEY_UP) {
		switch (Game::event.key.key) {
		case SDLK_Z:
			transform->setVy(0);
			break;
		case SDLK_S:
			transform->setVy(0);
			break;
		case SDLK_Q:
			transform->setVx(0);
			break;
		case SDLK_D:
			transform->setVx(0);
			break;
		}
	}*/

	//transform->printPos();

}