#include "utils/glUtils.h"

//Compile checks
void checkShaderCompile(GLuint shader) {
	char infoLog[512];
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
	}
}

void checkProgramLink(GLuint program) {
	char infoLog[512];
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "Program shader linking failed: " << infoLog << std::endl;
	}
}

//Easy vector prints
std::ostream& operator<<(std::ostream& stream, const glm::vec3& v) {
	stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const glm::dvec3& v) {
	stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return stream;
}