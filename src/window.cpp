#include "window.hpp"

namespace herbglitch {
    Window::Window(): width(800), height(600), mouseFirstMoved(true), xChange(0.0f), yChange(0.0f){
        for(size_t i = 0; i < 1024; i++){ keys[i] = false; }
    }

    Window::Window(GLint width, GLint height): width(width), height(height), mouseFirstMoved(true), xChange(0.0f), yChange(0.0f){
        for(size_t i = 0; i < 1024; i++){ keys[i] = false; }
    }

    Window::~Window(){
        glfwDestroyWindow(main);
        glfwTerminate();
    }

    int Window::init(){
        //Initialise GLFW
        if(!glfwInit()){
            printf("GLFW initialisation failed!\n");
            glfwTerminate();
            return 1;
        }

        //Setup GLFW window properties
        //OpenGl version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //core profile - no backwards compatibility
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //allow forward compatibility
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        main = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
        if(!main){
            printf("GLFW window creation failed!\n");
            glfwTerminate();
            return 1;
        }

        //get buffer size information
        glfwGetFramebufferSize(main, &bufferWidth, &bufferHeight);

        //set context for GLEW to use
        glfwMakeContextCurrent(main);

        //handle key + mouse input
        createCallbacks();
        glfwSetInputMode(main, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        //allow modern extension features
        glewExperimental = GL_TRUE;

        if(glewInit() != GLEW_OK){
            printf("GLEW initialisation failed!\n");
            glfwDestroyWindow(main);
            glfwTerminate();
            return 1;
        }

        glEnable(GL_DEPTH_TEST);

        //setup viewport size
        glViewport(0, 0, bufferWidth, bufferHeight);

        glfwSetWindowUserPointer(main, this);

        return 0;
    }

    void Window::createCallbacks(){
        glfwSetKeyCallback(main, handleKeys);
        glfwSetCursorPosCallback(main, handleMouse);
    }

    void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode){
        Window* theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){ glfwSetWindowShouldClose(window, GL_TRUE); }
        if(key >= 0 && key < 1024){
            if(action == GLFW_PRESS){ theWindow->keys[key] = true; }
            else if(action == GLFW_RELEASE){ theWindow->keys[key] = false; }
        }
    }

    void Window::handleMouse(GLFWwindow* window, double xPos, double yPos){
        Window* theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

        if(theWindow->mouseFirstMoved){
            theWindow->lastX = xPos;
            theWindow->lastY = yPos;
            theWindow->mouseFirstMoved = false;
        }

        theWindow->xChange = xPos - theWindow->lastX;
        theWindow->yChange = theWindow->lastY - yPos; //not inverted

        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
    }

    GLfloat Window::getXChange(){
        GLfloat theChange = xChange;
        xChange = 0.0f;
        return theChange;
    }

    GLfloat Window::getYChange(){
        GLfloat theChange = yChange;
        yChange = 0.0f;
        return theChange;
    }
}