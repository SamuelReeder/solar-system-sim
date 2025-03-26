#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Sphere {
public:
    // Constructs a sphere with a given radius, and subdivision levels.
    Sphere(float radius, unsigned int sectorCount, unsigned int stackCount);
    ~Sphere();

    void draw() const;

private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;

    void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};

#endif
