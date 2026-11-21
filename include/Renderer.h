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
	
	GLuint skyboxShader, mainShader, currentShader;

    GLuint skyboxVAO, skyboxVBO, skyboxTexture;
	
	void init(SDL_GLContext context);

	void use(GLuint shader) {
		glUseProgram(shader);
		currentShader = shader;
	}
	// void render();

	void initMain();
	void initSkybox();

    void renderSkybox();

	void setMat4(const std::string& name, const float* mat) const;
};

#endif // !RENDERER_H