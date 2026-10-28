#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include "cam.h"
#include "block.h"


#include <iostream>
#include <functional>




class WorldManager{
    public:
        WorldManager(unsigned int BlocksNumber_Temp) : BlocksNumber(BlocksNumber_Temp){
            creatingBlocksParameters();
            creatingBlockPositionsParameters();
        }

        const std::vector<Block>& getBlocks()const {
            return blocks;
        }

        const std::unordered_set<glm::ivec3, IVec3Hash>& getBlocksPositions()const {
            return blockPositions;
        }

        const Sun& getSun()const {
            return sun;
        }

        const PointLight& getPointLight()const {
            return pointLight;
        }

        void destroyBlock(const int ID, const glm::ivec3& Pos) {
            for (auto it = blocks.begin(); it != blocks.end(); ++it) {
                if (it->pos == Pos) {
                    glm::ivec3 pos = glm::ivec3(it->pos);
                    blockPositions.erase(pos);
                    blocks.erase(it);
                    return;
                }
            }
        }

        void PutBlock(const glm::vec3& NewPos){
            blocks.emplace_back(NewPos, BlockIDNumber++, 0);
            blockPositions.insert(glm::ivec3(NewPos));
        }


    private:
        std::vector<Block> blocks;
        std::unordered_set<glm::ivec3, IVec3Hash> blockPositions;        
        unsigned int BlocksNumber, BlockIDNumber;
        Sun sun;
        PointLight pointLight;


        void creatingBlocksParameters(){
            BlockIDNumber = 0;
            for(unsigned int x = 0 ; x < BlocksNumber / 10 ; x++){  
                for(unsigned int z = 0; z < BlocksNumber / 10 ; z++){
                    blocks.emplace_back(glm::vec3(x, 0.0, z), BlockIDNumber, 0);
                    blockPositions.insert(glm::ivec3(glm::vec3(x, 0.0, z)));
                    BlockIDNumber++;
                }
            }
               
        }

        void creatingBlockPositionsParameters(){
            for(const auto& block : blocks){
                blockPositions.insert(glm::ivec3(block.pos));
            }
        }

};