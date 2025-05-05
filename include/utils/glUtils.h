#ifndef GLUTILS_H
#define GLUTILS_H

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

void checkShaderCompile(GLuint shader);
void checkProgramLink(GLuint program);

std::ostream& operator<<(std::ostream& stream, const glm::vec3& v);
std::ostream& operator<<(std::ostream& stream, const glm::dvec3& v);

#endif // !GLUTILS_H
