#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

class ShadowMap {
public:
    ShadowMap(unsigned int width = 1024, unsigned int height = 1024);
    ~ShadowMap();

    void bindForWriting();
    void bindForReading(unsigned int textureUnit = 0);
    unsigned int getDepthMapId() const { return depthMapTextureId; }
    
    // get shadow transform matrix (from world to light space)
    void updateLightSpaceMatrix(const glm::vec3& lightPos, const glm::vec3& targetPos);
    glm::mat4 getLightSpaceMatrix() const { return lightSpaceMatrix; }

private:
    unsigned int depthMapFBO;
    unsigned int depthMapTextureId;
    unsigned int width, height;
    glm::mat4 lightSpaceMatrix;
};