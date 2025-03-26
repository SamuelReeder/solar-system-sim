#pragma once
#include "Mesh.h"

class Sphere : public Mesh {
public:
    Sphere(float radius = 1.0f, unsigned int segments = 36, unsigned int rings = 18);
    
    void initialize() override;
    float getRadius() const { return radius; }
    
private:
    float radius;
    unsigned int segments;
    unsigned int rings;
    
    void generateSphere();
};