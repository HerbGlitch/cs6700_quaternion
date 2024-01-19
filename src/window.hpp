#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace herbglitch {
    class Window {
    public:
        Window();
        Window(GLint width, GLint height);
        ~Window();

        int init();

        bool getShouldClose(){ return (bool)glfwWindowShouldClose(main); }
        void swapBuffers(){ glfwSwapBuffers(main); }

        GLfloat getBufferWidth(){ return (GLfloat)bufferWidth; }
        GLfloat getBufferHeight(){ return (GLfloat)bufferHeight; }
        bool* getKeys(){ return keys; }
        GLfloat getXChange();
        GLfloat getYChange();

    private:
        void createCallbacks();
        static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
        static void handleMouse(GLFWwindow* window, double xPos, double yPos);

        GLFWwindow *main;

        GLint width, height;
        GLint bufferWidth, bufferHeight;

        bool keys[1024];

        GLfloat lastX, lastY;
        GLfloat xChange, yChange;
        bool mouseFirstMoved;
    };
}