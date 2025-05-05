#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <string>

#include "glad/glad.h"

#include <SDL3/SDL.h>

class OpenGLRenderer {
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	SDL_GLContext glContext;
	GLuint shaderProgram;
	
	void init(SDL_GLContext context);
	void use() const;
	void render();

	void setMat4(const std::string& name, const float* mat) const;
};

#endif // !OPENGLRENDERER_H
