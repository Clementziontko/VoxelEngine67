#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>

class CubeMapTexture{
    public:
    CubeMapTexture(const char* texturPath_c, int textureNumber_c, bool gamma_c = false);

    CubeMapTexture(const CubeMapTexture&) = delete;
    CubeMapTexture& operator=(const CubeMapTexture&) = delete;

    ~CubeMapTexture();

    void inicjalizacja();
    void load();
    void textureBind();

    private:

    const char* texturPath;
    unsigned int ID;
    int width, height, nrChannels, textureNumber;
    unsigned char* data;
    bool gamma;
int offsets[6][2] = {
    {2, 1}, // GL_TEXTURE_CUBE_MAP_POSITIVE_X (Right)
    {0, 1}, // GL_TEXTURE_CUBE_MAP_NEGATIVE_X (Left)
    {1, 0}, // GL_TEXTURE_CUBE_MAP_POSITIVE_Y (Top)
    {1, 2}, // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y (Bottom)
    {1, 1}, // GL_TEXTURE_CUBE_MAP_POSITIVE_Z (Front) - ze słońcem
    {3, 1}  // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z (Back)
};



};