#pragma once 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>


constexpr unsigned int numberOfPlanes = 6;


class Frustum{
    public:

        Frustum(){}
        
        bool CheckIfChunkIsToCull(const glm::ivec3& chunkPosition);

        const glm::mat4& GetPV() const { return pvMatrix; }

        void UpdatePV(const glm::mat4& projection, const glm::mat4& view); 

    private:
        glm::mat4 pvMatrix;

        float CountingMatricesResult(const glm::vec4& plane, const glm::ivec3& minVec, const glm::ivec3& maxVec);

        float matricesEquazion(const glm::vec4& plane, const glm::vec3& positiveVertex);

};
