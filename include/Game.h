#ifndef Game_h
#define Game_h

//SDL
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
//#include <SDL3_gfxPrimitives.h>

//std
#include <iostream>
#include <vector>

//OpenGL
#include "glad/glad.h"
#include "glm/glm.hpp"

//Project classes
#include "MeshManager.h"
#include "OpenGLRenderer.h"
#include "utils/glUtils.h"
#include "ECS/Components.h"

class Game {
private:

public:
	Game();
	~Game();

	SDL_Window* window;
	TTF_Font* font{ nullptr };

	//static SDL_Renderer* renderer;
	// static SDL_Event event;
	static EntityManager entityManager;
	static OpenGLRenderer glRenderer;
	static std::vector<SDL_Event> events;

	MeshManager meshManager;

	bool physics = false;
	int klevel = 4;
	bool relativeState = false;
	bool isRunning = false;
	glm::ivec2 windowSize;

	Entity* mainCamera = nullptr;

	int age = 0; //Age in frames

	void init(const char* title, int width, int height, SDL_WindowFlags flags);
	void clean();

	void loadFont();

	void handleEvents();
	void update(int framelength);
	void render();
	
	void toggleCursor();
	void toggleCursor(bool state);
	//also lock mouse in the window when hidden

	void spawn(glm::dvec3 p0, glm::dvec3 v0, double m, double size, std::string meshname);
};

#endif //Game_h