#include "OpenGLRenderer.h"

#include "utils/glUtils.h"

#include <string>
#include <fstream>

// #include <direct.h>

std::string loadShaderFile(const std::string& filepath);

OpenGLRenderer::OpenGLRenderer() {};

OpenGLRenderer::~OpenGLRenderer() {
	SDL_GL_DestroyContext(glContext);
}

void OpenGLRenderer::init(SDL_GLContext context) {
	glContext = context;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// glEnable(GL_STENCIL_TEST);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glClearColor(.2f, .2f, .2f, 1.0f);

	//Build shader program
	std::string vrt = loadShaderFile("./assets/shaders/shader0.vert");
	std::string frag = loadShaderFile("./assets/shaders/shader0.frag");
	const char* vertexShaderSource = vrt.c_str();
	const char* fragmentShaderSource = frag.c_str();
	// std::cout << "Vertex: " << std::endl <<vertexShaderSource << std::endl
	// << "Fragment: "<< std::endl << fragmentShaderSource << std::endl;

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	checkShaderCompile(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompile(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkProgramLink(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void OpenGLRenderer::use() const {
	glUseProgram(shaderProgram);
}

void OpenGLRenderer::render() {

}

void OpenGLRenderer::setMat4(const std::string& name, const float* mat) const {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, mat);
}

std::string loadShaderFile(const std::string& filepath) {
	std::string result = "";
	std::string line = "";
	std::ifstream fileStream(filepath.c_str());

	// std::cout << "Current working directory: " << _getcwd(NULL, 0) << std::endl;
	// std::cout<<"Reading file "<<filepath<<std::endl;
	// if (fileStream.fail()) {
	// 	std::cout<<"Stream failed."<<std::endl;
	// }
	if (fileStream.is_open()) {
		while (std::getline(fileStream, line)) {
			result += line+'\n';
			// std::cout<<line<<std::endl;
		}
		fileStream.close();
	}

	// std::cout << "Result: " << result << std::endl;
	return result;
}