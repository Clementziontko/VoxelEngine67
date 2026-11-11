#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "mesh.h"
#include "ShaderManager.h"

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model {
public:
    std::vector<Texture> textures_loaded; // Stores loaded textures to prevent duplicate loading
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    // Constructor: expects a filepath to a 3D model
    Model(std::string const &path, bool gamma = false);

    // Draw the model and all its meshes
    void Draw(ShaderManager &shader);
    
private:
    // Load model with supported Assimp extensions from file
    void loadModel(std::string const &path);

    // Process an Assimp node recursively
    void processNode(aiNode *node, const aiScene *scene);

    // Process an individual mesh from Assimp
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    // Check material textures of a given type and load them if not loaded yet
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif