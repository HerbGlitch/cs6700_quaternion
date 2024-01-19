#include "camera.hpp"

namespace herbglitch {
    Camera::Camera(){}
    Camera::Camera(glm::vec3 position, glm::vec3 worldUp, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed): position(position), worldUp(worldUp), yaw(yaw), pitch(pitch), front(glm::vec3(0.0f, 0.0f, -1.0f)), moveSpeed(moveSpeed), turnSpeed(turnSpeed){ update(); }
    Camera::~Camera(){}

    void Camera::update(){
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); 
        front.y = sin(glm::radians(pitch)); 
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)); 
        front = glm::normalize(front);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    void Camera::keyControl(bool* keys, GLfloat dt){
        GLfloat velocity = moveSpeed * dt;

        if(keys[GLFW_KEY_W]){ position += front * velocity; }
        if(keys[GLFW_KEY_S]){ position -= front * velocity; }
        if(keys[GLFW_KEY_A]){ position -= right * velocity; }
        if(keys[GLFW_KEY_D]){ position += right * velocity; }
    }

    void Camera::mouseControl(GLfloat xChange, GLfloat yChange){
        xChange *= turnSpeed;
        yChange *= turnSpeed;

        yaw += xChange;
        pitch += yChange;

        if(pitch > 89.0f){ pitch = 89.0f; }
        if(pitch < -89.0f){ pitch = -89.0f; }

        update();
    }

    glm::mat4 Camera::calculateViewMatrix(){ return glm::lookAt(position, position + front, up); }

}