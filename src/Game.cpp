#include "Game.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#endif //!GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Physics.h"

EntityManager Game::entityManager;
OpenGLRenderer Game::glRenderer;
std::vector<SDL_Event> Game::events;

Game::Game() {}
Game::~Game() {}

void printm(Entity& e) {
	std::cout << e.getComponent<PointMass>().getGravComp() << std::endl;
	std::cout << e.getComponent<PointMass>().getMass() << std::endl;
}

void Game::init(const char* title, int width, int height, SDL_WindowFlags flags) {
	if (!SDL_Init(0)) {
		std::cout << "System initialization error: " << SDL_GetError() << std::endl;
		isRunning = false;
	} else {
		std::cout << "Systems initialized" << std::endl;
		
		windowSize = glm::ivec2(width, height);
		window = SDL_CreateWindow(title, width, height, flags);
		//SDL_WindowFlags windowFlags = SDL_GetWindowFlags(window);
		
		if (window) {
			std::cout << "Window created" << std::endl;
			//std::cout << "Requested flags: " << windowFlags << std::endl;
		}

		/*renderer = SDL_CreateRenderer(window, NULL);
		if (renderer) {
			std::cout << "Renderer created" << std::endl;
		}*/
		 
		if ((flags & SDL_WINDOW_OPENGL) != 0) {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

			SDL_GLContext glContext = SDL_GL_CreateContext(window);
			if (glContext) {
				std::cout << "OpenGL context created" << std::endl;
			} else {
				std::cout << "OpenGL context creation error: " << SDL_GetError() << std::endl;
			}
			SDL_GL_SetSwapInterval(1); //VSync

			SDL_GL_MakeCurrent(window, glContext);
			
			if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
				std::cout << "GLAD loading error: " << SDL_GetError() << std::endl;
			}

			glRenderer.init(glContext);
		} else {
			std::cout << "OpenGL not requested" << std::endl;
		}

		isRunning = true;

		//std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

		//loadFont();

		Physics::echo();

		Mesh* ico = meshManager.loadMesh("ico", "./assets/models/ico.obj");
		Mesh* coob = meshManager.loadMesh("coob", "./assets/models/coob.obj");

		auto& camera1(entityManager.addEntity());
		camera1.addComponent<TransformComponent>(0.f, 1.5f, 3.f);
		camera1.addComponent<Camera>(&glRenderer);
		camera1.addComponent<CamKeyboardController>();
		mainCamera = &camera1;

		Physics::import("assets/physcripts/0.json", &entityManager, &glRenderer, &meshManager);

		// glm::vec3 color_red(1.f, 0.f, 0.f);
		// glm::vec3 color_cyan(0.f, 1.f, 1.f);
		// glm::vec3 color_db(0.5f, .1f, .1f);
		// auto& e1(entityManager.addEntity());
		// e1.addComponent<TransformComponent>(0.f, 0.f, 0.f);
		// e1.addComponent<MeshComponent>(&glRenderer, &ico, color_red);
		// e1.getComponent<MeshComponent>().scale = glm::vec3(.5f);
		// e1.addComponent<PointMass>(1.e6);

		// auto& e2(entityManager.addEntity());
		// e2.addComponent<TransformComponent>(4.f, 0.f, 0.f);
		// e2.addComponent<MeshComponent>(&glRenderer, &ico, color_cyan);
		// e2.getComponent<MeshComponent>().scale = glm::vec3(.3f);
		// e2.addComponent<PointMass>(1.e6);

		// auto& e3(entityManager.addEntity());
		// e3.addComponent<TransformComponent>(0.f, 5.f, 0.f);
		// e3.addComponent<MeshComponent>(&glRenderer, &ico, color_db);
		// e3.getComponent<MeshComponent>().scale = glm::vec3(.2f);
		// e3.addComponent<PointMass>(1.e6);

		// printm(e1);
		// printm(e2);
		// printm(e3);

		// Physics::rkinit(&entityManager, klevel);

		toggleCursor(relativeState); //sync without changing
	}
}

void Game::loadFont() {
	TTF_Init();

    std::string fontPath = "assets/font/segoeui.ttf";
	font = TTF_OpenFont(fontPath.c_str(), 72);
	if (font == nullptr) {
		std::cout << "Could not load " << fontPath << " SDL_ttf Error : " << SDL_GetError() << std::endl;
	} else {
		std::cout << "Font " << fontPath << " loaded " << std::endl;
	}
}

void Game::handleEvents() {
	SDL_Event event;	
	std::string message;
	//Clear events vector
	events.clear();

	// std::cout<<"Newframe"<<std::endl;
	while (SDL_PollEvent(&event)) {
		events.push_back(event); //Sends events to registered list for entities to process
		// std::cout<<"polling event "<<event.type<<std::endl;
		
		//Still handle program wide events
		switch (event.type) {
			case SDL_EVENT_QUIT:
				isRunning = false;
				break;

			case SDL_EVENT_WINDOW_RESIZED:
				windowSize = glm::ivec2(event.window.data1, event.window.data2);
				glViewport(0, 0, windowSize.x, windowSize.y); //Synchronize OpenGL viewport size
				std::cout << "Window resized to " << glm::to_string(windowSize) << std::endl;
				break;

			case SDL_EVENT_KEY_DOWN:
				switch (event.key.key) {
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				case SDLK_TAB:
					toggleCursor();
					break;
				case SDLK_P:
					physics = !physics;
					message = (physics) ? "ON" : "OFF";
					std::cout << "Physics " << message << std::endl;
					break;
				case SDLK_N:
					spawn(glm::dvec3(5., 0., 0.), glm::dvec3(0., 0., 0.), 1.e6, 1, "coob");
				default:
					break;
				}
			default:
				break;
		}
	}
}

void Game::update(int framelength) {
	entityManager.refresh();

	double dt = (double)framelength/10; // framelength is in ms and dt is in s. compute in your head what time ratio that makes

	if (physics) {
		Physics::compute_rk2(&entityManager, dt);
		// Physics::compute(&entityManager, dt);
	}
	entityManager.update();

	age++;
}

void Game::render() {
	//Set empty color & clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glRenderer.use();
	
	if (mainCamera) {
		Camera& camera = mainCamera->getComponent<Camera>();
		camera.setCamera();
	} else {
		std::cout << "Camera not found ..." << std::endl;
	}
	// std::cout << "Projection : " << glm::to_string(camera.projection) << std::endl;
	// std::cout << "View : " << glm::to_string(camera.view) << std::endl;

	entityManager.render();

	SDL_GL_SwapWindow(window);
}

void Game::clean() {
	/*glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);*/
	
	TTF_CloseFont(font);
	TTF_Quit();

	SDL_DestroyWindow(window);
	//SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Travail termine !" << std::endl;
}

void Game::toggleCursor() {
	toggleCursor(!relativeState);
}

void Game::toggleCursor(bool state) {
	SDL_SetWindowRelativeMouseMode(window, state);
	SDL_WarpMouseInWindow(window, windowSize.x/2, windowSize.y/2);
	if (mainCamera) {
		mainCamera->getComponent<CamKeyboardController>().listencontrols = state;
	}
	relativeState = state;
}

void Game::spawn(glm::dvec3 p0, glm::dvec3 v0, double m, double size, std::string meshname) {
    auto& e(entityManager.addEntity());
    e.addComponent<TransformComponent>(0.f, 0.f, 0.f);
    e.addComponent<MeshComponent>(&glRenderer, meshManager.getMesh(meshname), glm::vec3(1.f, 1.f, 1.f));
    e.getComponent<MeshComponent>().scale = glm::vec3(size);
    e.addComponent<PointMass>(m, p0, v0);
	e.getComponent<PointMass>().rkinit(klevel);
}