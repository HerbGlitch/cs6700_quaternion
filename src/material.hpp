#pragma once

#include <GL/glew.h>

namespace herbglitch {
    class Material {
    public:
        Material();
        Material(GLfloat specularIntensity, GLfloat shininess);
        ~Material();

        void use(GLuint specularIntensityLocation, GLuint shininessLocation);

    private:
        GLfloat specularIntensity;
        GLfloat shininess;
    };
}