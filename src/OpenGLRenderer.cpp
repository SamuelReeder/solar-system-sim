#include "OpenGLRenderer.h"
#include <iostream>

OpenGLRenderer::OpenGLRenderer()
    : window(nullptr), width(0), height(0) {
}

OpenGLRenderer::~OpenGLRenderer() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

bool OpenGLRenderer::initialize(int width, int height, const std::string& title) {
    this->width = width;
    this->height = height;
    
    // initialize GLFW
    if (!glfwInit()) {
        std::cerr << "failed to initialize GLFW" << std::endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    // create window
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    
    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "failed to initialize GLAD" << std::endl;
        return false;
    }
    
    // configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    
    return true;
}

void OpenGLRenderer::clear(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::swapBuffers() {
    glfwSwapBuffers(window);
}

bool OpenGLRenderer::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void* OpenGLRenderer::getWindowHandle() const {
    return (void*)window;
}

void OpenGLRenderer::setViewport(int width, int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
}

void OpenGLRenderer::pollEvents() {
    glfwPollEvents();
}