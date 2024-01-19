#include "material.hpp"

namespace herbglitch {
    Material::Material(): specularIntensity(0.0f), shininess(0.0f){}
    Material::Material(GLfloat specularIntensity, GLfloat shininess): specularIntensity(specularIntensity), shininess(shininess){}
    Material::~Material(){}

    void Material::use(GLuint specularIntensityLocation, GLuint shininessLocation){
        glUniform1f(specularIntensityLocation, specularIntensity);
        glUniform1f(shininessLocation, shininess);
    }

}