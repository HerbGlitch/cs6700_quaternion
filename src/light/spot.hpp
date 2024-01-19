#pragma once
#include "point.hpp"

namespace herbglitch {
    namespace light {
        class Spot : public Point {
        public:
            Spot();
            Spot(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat edge);
            ~Spot();

            void use(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint edgeLocation);
            void setFlash(glm::vec3 position, glm::vec3 direction);

        private:
            glm::vec3 direction;

            GLfloat edge, procEdge;
        };
    }
}