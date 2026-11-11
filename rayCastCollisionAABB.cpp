
#include "rayCastCollisionAABB.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>








            void RayCastCollision::maxIminVecChunk(const glm::ivec3& chunkPos, glm::ivec3& MinVec, glm::ivec3& MaxVec){
                MinVec = chunkPos * 16;
                MaxVec = MinVec + glm::ivec3(16);
            }
    RayAABBResultChunk RayCastCollision::rayAABBChunk(const glm::vec3& camPos, const glm::vec3& camFront, const glm::ivec3& chunkPos){
        glm::ivec3 minVec;
        glm::ivec3 maxVec;
        maxIminVecChunk(chunkPos, minVec, maxVec);
        float tmin = 0.0f;
        float tmax = std::numeric_limits<float>::infinity();

        for(unsigned int os = 0 ; os < 3 ; os++){
            if(std::abs(camFront[os]) < 1e-6f){
                if(!(minVec[os] <= camPos[os] && maxVec[os] >= camPos[os])){
                    return {false, 0.0f};
                }
            }else{
                float t1 = (minVec[os] - camPos[os]) / camFront[os];
                float t2 = (maxVec[os] - camPos[os]) / camFront[os];

                if(t1 > t2){std::swap(t1, t2);}

                tmin = std::max(tmin, t1);
                tmax = std::min(tmax, t2);

                if(tmin > tmax){
                    return {false, 0.0f};
                }
            }
        }
        if(tmax < 0){
            return {false, 0.0f};
        }
        if(tmin < 0){
            return {true, 0.0f};
        }
        return{true, tmin};

    }



            void RayCastCollision::maxIminVecBlock(const glm::ivec3& blockPos, glm::ivec3& MinVec, glm::ivec3& MaxVec){
                MinVec = blockPos;
                MaxVec = blockPos + glm::ivec3(1.0f);
            }  
    RayAABBResultBlock RayCastCollision::rayAABBBlock(const glm::vec3& camPos, const glm::vec3& camFront, const glm::ivec3& pos){
            glm::ivec3 MinVec;
            glm::ivec3 MaxVec;
            maxIminVecBlock(pos, MinVec, MaxVec);
            float tmin = 0.0f;
            float tmax = std::numeric_limits<float>::infinity();

            glm::vec3 hitPos = glm::vec3(0.0f);
            glm::vec3 ktoraSciana = glm::vec3(0.0f);

            for(int i = 0; i < 3 ; i++){
                if(std::abs(camFront[i]) < 1e-6f){
                    if(!(MinVec[i] <= camPos[i] && MaxVec[i] >= camPos[i])){
                        return {false, glm::vec3(0.0), glm::vec3(0.0),  0.0f};
                    }
                }else{
                    float t1 = (MinVec[i] - camPos[i]) / camFront[i];
                    float t2 = (MaxVec[i] - camPos[i]) / camFront[i];

                    if(t1 > t2) {std::swap(t1, t2);}

                    tmin = std::max(tmin, t1);
                    tmax = std::min(tmax, t2);
                    if(tmin > tmax){
                        return {false, glm::vec3(0.0), glm::vec3(0.0),  0.0f};
                    }
                }
            }

                if(tmax < 0){
                    return {false, glm::vec3(0.0), glm::vec3(0.0),  0.0f};
                }

            hitPos = camPos + (tmin * camFront);
            for(int k = 0; k < 3; k++){
                if(abs(hitPos[k] - MinVec[k]) < 1e-6f){
                    ktoraSciana[k] = -1;
                }
                if(abs(hitPos[k] - MaxVec[k]) < 1e-6f){
                    ktoraSciana[k] = 1;
                }
            }
            int z = 0;
            for(int j = 0; j < 3 ; j++){
                if(ktoraSciana[j] != 0){
                    z++;
                }
            }
            if(z > 1){
                if(ktoraSciana[0] != 0){
                    ktoraSciana = glm::vec3(ktoraSciana[0], 0.0f, 0.0f);
                }else{
                    ktoraSciana = glm::vec3(0.0f, ktoraSciana[1], 0.0f);
                }
            }

            if(tmin < 0){
                return {true, hitPos, ktoraSciana, 0.0f};
            }
            return {true, hitPos, ktoraSciana, tmin};
        }   


