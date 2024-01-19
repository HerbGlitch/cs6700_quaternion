#include "directional.hpp"

namespace herbglitch {
    namespace light {
        Directional::Directional(): Light(), direction(glm::vec3(0.0f, -1.0f, 0.0f)){}
        Directional::Directional(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir): Light(red, green, blue, ambientIntensity, diffuseIntensity), direction(glm::vec3(xDir, yDir, zDir)){}
        Directional::~Directional(){}

        void Directional::use(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation){
            glUniform3f(ambientColorLocation, color.x, color.y, color.z);
            glUniform1f(ambientIntensityLocation, ambientIntensity);

            glUniform3f(directionLocation, direction.x, direction.y, direction.z);
            glUniform1f(diffuseIntensityLocation, diffuseIntensity);
        }

    }
}