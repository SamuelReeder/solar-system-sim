#include "ShadowMap.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

ShadowMap::ShadowMap(unsigned int width, unsigned int height)
    : width(width), height(height) {
    
    // create depth texture
    glGenTextures(1, &depthMapTextureId);
    glBindTexture(GL_TEXTURE_2D, depthMapTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                 width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    // create framebuffer object
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTextureId, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    // check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        // handle error
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMap::~ShadowMap() {
    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthMapTextureId);
}

void ShadowMap::bindForWriting() {
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::bindForReading(unsigned int textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, depthMapTextureId);
}

void ShadowMap::updateLightSpaceMatrix(const glm::vec3& lightPos, const glm::vec3& targetPos) {
    // orthogonal projection for directional sun light
    float nearPlane = 1.0f;
    float farPlane = 100.0f;
    float projectionSize = 50.0f;  // size of the projection volume
    
    glm::mat4 lightProjection = glm::ortho(
        -projectionSize, projectionSize,
        -projectionSize, projectionSize,
        nearPlane, farPlane
    );
    
    glm::mat4 lightView = glm::lookAt(
        lightPos,           // position
        targetPos,          // target
        glm::vec3(0, 1, 0)  // up vector
    );
    
    // calculate light space matrix (transforms world space to light space)
    lightSpaceMatrix = lightProjection * lightView;
}