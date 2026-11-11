#include "ShaderManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Constructor: compile and link shader program from file paths
ShaderManager::ShaderManager(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    if (geometryPath == nullptr) {
        kompilacjaShaderow(vertexPath, fragmentPath);
    } else {
        kompilacjaShaderow(vertexPath, fragmentPath, geometryPath);
    }
}

// Destructor: delete shader program if allocated
ShaderManager::~ShaderManager() {
    if (shaderProgram != 0) {
        glDeleteProgram(shaderProgram);
    }
}

// Activate the shader program
void ShaderManager::shaderRun() {
    glUseProgram(shaderProgram);
}

// Uniform setters
void ShaderManager::uniformSetInt(const char* location, int int_to_set) {
    glUniform1i(glGetUniformLocation(shaderProgram, location), int_to_set);
}

void ShaderManager::uniformSetFloat(const char* location, float float_to_set) {
    glUniform1f(glGetUniformLocation(shaderProgram, location), float_to_set);
}

void ShaderManager::uniformSetMat(const char* location, const glm::mat4& mat_to_set) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, location), 1, GL_FALSE, glm::value_ptr(mat_to_set));
}

void ShaderManager::uniformSetVec3(const char* location, const glm::vec3& vec3_to_set) {
    glUniform3fv(glGetUniformLocation(shaderProgram, location), 1, glm::value_ptr(vec3_to_set));
}

// Compile vertex, fragment, and optional geometry shaders, then link them
void ShaderManager::kompilacjaShaderow(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
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

    // Vertex Shader compilation and error checking
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "BLAD::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Geometry Shader compilation and error checking (if provided)
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

    // Fragment Shader compilation and error checking
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "BLAD::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl; // dopasowane do oryginału
    }

    // Shader Program creation, attachment, and linking
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    
    if (geometryPath != nullptr) {
        glAttachShader(shaderProgram, geometryShader);
    }
    
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Linking error checking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "BLAD::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Cleanup individual shaders after successful linking
    glDeleteShader(vertexShader);
    if (geometryPath != nullptr) {
        glDeleteShader(geometryShader);
    }
    glDeleteShader(fragmentShader);
}

// Utility function to read shader source code from a file
std::string ShaderManager::wczytaj_plik_szadera(const char* nazwaPliku) {
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