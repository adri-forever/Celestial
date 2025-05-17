#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "glad/glad.h"
#include "glm/glm.hpp"

//Will be renamed Mesh. Current Mesh object shall ultimately be renamed to MeshComponent, or something like that
//is this necessary ? we dont need to store all that do we. only store a vector of VAO
// not all that BUT YES IT IS. We need an object to store VAO and VBO so we can free them at the end of the program
// dont really care that much about optimizing. hell, im not going load 500 different meshes with all 50k faces, am i
class Mesh {
public:
    Mesh(std::string filepath);
    ~Mesh();

    std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	GLuint VAO=0, VBO=0;

    bool loadObj(std::string filePath); //take loadmesh from ECS Mesh

    void bind() const {
	    glBindVertexArray(VAO);
    }
};

class MeshManager {
private:
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::unordered_map<std::string, int> map;

public:
    Mesh& loadMesh(std::string name, std::string filepath); //create new mesh object and add it to our list

    Mesh& getMesh(std::string name); //returns VAO for requested mesh. returns -1 if non existent

};

#endif //MESHMANAGER_H