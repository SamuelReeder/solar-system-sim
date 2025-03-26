#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    Camera(float fov = 45.0f, float aspectRatio = 16.0f/9.0f);
    
    // movement methods
    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void rotate(float yaw, float pitch);
    
    // getters for matrices
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    
    // setters
    void setPosition(const glm::vec3& position);
    void setTarget(const glm::vec3& target);
    void setAspectRatio(float aspectRatio);
    
    // getters for position/orientation
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }
    glm::vec3 getUp() const { return up; }
    glm::vec3 getRight() const { return right; }
    
private:
    void updateVectors();
    
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;  // Usually (0,1,0)
    
    float yaw, pitch;
    float fov, aspectRatio;
    float nearPlane, farPlane;
    
    // camera options
    float movementSpeed;
    float mouseSensitivity;
};