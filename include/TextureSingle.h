#pragma once

#ifndef TEXTURE_SINGLE_H
#define TEXTURE_SINGLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class TextureSingle {
public:
    TextureSingle(const char* texturPath_c, int textureNumber_c, bool gamma_c = false);

    TextureSingle(const TextureSingle&) = delete;
    TextureSingle& operator=(const TextureSingle&) = delete;

    ~TextureSingle();

    void inicjalizacja();
    void load();
    void textureBind();

private:
    const char* texturPath;
    unsigned int ID;
    int width, height, nrChannels, textureNumber;
    unsigned char* data;
    bool gamma;
};

#endif