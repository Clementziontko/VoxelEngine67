#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "shaderManager.h"

class BlockBoundingBox {
public:
    BlockBoundingBox();
    ~BlockBoundingBox();

    void DrawGeometryShader(ShaderManager& shader, const glm::vec3& pos);
    void DrawStencilTest(ShaderManager& shader, ShaderManager& shaderBoundingBox, const glm::vec3& pos, unsigned int VAO);
    void Draw(ShaderManager& shader, const glm::vec3& pos);

private:
    unsigned int VBO, VAO, EBO, geometryVAO;
    glm::vec3 blockPos;
    glm::vec3 MinVec;
    glm::vec3 MaxVec;
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};