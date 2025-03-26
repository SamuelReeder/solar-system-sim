#pragma once
#include <memory>
#include <string>
#include <glm/glm.hpp>

class Shader;
class Sphere;

class CelestialBody {
public:
    CelestialBody(const std::string& name, float radius, float orbitRadius, 
                  float orbitPeriod, float rotationPeriod);
    
    void initialize();
    void update(float deltaTime);
    void render(Shader& shader);
    
    void setOrbitCenter(const glm::vec3& center);
    void setParent(std::shared_ptr<CelestialBody> parent);
    glm::vec3 getPosition() const;
    float getRadius() const;
    const std::string& getName() const { return name; }
    
private:
    std::string name;
    float radius;
    float orbitRadius;
    float orbitPeriod;     // in days
    float rotationPeriod;  // in days
    
    float orbitAngle;
    float rotationAngle;
    
    glm::vec3 position;
    glm::vec3 orbitCenter;
    
    std::weak_ptr<CelestialBody> parent;
    std::shared_ptr<Sphere> mesh;
};