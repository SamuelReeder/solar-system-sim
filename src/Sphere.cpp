#include "Sphere.h"
#include "Shader.h"
#include <glm/gtc/constants.hpp>

Sphere::Sphere(float radius, unsigned int segments, unsigned int rings)
    : radius(radius), segments(segments), rings(rings) {
}

void Sphere::initialize() {
    generateSphere();
    setupMesh();
}

void Sphere::generateSphere() {
    vertices.clear();
    indices.clear();
    
    float pi = glm::pi<float>();
    
    // generate vertices
    for (unsigned int y = 0; y <= rings; ++y) {
        float phi = y * pi / rings;
        
        for (unsigned int x = 0; x <= segments; ++x) {
            float theta = x * 2.0f * pi / segments;
            
            // position
            float xPos = radius * sin(phi) * cos(theta);
            float yPos = radius * cos(phi);
            float zPos = radius * sin(phi) * sin(theta);
            
            // normal (unit vector from origin)
            glm::vec3 normal = glm::normalize(glm::vec3(xPos, yPos, zPos));
            
            // texture coordinates
            float u = (float)x / segments;
            float v = (float)y / rings;
            
            vertices.push_back({
                {xPos, yPos, zPos},  // position
                normal,              // normal
                {u, v}               // texCoords
            });
        }
    }
    
    // generate indices
    for (unsigned int y = 0; y < rings; ++y) {
        for (unsigned int x = 0; x < segments; ++x) {
            unsigned int i1 = y * (segments + 1) + x;
            unsigned int i2 = i1 + 1;
            unsigned int i3 = (y + 1) * (segments + 1) + x;
            unsigned int i4 = i3 + 1;
            
            // two triangles per quad
            indices.push_back(i1);
            indices.push_back(i3);
            indices.push_back(i2);
            
            indices.push_back(i2);
            indices.push_back(i3);
            indices.push_back(i4);
        }
    }
}