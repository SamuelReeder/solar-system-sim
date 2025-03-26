#include "Sphere.h"
#include <cmath>
#include <iostream>

Sphere::Sphere(float radius, unsigned int sectorCount, unsigned int stackCount) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float x, y, z, xy; // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius; // vertex normal
    float s, t; // vertex texCoord

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (unsigned int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep; // from pi/2 to -pi/2
        xy = radius * cosf(stackAngle); // r * cos(u)
        z = radius * sinf(stackAngle);  // r * sin(u)

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep; // from 0 to 2pi

            Vertex vertex;
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            vertex.Position = glm::vec3(x, y, z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertex.Normal = glm::vec3(nx, ny, nz);

            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertex.TexCoords = glm::vec2(s, t);

            vertices.push_back(vertex);
        }
    }

    unsigned int k1, k2;
    for (unsigned int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;        // beginning of next stack

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    indexCount = indices.size();
    setupMesh(vertices, indices);
}

Sphere::~Sphere() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Sphere::setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Sphere::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
