#pragma once
#include <memory>
#include "Renderer.h"
#include "Camera.h"
#include "Shader.h"
#include "SolarSystem.h"

struct GLFWwindow;

class Simulation {
public:
    Simulation();
    ~Simulation();
    
    bool initialize(int width, int height);
    void run();
    
private:
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<SolarSystem> solarSystem;
    
    float lastFrameTime;
    float deltaTime;
    
    // input state
    bool firstMouse;
    float lastX, lastY;
    bool freeLookMode;
    
    void processInput();
    void update();
    void render();
    
    // static callbacks for GLFW
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    // static pointer to current instance for callbacks
    static Simulation* currentInstance;
};