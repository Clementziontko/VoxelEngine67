#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <limits>
#include <algorithm>

#include "mesh.h"
#include "meshEasy.h"
#include "ShaderManager.h"

class BoundingBox {
public:
    BoundingBox(std::vector<Mesh>& meshes);

    void Draw(ShaderManager& shader, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& backpackModel);
    
    bool CheckCollision(const glm::vec3& camPos, const glm::vec3& camFront);

private:
    MeshEasy collisionBox;
    glm::vec3 MinVec;
    glm::vec3 MaxVec;
    std::vector<glm::vec3> narozniki;
    std::vector<float> narozniki_float;
    std::vector<unsigned int> indices;

    void iteracjaPoModelu(std::vector<Mesh>& meshes);
    void tworzenienaroznikow();
    void naroznikiNaFloat();
    bool rayAABB(const glm::vec3& camPos, const glm::vec3& camFront);
};
