#pragma once
#include <vector>
#include <memory>
#include <string>

class CelestialBody;
class Shader;

class SolarSystem {
public:
    SolarSystem();
    
    void initialize();
    void update(float deltaTime);
    void render(Shader& shader);
    
    // methods for adding bodies
    void addStar(const std::string& name, float radius, float rotationPeriod);
    void addPlanet(const std::string& name, float radius, float orbitRadius, 
                  float orbitPeriod, float rotationPeriod);
    void addMoon(const std::string& planetName, const std::string& name, 
                float radius, float orbitRadius, float orbitPeriod, float rotationPeriod);
    
private:
    std::vector<std::shared_ptr<CelestialBody>> bodies;
    std::shared_ptr<CelestialBody> findBodyByName(const std::string& name);
};