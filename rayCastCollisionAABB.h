#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

struct RayAABBResultBlock {
    bool hit;
    glm::vec3 hitPos;
    glm::vec3 ktoraŚciana;
    float distance;
};

struct RayAABBResultChunk{
    bool hit;
    float distance;

};

class RayCastCollision{
    public:
        RayAABBResultChunk rayAABBChunk(const glm::vec3& camPos, const glm::vec3& camFront, const glm::ivec3& chunkPos);

        RayAABBResultBlock rayAABBBlock(const glm::vec3& camPos, const glm::vec3& camFront, const glm::ivec3& pos);

    private:
        void maxIminVecBlock(const glm::ivec3& pos, glm::ivec3& MinVec, glm::ivec3& MaxVec);

        void maxIminVecChunk(const glm::ivec3& chunkPos, glm::ivec3& MinVec, glm::ivec3& MaxVec);
};