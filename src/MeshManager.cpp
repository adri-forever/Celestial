#include "MeshManager.h"

#include <fstream>
#include <sstream>

Mesh::Mesh(std::string filepath) {
    if (loadObj(filepath)) {
        // Generate and bind the Vertex Array Object
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        //std::cout << "Binding VAO " << VAO << std::endl;

        // Generate and bind the Vertex Buffer Object
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        //std::cout << "Binding VBO " << VBO << std::endl;

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind the VAO and VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    } else {
        std::cout << "Failed to load mesh " << filepath << std::endl;
    }
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

bool Mesh::loadObj(std::string filepath) {
	std::string line = "";
	std::string prefix = "";
	std::ifstream fileStream(filepath.c_str());

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices, temp_normals;
	std::vector<glm::vec2> temp_uvs;

	if (fileStream.is_open()) {
		std::cout << "Reading file " << filepath << std::endl;
		while (std::getline(fileStream, line)) {
			std::istringstream iss(line);
			// std::cout << line;

			iss >> prefix;
			if (prefix=="v") {
				glm::vec3 vertex;
				iss >> vertex.x >> vertex.y >> vertex.z;

				// std::cout << " (interpreted as vertex ("
				// << vertex.x << ", " << vertex.y << ", " << vertex.z << ") )" << std::endl;

				temp_vertices.push_back(vertex);

			} else if (prefix=="vt") {
				glm::vec2 uv;
				iss >> uv.x >> uv.y;
				
				// std::cout << " (interpreted as uv ("
				// << uv.x << ", " << uv.y << ") )" << std::endl;
				
				temp_uvs.push_back(uv);

			} else if (prefix=="vn") {
				glm::vec3 normal;
				iss >> normal.x >> normal.y >> normal.z;

				// std::cout << " (interpreted as normal ("
				// << normal.x << ", " << normal.y << ", " << normal.z << ") )" << std::endl;

				temp_normals.push_back(normal);

			} else if (prefix=="f") {
				std::string vertex1, vertex2, vertex3;
				std::string cvert;
				int vertcount = 0;

				// std::cout << " (interpreted as face )" << std::endl;

				while (iss >> cvert) {
					vertcount++;
					if (vertcount > 3) {
						// std::cout << "Counting more than 3 vertices ! (ending face)" << std::endl;
						break;
					}
					
					unsigned int vertexIndex, uvIndex, normalIndex;
					std::istringstream inneriss(cvert);
					std::string part = "";

					std::getline(inneriss, part, '/');
					vertexIndex = std::stoi(part);
					std::getline(inneriss, part, '/');
					uvIndex = std::stoi(part);
					std::getline(inneriss, part, '/');
					normalIndex = std::stoi(part);
					// std::cout << "(face reading v " << vertexIndex << ", uv " << uvIndex << ", normal " << normalIndex << ")" << std::endl;

					vertexIndices.push_back(vertexIndex);
					uvIndices.push_back(uvIndex);
					normalIndices.push_back(normalIndex);
				}
			} else {
				// std::cout << " (ignored)"<<std::endl;
			}
		}

		for (unsigned int i=0; i < vertexIndices.size(); i++) {
			unsigned int vertexIndex = vertexIndices[i];
			glm::vec3 vertex = temp_vertices[vertexIndex-1];
			vertices.push_back(vertex);
			// std::cout << "Pushed vertex " << vertexIndex << " : " << vertex << std::endl;
		}
		for (unsigned int i=0; i < uvIndices.size(); i++) {
			unsigned int uvIndex = uvIndices[i];
			glm::vec2 uv = temp_uvs[uvIndex-1];
			uvs.push_back(uv);
			// std::cout << "Pushed uv " << uvIndex << " : (" << uv.x << ", " << uv.y << ")" << std::endl;
		}
		for (unsigned int i=0; i < normalIndices.size(); i++) {
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 normal = temp_normals[normalIndex-1];
			normals.push_back(normal);
			// std::cout << "Pushed normal " << normalIndex << " : " << normal << std::endl;
		}

		fileStream.close();
	}

	return true;
}

Mesh& MeshManager::loadMesh(std::string name, std::string filepath) {
    Mesh* m = new Mesh(filepath);
    // m->loadObj(filepath);
    std::unique_ptr<Mesh> uPtr{ m };
    meshes.emplace_back(std::move(uPtr));
    map.emplace(name, meshes.size()-1);
    return *m;
}

Mesh& MeshManager::getMesh(std::string name) {
    return *meshes[map[name]];
}