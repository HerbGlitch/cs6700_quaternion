#pragma once

#include <GL/glew.h>
#include <stb/stb_image.h>

namespace herbglitch {
    class Texture {
    public:
        Texture();
        Texture(char* filePath);
        ~Texture();

        bool load();
        bool loadAlpha();
        void use();
        void clear();

    private:
        GLuint id;
        int width, height, bitDepth;

        char* filePath;
    };
}