#include "Game.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#endif //!GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

EntityManager Game::entityManager;
SDL_Event Game::event;
OpenGLRenderer Game::glRenderer;

//Wacky init
// auto& player(entityManager.addEntity());

enum groupLabels : std::size_t {
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders,
	groupParticles,
	groupUI
};

constexpr groupLabels groupOrder[] = {
	groupLabels::groupMap,
	groupLabels::groupPlayers,
	groupLabels::groupEnemies,
	groupLabels::groupColliders,
	groupLabels::groupParticles,
	groupLabels::groupUI
};

Game::Game() {}
Game::~Game() {}

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

		glm::vec3 color_red(1.f, 0.f, 0.f);
		glm::vec3 color_cyan(0.f, 1.f, 1.f);
		auto& e1(entityManager.addEntity());
		e1.addComponent<TransformComponent>(-.5, 0., 0.);
		e1.addComponent<Mesh>(&glRenderer, "./assets/models/ico.obj", color_red);
		e1.getComponent<Mesh>().scale = glm::vec3(.5f);
		// std::cout << e1.getComponent<TransformComponent>().position << std::endl;

		auto& e2(entityManager.addEntity());
		e2.addComponent<TransformComponent>(2.f, 0.f, 0.f);
		e2.addComponent<Mesh>(&glRenderer, "./assets/models/coob.obj", color_cyan);
		e2.getComponent<Mesh>().scale.y = 5.f;

		auto& camera1(entityManager.addEntity());
		camera1.addComponent<TransformComponent>(0.f, 1.5f, 3.f);
		camera1.addComponent<Camera>(&glRenderer);
		mainCamera = &camera1;
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
	SDL_PollEvent(&event);
	//std::cout << "Handling event of type " << event.type << std::endl;
	switch (event.type) {
		case SDL_EVENT_QUIT:
			isRunning = false;
			break;

		case SDL_EVENT_WINDOW_RESIZED:
			windowSize = glm::ivec2(event.window.data1, event.window.data2);
			glViewport(0, 0, windowSize.x, windowSize.y); //Synchronize OpenGL viewport size
			if (mainCamera) { //Update 3D view aspect ratio
				mainCamera->getComponent<Camera>().updateProjection((float)windowSize.x/(float)windowSize.y);
			}
			std::cout << "Window resized to " << glm::to_string(windowSize) << std::endl;
			break;

		case SDL_EVENT_KEY_DOWN:
			switch (event.key.key) {
			case SDLK_ESCAPE:
				isRunning = false;
				break;
			case SDLK_TAB:
				toggleCursor();
			default:
				break;
			}/*
		case SDL_EVENT_MOUSE_MOTION:
			std::cout << "Mouse moved to " << event.motion.x << ", " << event.motion.y << std::endl;
			if (!SDL_CursorVisible()
				&& (std::abs(event.motion.x - windowSize.getX()) > 64
					|| std::abs(event.motion.y - windowSize.getY()) > 64)
				) {
				SDL_WarpMouseInWindow(window, windowSize.getX() / 2, windowSize.getY() / 2 );
				std::cout << "Mouse force moved" << std::endl;
			}*/
		default:
			break;
	}
}

void Game::update(int framelength) {
	entityManager.refresh();
	entityManager.update();

	age++;
}

void Game::render() {
	//Set empty color & clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glRenderer.use();
	
	Camera& camera = mainCamera->getComponent<Camera>();
	camera.setCamera();
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
	toggleCursor(!SDL_CursorVisible());
}

void Game::toggleCursor(bool state) {
	if (state) {
		SDL_WarpMouseInWindow(window, windowSize.x/2, windowSize.y/2);
		SDL_ShowCursor();
		SDL_SetWindowMouseGrab(window, false);
	} else {
		SDL_HideCursor();
		SDL_SetWindowMouseGrab(window, true);
	}
}