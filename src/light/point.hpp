#pragma once

#include "light.hpp"

namespace herbglitch {
    namespace light {
        class Point : public Light {
        public:
            Point();
            Point(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat constant, GLfloat linear, GLfloat exponent);
            ~Point();

            void use(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

        protected:
            glm::vec3 position;

            GLfloat constant, linear, exponent;
        };
    }
}