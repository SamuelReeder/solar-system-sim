#include "CelestialBody.h"
#include "Sphere.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

CelestialBody::CelestialBody(const std::string& name, float radius, float orbitRadius, 
                             float orbitPeriod, float rotationPeriod)
    : name(name), 
      radius(radius), 
      orbitRadius(orbitRadius), 
      orbitPeriod(orbitPeriod),
      rotationPeriod(rotationPeriod),
      orbitAngle(0.0f),
      rotationAngle(0.0f),
      position(0.0f),
      orbitCenter(0.0f),
      mesh(std::make_shared<Sphere>(radius)) {
}

void CelestialBody::initialize() {
    mesh->initialize();
}

void CelestialBody::update(float deltaTime) {
    // update orbit position (convert days to seconds for simulation)
    if (orbitPeriod > 0.0f) {
        // 2π / (orbital period in days * day in seconds) * delta time
        float angleSpeed = (2.0f * glm::pi<float>()) / (orbitPeriod * 86400.0f);
        orbitAngle += angleSpeed * deltaTime;
        
        // update position based on orbit
        position.x = orbitCenter.x + orbitRadius * cos(orbitAngle);
        position.z = orbitCenter.z + orbitRadius * sin(orbitAngle);
        position.y = orbitCenter.y; // assuming orbit is on xz plane
    } else {
        // no orbit, body stays at center
        position = orbitCenter;
    }
    
    // update rotation
    if (rotationPeriod > 0.0f) {
        float rotationSpeed = (2.0f * glm::pi<float>()) / (rotationPeriod * 86400.0f);
        rotationAngle += rotationSpeed * deltaTime;
    }
    
    // update model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    mesh->setModelMatrix(model);
}

void CelestialBody::render(Shader& shader) {
    mesh->render(shader);
}

void CelestialBody::setOrbitCenter(const glm::vec3& center) {
    orbitCenter = center;
}

glm::vec3 CelestialBody::getPosition() const {
    return position;
}

float CelestialBody::getRadius() const {
    return radius;
}