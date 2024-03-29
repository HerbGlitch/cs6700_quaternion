#include "spot.hpp"

namespace herbglitch {
    namespace light {
        Spot::Spot(): direction(glm::vec3(0.0f, -1.0f, 0.0f)), procEdge(cosf(glm::radians(edge))), Point(){}
        Spot::Spot(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat edge): direction(glm::normalize(glm::vec3(xDir, yDir, zDir))), edge(edge), procEdge(cosf(glm::radians(edge))), Point(red, green, blue, ambientIntensity, diffuseIntensity, xPos, yPos, zPos, constant, linear, exponent){}
        Spot::~Spot(){}

        void Spot::use(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint edgeLocation){
            glUniform3f(ambientColorLocation, color.x, color.y, color.z);
            glUniform1f(ambientIntensityLocation, ambientIntensity);
            glUniform1f(diffuseIntensityLocation, diffuseIntensity);

            glUniform3f(positionLocation, position.x, position.y, position.z);
            glUniform1f(constantLocation, constant);
            glUniform1f(linearLocation, linear);
            glUniform1f(exponentLocation, exponent);

            glUniform3f(directionLocation, direction.x, direction.y, direction.z);
            glUniform1f(edgeLocation, procEdge);
        }

        void Spot::setFlash(glm::vec3 position, glm::vec3 direction){
            this->position = position;
            this->direction = direction;
        }
    }
}