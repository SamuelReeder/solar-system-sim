#include "SolarSystem.h"
#include "CelestialBody.h"
#include "Shader.h"
#include <iostream>

SolarSystem::SolarSystem() {
}

void SolarSystem::initialize() {
    for (auto& body : bodies) {
        body->initialize();
    }
}

void SolarSystem::update(float deltaTime) {
    for (auto& body : bodies) {
        body->update(deltaTime);
    }
}

void SolarSystem::render(Shader& shader) {
    for (auto& body : bodies) {
        body->render(shader);
    }
}

void SolarSystem::addStar(const std::string& name, float radius, float rotationPeriod) {
    auto star = std::make_shared<CelestialBody>(name, radius, 0.0f, 0.0f, rotationPeriod);
    star->setOrbitCenter(glm::vec3(0.0f));
    bodies.push_back(star);
}

void SolarSystem::addPlanet(const std::string& name, float radius, float orbitRadius, 
                            float orbitPeriod, float rotationPeriod) {
    auto planet = std::make_shared<CelestialBody>(name, radius, orbitRadius, orbitPeriod, rotationPeriod);
    planet->setOrbitCenter(glm::vec3(0.0f));
    bodies.push_back(planet);
}

void SolarSystem::addMoon(const std::string& planetName, const std::string& name, 
                          float radius, float orbitRadius, float orbitPeriod, float rotationPeriod) {
    auto planet = findBodyByName(planetName);
    if (!planet) {
        std::cerr << "error: cannot add moon to nonexistent planet " << planetName << std::endl;
        return;
    }
    
    auto moon = std::make_shared<CelestialBody>(name, radius, orbitRadius, orbitPeriod, rotationPeriod);
    moon->setOrbitCenter(planet->getPosition());
    bodies.push_back(moon);
}

std::shared_ptr<CelestialBody> SolarSystem::findBodyByName(const std::string& name) {
    for (auto& body : bodies) {
        if (body->getName() == name) {
            return body;
        }
    }
    return nullptr;
}