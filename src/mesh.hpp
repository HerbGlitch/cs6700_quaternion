#pragma once

#include <GL/glew.h>

namespace herbglitch{
    class Mesh {
    public:
        Mesh();
        ~Mesh();

        void create(GLfloat *vertices, unsigned int *indices, unsigned int numOfVerticies, unsigned int numOfIndices);
        void render();
        void clear();

    private:
        GLuint VAO, VBO, IBO;
        GLsizei indexCount;
    };
}
