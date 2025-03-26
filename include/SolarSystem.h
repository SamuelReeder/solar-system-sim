#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

class CelestialBody;
class Shader;

class SolarSystem {
public:
    SolarSystem();
    
    void initialize();
    void update(float deltaTime);
    void render(Shader& shader);
    
    // separate render methods for shadow mapping and different shaders
    void renderDepth(Shader& shadowShader);
    void renderPlanets(Shader& planetShader);
    void renderSun(Shader& sunShader);
    
    // add bodies
    void addStar(const std::string& name, float radius, float rotationPeriod);
    void addPlanet(const std::string& name, float radius, float orbitRadius, 
                  float orbitPeriod, float rotationPeriod);
    void addMoon(const std::string& planetName, const std::string& name, 
                float radius, float orbitRadius, float orbitPeriod, float rotationPeriod);
    
    // get sun position for lighting
    glm::vec3 getSunPosition() const;
    
private:
    std::vector<std::shared_ptr<CelestialBody>> bodies;
    std::shared_ptr<CelestialBody> sun;  // direct access to the sun
    std::shared_ptr<CelestialBody> findBodyByName(const std::string& name);
};