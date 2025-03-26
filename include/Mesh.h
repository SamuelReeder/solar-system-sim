#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Shader;

class Mesh {
public:
    Mesh();
    virtual ~Mesh();
    
    virtual void initialize() = 0;
    virtual void render(Shader& shader);
    
    void setModelMatrix(const glm::mat4& modelMatrix);
    glm::mat4 getModelMatrix() const;
    
protected:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    unsigned int vao, vbo, ebo;
    bool initialized;
    glm::mat4 modelMatrix;
    
    void setupMesh();
};