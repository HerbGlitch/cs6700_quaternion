#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "commonValues.hpp"

#include "window.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "material.hpp"

#include "light/directional.hpp"
#include "light/point.hpp"
#include "light/spot.hpp"

using namespace herbglitch;

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader *> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

light::Directional mainLight;
light::Point pointLights[MAX_POINT_LIGHTS];
light::Spot spotLights[MAX_SPOT_LIGHTS];

GLfloat dt = 0.0f;
GLfloat lastTime = 0.0f;

//vertex shader
static const char* vShader = "src/shaders/shader.vert";

//fragment shader
static const char* fShader = "src/shaders/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset){
    for(size_t i = 0; i < indiceCount; i += 3){
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

        vertices[in0    ] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;

        vertices[in1    ] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;

        vertices[in2    ] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;
    }

    for(size_t i = 0; i < verticeCount / vLength; i++){
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);

        vertices[nOffset    ] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}

void createObjects() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat verticies[] = {
    //  x      y      z       u     v      nx    ny    nz
        -1.0f, -1.0f, -0.6f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.0f, -1.0f,  1.0f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.0f,  1.0f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,   0.0f,  0.0f,  0.0f, -1.0f, 0.0f,
         10.0f, 0.0f, -10.0f,  10.0f,  0.0f,  0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f,  10.0f,   0.0f, 10.0f,  0.0f, -1.0f, 0.0f,
         10.0f, 0.0f,  10.0f,  10.0f, 10.0f,  0.0f, -1.0f, 0.0f
    };

    calcAverageNormals(indices, 12, verticies, 32, 8, 5);

    Mesh *obj1 = new Mesh();
    obj1->create(verticies, indices, 32, 12);
    meshList.push_back(obj1);

    Mesh *obj2 = new Mesh();
    obj2->create(verticies, indices, 32, 12);
    meshList.push_back(obj2);

    Mesh *obj3 = new Mesh();
    obj3->create(floorVertices, floorIndices, 32, 6);
    meshList.push_back(obj3);
}

void createShaders(){
    Shader *shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader);
    shaderList.push_back(shader1);
}

int main(){
    mainWindow = Window(1920, 1080);
    mainWindow.init();

    createObjects();
    createShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

    brickTexture = Texture((char *)"src/textures/brick_s.png");
    brickTexture.load();

    dirtTexture = Texture((char *)"src/textures/dirt_s.png");
    dirtTexture.load();

    plainTexture = Texture((char *)"src/textures/plain.png");
    plainTexture.load();

    shinyMaterial = Material(4.0f, 256.0f);
    dullMaterial = Material(0.3f, 4.0f);


    mainLight = light::Directional(1.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.0f, 0.0f, -1.0f);

    unsigned int pointLightCount = 0;

    pointLights[0] = light::Point(0.0f, 0.0f, 1.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.3f, 0.2f, 0.1f);
    // pointLightCount++;

    pointLights[1] = light::Point(0.0f, 1.0f, 0.0f, 0.0f, 0.1f, -4.0f, 2.0f, 0.0f, 0.3f, 0.1f, 0.1f);
    // pointLightCount++;

    unsigned int spotLightCount = 0;

    spotLights[0] = light::Spot(1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 20.0f);
    spotLightCount++;

    spotLights[1] = light::Spot(1.0f, 1.0f, 1.0f, 
                                0.0f, 1.0f, 
                                0.0f, -1.5f, 0.0f, 
                                -100.0f, -1.0f, 0.0f, 
                                1.0f, 0.0f, 0.0f, 20.0f);
    spotLightCount++;


    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePostion = 0;
    GLuint uniformSpecularIntensity = 0, uniformShininess = 0;
    glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.1f, 100.0f);

    //Loop until window closed
    while(!mainWindow.getShouldClose()){
        GLfloat now = glfwGetTime();
        dt = now - lastTime;
        lastTime = now;

        //get + handle user input events
        glfwPollEvents();

        camera.keyControl(mainWindow.getKeys(), dt);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        //clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList.at(0)->use();
        uniformModel = shaderList.at(0)->getModelLoation();
        uniformProjection = shaderList.at(0)->getProjectionLocation();
        uniformView = shaderList.at(0)->getViewLocation();
        uniformEyePostion = shaderList.at(0)->getEyePositionLocation();
        uniformSpecularIntensity = shaderList.at(0)->getSpecularIntensityLocation();
        uniformShininess = shaderList.at(0)->getShininessLocation();

        glm::vec3 lowerLight = camera.getPosition();
        lowerLight.y -= 0.3f;
        spotLights[0].setFlash(lowerLight, camera.getDirection());

        shaderList[0]->setDirectionalLight(&mainLight);
        shaderList[0]->setPointLights(pointLights, pointLightCount);
        shaderList[0]->setSpotLights(spotLights, spotLightCount);

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePostion, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        brickTexture.use();
        shinyMaterial.use(uniformSpecularIntensity, uniformShininess);
        meshList.at(0)->render();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
        // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.use();
        dullMaterial.use(uniformSpecularIntensity, uniformShininess);
        meshList.at(1)->render();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        plainTexture.use();
        shinyMaterial.use(uniformSpecularIntensity, uniformShininess);
        meshList.at(2)->render();

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}