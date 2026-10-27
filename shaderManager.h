#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>



class ShaderManager{
    public:
        unsigned int shaderProgram = 0;
        ShaderManager(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr){
            if(geometryPath == nullptr){
                kompilacjaShaderow(vertexPath, fragmentPath);
            }
            else{
                kompilacjaShaderow(vertexPath, fragmentPath, geometryPath);
            }
        }

        ShaderManager(const ShaderManager&) = delete;
        ShaderManager& operator=(const ShaderManager&) = delete;

        ~ShaderManager(){
            if(shaderProgram != 0) {glDeleteProgram(shaderProgram);}
        }

        void shaderRun(){
            glUseProgram(shaderProgram);
        }
       
        void uniformSetInt(const char* location, int int_to_set){
            glUniform1i(glGetUniformLocation(shaderProgram, location), int_to_set);
        }

        void uniformSetFloat(const char* location, float float_to_set){
            glUniform1f(glGetUniformLocation(shaderProgram, location), float_to_set);
        }

        void uniformSetMat(const char* location, const glm::mat4& mat_to_set){
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, location), 1, GL_FALSE, glm::value_ptr(mat_to_set));
        }

        void uniformSetVec3(const char* location, const glm::vec3& vec3_to_set){
            glUniform3fv(glGetUniformLocation(shaderProgram, location), 1, glm::value_ptr(vec3_to_set));
        }


    private:

        void kompilacjaShaderow(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) {
            // 1. Wczytywanie kodu shaderów z plików
            std::string vertexCode = wczytaj_plik_szadera(vertexPath);
            std::string fragmentCode = wczytaj_plik_szadera(fragmentPath);
            
            std::string geometryCode;
            const char* vertexShaderSource = vertexCode.c_str();
            const char* fragmentShaderSource = fragmentCode.c_str();
            const char* geometryShaderSource = nullptr;

            if (geometryPath != nullptr) {
                geometryCode = wczytaj_plik_szadera(geometryPath);
                geometryShaderSource = geometryCode.c_str();
            }

            int success;
            char infoLog[512];

            // 2. Vertex Shader - kompilacja i sprawdzenie błędów
            unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);

            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                std::cout << "BLAD::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            // 3. Geometry Shader - kompilacja i sprawdzenie błędów (jeśli ścieżka została podana)
            unsigned int geometryShader = 0;
            if (geometryPath != nullptr) {
                geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
                glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
                glCompileShader(geometryShader);

                glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
                    std::cout << "BLAD::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
                }
            }

            // 4. Fragment Shader - kompilacja i sprawdzenie błędów
            unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);

            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                std::cout << "BLAD::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            // 5. Tworzenie Programu Shaderów, dołączanie i linkowanie
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            
            if (geometryPath != nullptr) {
                glAttachShader(shaderProgram, geometryShader);
            }
            
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);

            // Sprawdzenie błędów linkowania programu
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                std::cout << "BLAD::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }

            // 6. Czyszczenie – usuwamy shadery po ich pomyślnym zlinkowaniu do programu
            glDeleteShader(vertexShader);
            if (geometryPath != nullptr) {
                glDeleteShader(geometryShader);
            }
            glDeleteShader(fragmentShader);
        }

        std::string wczytaj_plik_szadera(const char* nazwaPliku) {
            std::ifstream plik(nazwaPliku, std::ios::ate | std::ios::binary);

            if (!plik.is_open()) {
                std::cerr << "BLAD: Nie udało się otworzyć pliku shadera: " << nazwaPliku << "\n";
                throw std::runtime_error("Blad otwierania shadera");
            }

            size_t rozmiarPliku = static_cast<size_t>(plik.tellg());
            
            plik.seekg(0, std::ios::beg);

            std::string bufor;
            bufor.resize(rozmiarPliku);

            plik.read(&bufor[0], rozmiarPliku);
            
            return bufor;
        }

};


