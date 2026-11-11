#pragma once

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class ShaderManager {
public:
    unsigned int shaderProgram = 0;

    ShaderManager(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

    ~ShaderManager();

    void shaderRun();
    void uniformSetInt(const char* location, int int_to_set);
    void uniformSetFloat(const char* location, float float_to_set);
    void uniformSetMat(const char* location, const glm::mat4& mat_to_set);
    void uniformSetVec3(const char* location, const glm::vec3& vec3_to_set);

private:
    void kompilacjaShaderow(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    std::string wczytaj_plik_szadera(const char* nazwaPliku);
};

#endif