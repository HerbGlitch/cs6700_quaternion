#include "texture.hpp"

namespace herbglitch {
    Texture::Texture(): id(0), width(0), height(0), bitDepth(0), filePath((char *)"") {}
    Texture::Texture(char* filePath): id(0), width(0), height(0), bitDepth(0), filePath(filePath) {}
    Texture::~Texture(){ clear(); }

    bool Texture::load(){
        unsigned char *texData = stbi_load(filePath, &width, &height, &bitDepth, 0);

        if(!texData){
            printf("failed to find %s\n", filePath);
            return false;
        }

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(texData);
        return true;
    }

    bool Texture::loadAlpha(){
        unsigned char *texData = stbi_load(filePath, &width, &height, &bitDepth, 0);

        if(!texData){
            printf("failed to find %s\n", filePath);
            return false;
        }

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(texData);
        return true;
    }

    void Texture::use(){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture::clear(){
        glDeleteTextures(1, &id);
        id = 0;
        width = 0;
        height = 0;
        bitDepth = 0;
        filePath = 0;
    }
}