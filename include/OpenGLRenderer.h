#pragma once
#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer();
    
    bool initialize(int width, int height, const std::string& title) override;
    void clear(const glm::vec4& color) override;
    void swapBuffers() override;
    bool shouldClose() const override;
    void* getWindowHandle() const override;
    void setViewport(int width, int height) override;
    void pollEvents() override;

private:
    GLFWwindow* window;
    int width, height;
};