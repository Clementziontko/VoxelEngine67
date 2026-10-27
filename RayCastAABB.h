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

class RayCastCollision{
    public:

        RayAABBResultBlock rayAABB(const glm::vec3& camPos, const glm::vec3& camFront, const glm::vec3& pos){
            maxIminVec(pos);
            std::vector<glm::vec2> wyniki;
            float newMax;
            float newMin;
            float inf = std::numeric_limits<float>::infinity();
            glm::vec2 czWspolna;
            glm::vec3 hitPos = glm::vec3(0.0f);
            glm::vec3 ktoraSciana = glm::vec3(0.0f);
            bool ZP1 = true;
            for(int i = 0; i < 3 ; i++){
                newMax = MaxVec[i] - camPos[i];
                newMin = MinVec[i] - camPos[i];
                if(camFront[i] > 0){
                    newMax = newMax / camFront[i];
                    newMin = newMin / camFront[i];
                    wyniki.emplace_back(newMin, newMax);
                }
                else if(camFront[i] < 0){
                    newMax = newMax / camFront[i];
                    newMin = newMin / camFront[i];
                    wyniki.emplace_back(newMax, newMin);
                }
                else{
                    if(MinVec[i] <= camPos[i] && MaxVec[i] >= camPos[i]){
                        wyniki.emplace_back(-inf, inf);
                    }
                    else{
                        return {false, glm::vec3(0.0), glm::vec3(0.0),  0.0f};
                    }
                }
            }
            czWspolna.x = std::max(wyniki[0].x, wyniki[1].x);
            czWspolna.y = std::min(wyniki[0].y, wyniki[1].y);

            if(czWspolna.x >  czWspolna.y){
                return {false, hitPos, glm::vec3(0.0),  0.0f};
            }
            if(czWspolna.y < 0){
                return {false, hitPos, glm::vec3(0.0),  0.0f};
            }
            czWspolna.x = std::max(czWspolna.x, wyniki[2].x);
            czWspolna.y = std::min(czWspolna.y, wyniki[2].y);
            if(czWspolna.x >  czWspolna.y){
                return {false, hitPos, glm::vec3(0.0),  0.0f};
            }
            if(czWspolna.y < 0){
                return {false, hitPos, glm::vec3(0.0),  0.0f};
            }
            hitPos = camPos + (czWspolna.x * camFront);
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

            if(czWspolna.x < 0){
                return {true, hitPos, ktoraSciana, std::max(czWspolna.x, 0.0f)};
            }
            return {true, hitPos, ktoraSciana, czWspolna.x};
        }   


    private:
        glm::vec3 MinVec;
        glm::vec3 MaxVec;
        void maxIminVec(const glm::vec3& pos){
            MinVec = pos;
            MaxVec = pos + glm::vec3(1.0f);
        }  

};