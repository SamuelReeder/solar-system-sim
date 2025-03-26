#pragma once
#include <glm/glm.hpp>
#include <string>

class Renderer {
public:
    virtual ~Renderer() = default;
    
    virtual bool initialize(int width, int height, const std::string& title) = 0;
    virtual void clear(const glm::vec4& color) = 0;
    virtual void swapBuffers() = 0;
    virtual bool shouldClose() const = 0;
    virtual void* getWindowHandle() const = 0;
    virtual void setViewport(int width, int height) = 0;
    virtual void pollEvents() = 0;
};