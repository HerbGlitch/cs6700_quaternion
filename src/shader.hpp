#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "commonValues.hpp"

#include "light/directional.hpp"
#include "light/point.hpp"
#include "light/spot.hpp"

namespace herbglitch {
    class Shader {
    public:
        Shader();
        ~Shader();

        void createFromString(const char* vertexCode, const char* fragmentCode);
        void createFromFiles(const char* vertexPath, const char* fragmentPath);

        std::string readFile(const char* path);

        void use();
        void clear();

        void setDirectionalLight(light::Directional *dLight);
        void setPointLights(light::Point *pLight, unsigned int lightCount);
        void setSpotLights(light::Spot *sLight, unsigned int lightCount);

        GLuint getProjectionLocation(){ return uniformProjection; }
        GLuint getModelLoation(){ return uniformModel; }
        GLuint getViewLocation(){ return uniformView; }
        GLuint getAmbientIntensityLocation(){ return uniformDirectionalLight.uniformAmbientIntensity; }
        GLuint getAmbientColorLocation(){ return uniformDirectionalLight.uniformColor; }
        GLuint getDiffuseIntensityLocation(){ return uniformDirectionalLight.uniformDiffuseIntensity; }
        GLuint getDirectionLocation(){ return uniformDirectionalLight.uniformDirection; }
        GLuint getSpecularIntensityLocation(){ return uniformSpecularIntensity; }
        GLuint getShininessLocation(){ return uniformShininess; }
        GLuint getEyePositionLocation(){ return uniformEyePosition; }

    private:
        void compile(const char* vertexCode, const char* fragmentCode);
        void add(GLuint theProgram, const char* shaderCode, GLenum shaderType);

        struct {
            GLuint uniformColor;
            GLuint uniformAmbientIntensity;
            GLuint uniformDiffuseIntensity;

            GLuint uniformDirection;
        } uniformDirectionalLight;

        struct {
            GLuint uniformColor;
            GLuint uniformAmbientIntensity;
            GLuint uniformDiffuseIntensity;

            GLuint uniformPosition;
            GLuint uniformConstant;
            GLuint uniformLinear;
            GLuint uniformExponent;
        } uniformPointLight[MAX_POINT_LIGHTS];

        GLuint uniformSpotLightCount;

        struct {
            GLuint uniformColor;
            GLuint uniformAmbientIntensity;
            GLuint uniformDiffuseIntensity;

            GLuint uniformPosition;
            GLuint uniformConstant;
            GLuint uniformLinear;
            GLuint uniformExponent;

            GLuint uniformDirection;
            GLuint uniformEdge;
        } uniformSpotLight[MAX_SPOT_LIGHTS];

        int pointLightCount;
        int spotLightCount;

        GLuint id, uniformProjection, uniformModel, uniformView, uniformEyePosition;
        GLuint uniformSpecularIntensity, uniformShininess;
        GLuint uniformPointLightCount;
    };
}