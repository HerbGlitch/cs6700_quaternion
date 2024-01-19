#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace herbglitch {
    class Camera {
    public:
        Camera();
        Camera(glm::vec3 position, glm::vec3 worldUp, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed);
        ~Camera();

        void keyControl(bool* keys, GLfloat dt);
        void mouseControl(GLfloat xChange, GLfloat yChange);

        glm::mat4 calculateViewMatrix();

        glm::vec3 getPosition(){ return position; }
        glm::vec3 getDirection(){ return glm::normalize(front); }

    private:
        void update();

        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        GLfloat yaw, pitch;
        GLfloat moveSpeed, turnSpeed;
    };
}