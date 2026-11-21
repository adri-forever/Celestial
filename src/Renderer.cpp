#include "Renderer.h"

#include "utils/glUtils.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif //STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <string>
#include <fstream>

// #include <direct.h>
int triVertCount = 3;
float skyboxVertices[] = { // triangles
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};
int skyboxVerticesSize = sizeof(skyboxVertices) / sizeof(skyboxVertices[0]);
int skyboxFaceCount = skyboxVerticesSize / triVertCount;

std::string loadShaderFile(const std::string& filepath);

Renderer::Renderer() {};

Renderer::~Renderer() {
	SDL_GL_DestroyContext(glContext);
}

void Renderer::init(SDL_GLContext context) {
	glContext = context;

	initMain();
	initSkybox();
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// glEnable(GL_STENCIL_TEST);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void Renderer::initMain() {
	std::cout << "Building main shader" << std::endl;

	std::string vrt = loadShaderFile("./assets/shaders/main/main.vert");
	std::string frag = loadShaderFile("./assets/shaders/main/main.frag");
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

	mainShader = glCreateProgram();
	glAttachShader(mainShader, vertexShader);
	glAttachShader(mainShader, fragmentShader);
	glLinkProgram(mainShader);
	checkProgramLink(mainShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Renderer::initSkybox() {
	std::cout << "Building skybox shader" << std::endl;
	
	// Create program
	std::string vrt = loadShaderFile("./assets/shaders/skybox/skybox.vert");
	std::string frag = loadShaderFile("./assets/shaders/skybox/skybox.frag");
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

	skyboxShader = glCreateProgram();
	glAttachShader(skyboxShader, vertexShader);
	glAttachShader(skyboxShader, fragmentShader);
	glLinkProgram(skyboxShader);
	checkProgramLink(skyboxShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Bind textures
	glGenTextures(1, &skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	std::string textures[] = {"sides_xpos.png", "sides_xneg.png", "top.png", "bottom.png", "sides.png", "sides.png"};
	int width, height, channels;
	std::string tex;
	for (int i=0; i<size(textures); i++) {
		std::cout << "Reading texture " << textures[i] << std::endl;
		tex = "./assets/textures/skybox/"+textures[i];
		unsigned char *texdata = stbi_load(tex.c_str(), &width, &height, &channels, 0);
		if (texdata) {
			// for (int j=0; j < std::min(width*height, 100); j++) { // Print ptexture pixels
			// 	std::cout << "Pixel " << j << ": ";
			// 	for (int k=0; k < channels; k++) {
			// 		printf("%02x, ", texdata[channels*j+k]);
			// 		// std::cout << std::hex << texdata[channels*i+j] << ", ";
			// 	}
			// 	std::cout << std::endl;
			// }
			if (channels == 3) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (GLuint)i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texdata);
			} else if (channels == 4) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (GLuint)i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);

			} else {
				std::cout << "I have " << channels << " channels i dont know what to do" << std::endl;
			}
		} else {
			std::cout << "Could not load skybox data at " << tex << " (face " << i << ")" << std::endl;
		}
		stbi_image_free(texdata);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Set mesh	
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);

	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices[0], GL_STATIC_DRAW);

	std::cout << "Skybox VAO: " << skyboxVAO << ", VBO: " << skyboxVBO << std::endl;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, triVertCount * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Renderer::renderSkybox() {
	glDepthMask(GL_FALSE);
	use(skyboxShader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, skyboxFaceCount);
	glDepthMask(GL_TRUE);
}

void Renderer::setMat4(const std::string& name, const float* mat) const {
	// std::cout << name << std::endl;
	// std::cout << name.c_str() << std::endl;
	// printf("glGetUniformLocation = %p\n", (void*)glGetUniformLocation);
	// printf("program = %u\n", program);
	// printf("is program = %d\n", glIsProgram(program));
	GLuint location = glGetUniformLocation(currentShader, name.c_str()); // WHICH SHADER (context)???? make use method and currentShader attribute to keep memory ?
	glUniformMatrix4fv(location, 1, GL_FALSE, mat);
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