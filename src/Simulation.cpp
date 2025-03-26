#include "Simulation.h"
#include "OpenGLRenderer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <chrono>

// initialize static member
Simulation* Simulation::currentInstance = nullptr;

Simulation::Simulation()
    : lastFrameTime(0.0f), 
      deltaTime(0.0f), 
      lastFpsLimitTime(0.0),
      firstMouse(true), 
      lastX(0.0f), 
      lastY(0.0f),
      freeLookMode(false) {
    currentInstance = this;
}

Simulation::~Simulation() {
    currentInstance = nullptr;
}

bool Simulation::initialize(int width, int height) {
    // create renderer
    renderer = std::make_unique<OpenGLRenderer>();
    if (!renderer->initialize(width, height, "Solar System Simulation")) {
        return false;
    }
    
    // create camera
    camera = std::make_unique<Camera>(45.0f, (float)width / (float)height);
    camera->setPosition(glm::vec3(0.0f, 10.0f, 30.0f));
    camera->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
    
    // create shadow map
    shadowMap = std::make_unique<ShadowMap>(2048, 2048);
    
    // load shaders
    planetShader = std::make_unique<Shader>();
    if (!planetShader->load("shaders/planet.vert", "shaders/planet.frag")) {
        return false;
    }
    
    sunShader = std::make_unique<Shader>();
    if (!sunShader->load("shaders/sun.vert", "shaders/sun.frag")) {
        return false;
    }
    
    shadowShader = std::make_unique<Shader>();
    if (!shadowShader->load("shaders/shadow_mapping.vert", "shaders/shadow_mapping.frag")) {
        return false;
    }
    
    // setup solar system
    solarSystem = std::make_unique<SolarSystem>();
    
    // add solar bodies (same as before)
    solarSystem->addStar("Sun", 2.0f, 27.0f);
    solarSystem->addPlanet("Mercury", 0.08f, 4.0f, 88.0f, 58.6f);
    solarSystem->addPlanet("Venus", 0.21f, 7.0f, 225.0f, 243.0f);
    solarSystem->addPlanet("Earth", 0.22f, 10.0f, 365.0f, 1.0f);
    solarSystem->addMoon("Earth", "Moon", 0.06f, 0.5f, 27.3f, 27.3f);
    solarSystem->addPlanet("Mars", 0.12f, 15.0f, 687.0f, 1.03f);
    solarSystem->addPlanet("Jupiter", 0.5f, 22.0f, 4333.0f, 0.41f);
    solarSystem->addPlanet("Saturn", 0.45f, 28.0f, 10759.0f, 0.45f);
    solarSystem->addPlanet("Uranus", 0.3f, 33.0f, 30687.0f, 0.72f);
    solarSystem->addPlanet("Neptune", 0.3f, 38.0f, 60190.0f, 0.67f);
    
    solarSystem->initialize();
    
    // setup input callbacks
    GLFWwindow* window = static_cast<GLFWwindow*>(renderer->getWindowHandle());
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    // set initial mouse position
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    
    // print controls
    std::cout << "Controls:\n"
              << "  WASD - Move camera\n"
              << "  Q/E  - Move up/down\n"
              << "  Mouse - Look around (hold right mouse button)\n"
              << "  F    - Toggle free-look mode\n"
              << "  Shift - Move faster\n"
              << "  ESC  - Exit\n";
    
    return true;
}

void Simulation::run() {
    lastFpsLimitTime = glfwGetTime();
    
    while (!renderer->shouldClose()) {
        // calculate delta time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        
        // process input, update, and render
        processInput();
        update();
        render();
        
        // swap buffers and poll events
        renderer->swapBuffers();
        renderer->pollEvents();
        
        // limit frame rate
        double currentFrameTime = glfwGetTime();
        double frameTime = currentFrameTime - lastFpsLimitTime;
        if (frameTime < targetFrameTime) {
            // sleep for the remaining time to hit target frame rate
            std::this_thread::sleep_for(std::chrono::duration<double>(targetFrameTime - frameTime));
        }
        lastFpsLimitTime = glfwGetTime();
    }
}

void Simulation::processInput() {
    GLFWwindow* window = static_cast<GLFWwindow*>(renderer->getWindowHandle());
    
    // speed multiplier when shift is pressed
    float speedMultiplier = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || 
        glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        speedMultiplier = 5.0f;
    }
    
    // WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->moveForward(5.0f * deltaTime * speedMultiplier);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->moveForward(-5.0f * deltaTime * speedMultiplier);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->moveRight(-5.0f * deltaTime * speedMultiplier);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->moveRight(5.0f * deltaTime * speedMultiplier);
    
    // Q/E for up/down movement
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->moveUp(-5.0f * deltaTime * speedMultiplier);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->moveUp(5.0f * deltaTime * speedMultiplier);
}

void Simulation::update() {
    // update solar system with time scaled
    solarSystem->update(deltaTime * 5000.0f);
    
    // get the position of the sun
    glm::vec3 sunPosition = solarSystem->getSunPosition();
    
    // update shadow mapping
    shadowMap->updateLightSpaceMatrix(sunPosition, glm::vec3(0.0f));
}

void Simulation::render() {
    // render shadows first
    renderShadowPass();
    
    // then render main scene
    renderMainPass();
}

void Simulation::renderShadowPass() {
    // bind shadow map FBO
    shadowMap->bindForWriting();
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // use shadow shader for depth pass
    shadowShader->use();
    
    // set light space matrix
    glm::mat4 lightSpaceMatrix = shadowMap->getLightSpaceMatrix();
    shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    
    // render scene from light's point of view (except sun)
    solarSystem->renderDepth(*shadowShader);
    
    // reset framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Simulation::renderMainPass() {
    // restore viewport
    int width, height;
    glfwGetFramebufferSize(static_cast<GLFWwindow*>(renderer->getWindowHandle()), &width, &height);
    glViewport(0, 0, width, height);
    
    // clear screen
    renderer->clear(glm::vec4(0.0f, 0.0f, 0.05f, 1.0f));
    
    // bind shadow map for reading
    shadowMap->bindForReading(0);
    
    // get the position of the sun
    glm::vec3 sunPosition = solarSystem->getSunPosition();
    
    // render planets with shadows
    planetShader->use();
    
    // set matrices
    planetShader->setMat4("view", camera->getViewMatrix());
    planetShader->setMat4("projection", camera->getProjectionMatrix());
    planetShader->setMat4("lightSpaceMatrix", shadowMap->getLightSpaceMatrix());
    
    // set lighting properties
    planetShader->setVec3("viewPos", camera->getPosition());
    planetShader->setVec3("lightPos", sunPosition);
    planetShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 0.9f));
    
    // set texture
    planetShader->setInt("shadowMap", 0); // bind to texture unit 0
    
    // render planets
    solarSystem->renderPlanets(*planetShader);
    
    // render sun with special shader
    sunShader->use();
    sunShader->setMat4("view", camera->getViewMatrix());
    sunShader->setMat4("projection", camera->getProjectionMatrix());
    
    // render sun
    solarSystem->renderSun(*sunShader);
}


// Static callback implementations
void Simulation::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    if (!currentInstance) return;
    
    currentInstance->renderer->setViewport(width, height);
    if (currentInstance->camera)
        currentInstance->camera->setAspectRatio((float)width / (float)height);
}

void Simulation::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!currentInstance || !currentInstance->camera) return;
    
    if (currentInstance->firstMouse) {
        currentInstance->lastX = xpos;
        currentInstance->lastY = ypos;
        currentInstance->firstMouse = false;
    }
    
    float xoffset = xpos - currentInstance->lastX;
    float yoffset = currentInstance->lastY - ypos; // reversed y-coordinates
    
    currentInstance->lastX = xpos;
    currentInstance->lastY = ypos;
    
    // only rotate camera if in free-look mode or right mouse button is pressed
    if (currentInstance->freeLookMode || 
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        currentInstance->camera->rotate(xoffset, yoffset);
    }
}

void Simulation::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (!currentInstance || !currentInstance->camera) return;
    
    // zoom in/out with scroll wheel
    currentInstance->camera->moveForward(yoffset * 2.0f);
}

void Simulation::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (!currentInstance) return;
    
    // exit on ESC
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    // toggle free-look mode with F key
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        currentInstance->freeLookMode = !currentInstance->freeLookMode;
        
        if (currentInstance->freeLookMode) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}