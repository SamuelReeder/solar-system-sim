#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, float aspectRatio)
    : position(0.0f, 0.0f, 10.0f),
      front(0.0f, 0.0f, -1.0f),
      up(0.0f, 1.0f, 0.0f),
      worldUp(0.0f, 1.0f, 0.0f),
      right(1.0f, 0.0f, 0.0f),
      yaw(-90.0f),
      pitch(0.0f),
      fov(fov),
      aspectRatio(aspectRatio),
      nearPlane(0.1f),
      farPlane(1000.0f),
      movementSpeed(5.0f),
      mouseSensitivity(0.1f) {
    updateVectors();
}

void Camera::moveForward(float distance) {
    position += front * distance;
}

void Camera::moveRight(float distance) {
    position += right * distance;
}

void Camera::moveUp(float distance) {
    position += worldUp * distance;  // use worldUp instead of camera up
}

void Camera::rotate(float deltaYaw, float deltaPitch) {
    yaw += deltaYaw * mouseSensitivity;
    pitch += deltaPitch * mouseSensitivity;
    
    // constrain pitch to avoid flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::setPosition(const glm::vec3& position) {
    this->position = position;
}

void Camera::setTarget(const glm::vec3& target) {
    if (target != position) {
        front = glm::normalize(target - position);
        // recalculate yaw and pitch
        pitch = glm::degrees(asin(front.y));
        yaw = glm::degrees(atan2(front.z, front.x));
        updateVectors();
    }
}

void Camera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
}

void Camera::updateVectors() {
    // calculate front vector from yaw and pitch
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    
    // recalculate right and up vectors
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}