#include "frustum.h"
#include <cmath>
#include<unordered_map>
#include <glm/gtc/matrix_access.hpp>


void Frustum::UpdatePV(const glm::mat4& projection, const glm::mat4& view){
    pvMatrix = projection * view;
}

float Frustum::matricesEquazion(const glm::vec4& plane, const glm::vec3& positiveVertex){
    return plane.x * positiveVertex.x + plane.y * positiveVertex.y + plane.z * positiveVertex.z + plane.w;
}   


float Frustum::CountingMatricesResult(const glm::vec4& plane, const glm::ivec3& minVec, const glm::ivec3& maxVec){
    glm::vec3 positiveVertex;
    for(unsigned int i = 0 ; i < 3 ; i++){
        if(plane[i] >= 0){positiveVertex[i] = maxVec[i];}else{positiveVertex[i] = minVec[i];}
    }
    return matricesEquazion(plane, positiveVertex);
}


bool Frustum::CheckIfChunkIsToCull(const glm::ivec3& chunkPosition){
    std::vector<glm::vec4> planes;
    planes.reserve(numberOfPlanes);
    glm::mat4 pv = GetPV();
    glm::ivec3 minVec = chunkPosition * 16;
    glm::ivec3 maxVec = minVec + glm::ivec3(16);
    glm::vec4 row0 = glm::vec4(pv[0][0], pv[1][0], pv[2][0], pv[3][0]);
    glm::vec4 row1 = glm::vec4(pv[0][1], pv[1][1], pv[2][1], pv[3][1]);
    glm::vec4 row2 = glm::vec4(pv[0][2], pv[1][2], pv[2][2], pv[3][2]);
    glm::vec4 row3 = glm::vec4(pv[0][3], pv[1][3], pv[2][3], pv[3][3]);

    planes.emplace_back(row3 + row0);
    planes.emplace_back(row3 - row0);

    planes.emplace_back(row3 + row1);
    planes.emplace_back(row3 - row1);

    planes.emplace_back(row3 + row2);
    planes.emplace_back(row3 - row2);
    


    for(const auto& Plane : planes){
        float Result = CountingMatricesResult(Plane, minVec, maxVec);
        if(Result < 0){
            return true;
        }
    }
    return false;
}

