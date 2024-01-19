#pragma once

#include "light.hpp"

namespace herbglitch {
    namespace light {
        class Directional : public Light {
        public:
            Directional();
            Directional(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir);
            ~Directional();

            void use(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);

        private:
            glm::vec3 direction;
        };
    }
}