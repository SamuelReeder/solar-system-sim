#pragma once
#include <memory>
#include "Renderer.h"
#include "Camera.h"
#include "Shader.h"
#include "SolarSystem.h"
#include "ShadowMap.h"

// Forward declare GLFWwindow
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
    std::unique_ptr<Shader> planetShader;
    std::unique_ptr<Shader> sunShader;
    std::unique_ptr<Shader> shadowShader;
    std::unique_ptr<SolarSystem> solarSystem;
    std::unique_ptr<ShadowMap> shadowMap;
    
    float lastFrameTime;
    float deltaTime;
    
    // fps limiting
    double lastFpsLimitTime;
    const double targetFrameTime = 1.0 / 60.0; // target 60 FPS
    
    // input state
    bool firstMouse;
    float lastX, lastY;
    bool freeLookMode;
    
    void processInput();
    void update();
    void render();
    void renderShadowPass();
    void renderMainPass();
    
    // callbacks
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    static Simulation* currentInstance;
};