#ifndef RENDERER_H
#define RENDERER_H

#include <string>

#include "glad/glad.h"

#include <SDL3/SDL.h>

class Renderer {
public:
	Renderer();
	~Renderer();

	SDL_GLContext glContext;
	
	GLuint skyboxShader, mainShader;

    GLuint skyboxVAO, skyboxVBO, skyboxTexture;
	
	void init(SDL_GLContext context);
	// void use() const;
	// void render();

	void initMain();
	void initSkybox();

    void renderSkybox();

	void setMat4(const std::string& name, const float* mat) const;
};

#endif // !RENDERER_H