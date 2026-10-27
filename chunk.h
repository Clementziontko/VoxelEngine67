#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include "cam.h"
#include "block.h"

#include <iostream>
#include <cstddef>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <tuple>
#include <utility>


class Chunk{
    public:
        Chunk(const std::vector<Block>& blocksAll, const std::unordered_set<glm::ivec3, IVec3Hash>& blockPositions){
            CreatingChunksInfo(blocksAll);
            CreatingMeshVerticesForEachChunk(blockPositions);
            SettingAttributesForEachChunk();
        }

        void DrawAll(ShaderManager& shader, CAM& cam, const Sun& sun, const PointLight& pointLight){
                shader.uniformSetVec3("sun.direction", sun.direction);
                shader.uniformSetVec3("sun.ambient", sun.ambient);
                shader.uniformSetVec3("sun.diffuse", sun.diffuse);
                shader.uniformSetVec3("sun.specular",sun.specular);

            //POINT LIGHT
                shader.uniformSetVec3("pl.position", pointLight.position);
                shader.uniformSetFloat("pl.constant", 1.0f);
                shader.uniformSetFloat("pl.linear", 0.027f);
                shader.uniformSetFloat("pl.quadratic", 0.0028f);
                shader.uniformSetVec3("pl.ambient", pointLight.ambient);
                shader.uniformSetVec3("pl.diffuse", pointLight.diffuse);
                shader.uniformSetVec3("pl.specular", pointLight.specular);

            //RESZTA
                shader.uniformSetInt("blinn", 1);
                shader.uniformSetInt("gamma", 0);
                shader.uniformSetInt("StencilTest", 0);
                shader.uniformSetVec3("viewPos", cam.getViewPos());
                glm::mat4 cube(1.0);
                cube = glm::translate(cube, glm::vec3(0.0f));
                shader.uniformSetMat("model", cube);
                // for(auto& [chunkPos, Chunk] : chunkPositions){
            for(const auto& Chunk : Chunks){
                glBindVertexArray(Chunk.VAO);
                glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(Chunk.meshVertices.size()));
                glBindVertexArray(0);
            }
            
        }

        void UpdateChunk(const glm::ivec3& blockPos, BlockAction action, const std::vector<Block>& blocksAll, const std::unordered_set<glm::ivec3, IVec3Hash>& blockPositions){
            bool czyZnaleziono = false;
            glm::ivec3 chunkPosToChange = blokPosNAChunkPos(blockPos);
            // std::cout<<chunkPosToChange.x<<" "<<chunkPosToChange.y<<" "<<chunkPosToChange.z<<std::endl;
            bool blokJestNaGranicy = false;
            // auto it = chunkPositions.find(chunkPosToChange);
            // if(it != chunkPositions.end()){
            //     auto& Chunk = it->second;
            //     std::cout<<"NIgger"<<std::endl;
            for(auto& Chunk : Chunks){
                if(Chunk.chunkPos == chunkPosToChange){
                    Chunk.dirty = true;
                    if(action == BlockAction::Broken ){
                        UpdateChunkAfterDestroingBlock(Chunk, blockPos);
                        czyZnaleziono = true;
                        // break;
                    }else if(action == BlockAction::Placed){
                        UpdateChunkAfterPuttingNewBlock(Chunk, blockPos, blocksAll);
                        czyZnaleziono = true;
                        // break;
                    }
                }
            }
             if(CheckIfBlockPosIsOnChunkBorder(blockPos, chunkPosToChange)){
                std::vector<glm::ivec3> ChunkPosesToChange = checkWhatChunkIsToChangeWhenBlockIsOnChunkBorder(blockPos, chunkPosToChange);
                          
               
                //  for(auto& [chunkPos, Chunk] : chunkPositions){    
                for(auto& Chunk : Chunks){

                    for(auto& Chunkpos : ChunkPosesToChange){
                        //  auto it = chunkPositions.find(Chunkpos);
                        //  if(it != chunkPositions.end()){
                            // auto Chunk = it->second;
                            Chunk.dirty = true;
                        }
                    }
                    
                }
                blokJestNaGranicy = true;
            // }




            if(!czyZnaleziono){
                std::cout<<"1: Updated GG"<<std::endl;
                chunkInfo c;
                c.NowoNarodzonyWTrakcieProgramu = true;
                c.dirty = true;
                c.chunkID = 67;
                c.chunkPos = chunkPosToChange;
                Chunks.push_back(c);
                auto& newChunk = Chunks.back();
             
                if(action == BlockAction::Placed){
                    std::cout<<"2: Updated GG"<<std::endl;
                    UpdateChunkAfterPuttingNewBlock(newChunk, blockPos, blocksAll);
                }
                
            }
            //  for(auto& [chunkPos, Chunk] : chunkPositions){
            for(auto& Chunk : Chunks){
                if(Chunk.dirty){
                    CreateMeshVertices(Chunk.chunkBlocks, blockPositions, Chunk);
                    if(Chunk.NowoNarodzonyWTrakcieProgramu){
                        glGenVertexArrays(1, &Chunk.VAO);
                        glGenBuffers(1, &Chunk.VBO);

                        glBindVertexArray(Chunk.VAO);
                        glBindBuffer(GL_ARRAY_BUFFER, Chunk.VBO);

                        glBufferData(GL_ARRAY_BUFFER, Chunk.meshVertices.size() * sizeof(Vertex), Chunk.meshVertices.data(), GL_DYNAMIC_DRAW);

                        glEnableVertexAttribArray(0);
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, pos));
                        glEnableVertexAttribArray(1);
                        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, nor));
                        glEnableVertexAttribArray(2);
                        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, uv));

                        glBindVertexArray(0);
                        Chunk.NowoNarodzonyWTrakcieProgramu = false;
                        Chunk.dirty = false;
                    }else{
                        glBindBuffer(GL_ARRAY_BUFFER, Chunk.VBO);
                        glBufferData(GL_ARRAY_BUFFER, Chunk.meshVertices.size() * sizeof(Vertex), Chunk.meshVertices.data(), GL_DYNAMIC_DRAW);
                        glBindBuffer(GL_ARRAY_BUFFER, 0);
                        Chunk.dirty = false;
                    }
                }
            }
        }

        unsigned int getChunkVBOForBlock(const glm::ivec3& blockPos){
            for(auto& Chunk : Chunks){
                glm::ivec3 chunkPoToCheck = blokPosNAChunkPos(blockPos);
                if(Chunk.chunkPos == chunkPoToCheck){
                    return Chunk.VBO;
                }
            }
            return 1;
        }

        ~Chunk(){
            //  for(auto& [chunkPos, Chunk] : chunkPositions){
            for(auto& Chunk : Chunks){
                glDeleteVertexArrays(1, &Chunk.VAO);
                glDeleteBuffers(1, &Chunk.VBO);
            }
        }

    private:
        std::vector<float> vertices = {
            // position           normal           UV
            // BACK (Z = 0)
            0.0f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
            1.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
            1.0f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,

            1.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
            0.0f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,

            // FRONT (Z = 1)
            0.0f, 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
            1.0f, 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
            1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f,

            1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

            // LEFT (X = 0)
            0.0f, 0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
            0.0f, 0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,

            0.0f, 1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
            0.0f, 1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
            0.0f, 0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,

            // RIGHT (X = 1)
            1.0f, 0.0f, 1.0f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,

            1.0f, 1.0f, 0.0f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,

            // BOTTOM (Y = 0)
            0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
            1.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,

            1.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
            0.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
            0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,

            // TOP (Y = 1)
            0.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
            1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,

            1.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
            0.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
            0.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f
        };
        
        std::vector<glm::ivec3> faces = {
            glm::ivec3(1.0f, 0.0f, 0.0f), glm::ivec3(0.0f, 1.0f, 0.0f), glm::ivec3(0.0f, 0.0f, 1.0f), 
            glm::ivec3(-1.0f, 0.0f, 0.0f), glm::ivec3(0.0f, -1.0f, 0.0f),glm::ivec3(0.0f, 0.0f, -1.0f)
        };

        struct Vertex {
            glm::vec3 pos;
            glm::vec3 nor;
            glm::vec2 uv;
        };
        
        struct chunkInfo{
            std::vector<Block> chunkBlocks;
            std::unordered_set<glm::ivec3, IVec3Hash> chunkBlockPositions;
            unsigned int VBO, VAO;
            unsigned int chunkID = 1;
            std::vector<Vertex> meshVertices;
            glm::ivec3 chunkPos;
            bool dirty = false;
            bool NowoNarodzonyWTrakcieProgramu = false;
        };

        std::unordered_map<glm::ivec3, chunkInfo, IVec3Hash> chunkPositions;
        std::vector<chunkInfo> Chunks;
        
        void CreateMeshVertices(const std::vector<Block>& blocks, const std::unordered_set<glm::ivec3, IVec3Hash>& AllblockPositions, chunkInfo& singleChunk){
            singleChunk.meshVertices.clear();
            
            for(const auto& Blok : blocks){
                std::vector<glm::ivec3> BlockedFaces;
                bool blokJestNaGranicy = false;
                
                  
                    
                if(CheckIfBlockPosIsOnChunkBorder(Blok.pos, singleChunk.chunkPos)){
                    blokJestNaGranicy = true;
                }
                for(const auto& Face : faces){
                    if(blokJestNaGranicy){
                        glm::ivec3 neighborPos = Blok.pos + Face;
                        if(ifHasBlockAt(neighborPos, blocks, AllblockPositions))
                        {
                            BlockedFaces.push_back(Face);
                        }else{
                            continue;
                        }

                    }else{

                        glm::ivec3 neighborPos = Blok.pos + Face;
                        if(ifHasBlockAt(neighborPos, blocks, singleChunk.chunkBlockPositions))
                        {
                            BlockedFaces.push_back(Face);
                        }else{
                            continue;
                        }

                    }

                }

                for(int i = 0 ; i < vertices.size() ; i += 8){
                    bool czyZlapanoZlaSciana = false;
                    for(const auto& Face : BlockedFaces){
                        if(Face == glm::ivec3(0.0f, 0.0f, -1.0f) && i == 0 || Face == glm::ivec3(0.0f, 0.0f, 1.0f) && i == 48 ||
                            Face == glm::ivec3(-1.0f, 0.0f, 0.0f) && i == 96 || Face == glm::ivec3(1.0f, 0.0f, 0.0f) && i == 144 ||
                            Face == glm::ivec3(0.0f, -1.0f, 0.0f) && i == 192 || Face == glm::ivec3(0.0f, 1.0f, 0.0f) && i == 240){
                            i += 40;
                            czyZlapanoZlaSciana = true;
                            break;
                        }
                    }
                    if(czyZlapanoZlaSciana){
                        continue;
                    }else{
                        Vertex v;
                        v.pos = glm::ivec3(vertices[i], vertices[i + 1], vertices[i + 2]) + Blok.pos;
                        v.nor = glm::ivec3(vertices[i + 3] , vertices[i + 4] , vertices[i + 5]);
                        v.uv = glm::ivec2(vertices[i + 6], vertices[i + 7]);

                        singleChunk.meshVertices.push_back(v);
                    }
                }
            }
        }

        bool ifHasBlockAt(glm::ivec3& Pos, const std::vector<Block>& blocks, const std::unordered_set<glm::ivec3, IVec3Hash>& blockPositions){
            return blockPositions.find(glm::ivec3(Pos)) != blockPositions.end();
        }

        glm::ivec3 blokPosNAChunkPos(const glm::ivec3& blokPos){
            int chunkX = static_cast<int>(std::floor(blokPos.x / 16.0f));
            int chunkY = static_cast<int>(std::floor(blokPos.y / 16.0f));
            int chunkZ = static_cast<int>(std::floor(blokPos.z / 16.0f));
            return glm::ivec3(chunkX, chunkY, chunkZ);
        }

        bool czyTakiChunkJuzIstnieje(const glm::ivec3& chunkPos, const Block& block){
            auto it = chunkPositions.find(chunkPos);
            if(it != chunkPositions.end()){
                auto Chunk = it->second;
            // for(auto& Chunk : Chunks){
                // if(Chunk.chunkPos == chunkPos){
                    Chunk.chunkBlocks.emplace_back(block);
                    Chunk.chunkBlockPositions.insert(glm::ivec3(block.pos));

                    return true;
                    
                // }
            }

            return false;
        }

        void przypisanieBlokuDoPustegoChunka(const glm::ivec3& chunkPos, const Block& block){
            chunkInfo Chunk;
            // chunkInfo& Chunk = chunkPositions[chunkPos];
            Chunk.chunkBlocks.push_back(block);
            Chunk.chunkBlockPositions.insert(glm::ivec3(block.pos));
            Chunk.chunkPos = chunkPos;
            // chunkPositions.emplace(chunkPos, std::move(Chunk));
            Chunks.emplace_back(std::move(Chunk));
            // Chunks.emplace_back(chunkPositions.at(chunkPos));

        }

        void SettingAttributesForEachChunk(){
            //  for(auto& [chunkPos, Chunk] : chunkPositions){   
            for(auto& Chunk : Chunks){
                glGenVertexArrays(1, &Chunk.VAO);
                glGenBuffers(1, &Chunk.VBO);
                // glGenBuffers(1, &VBOInstance);

                glBindVertexArray(Chunk.VAO);
                glBindBuffer(GL_ARRAY_BUFFER, Chunk.VBO);

                glBufferData(GL_ARRAY_BUFFER, Chunk.meshVertices.size() * sizeof(Vertex), Chunk.meshVertices.data(), GL_STATIC_DRAW);

                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, pos));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, nor));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, uv));

                // glBindBuffer(GL_ARRAY_BUFFER, VBOInstance);
                // glBufferData(GL_ARRAY_BUFFER, offsets.size() * sizeof(glm::vec3), offsets.data(), GL_STATIC_DRAW);
                // glEnableVertexAttribArray(3);
                // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                // glVertexAttribDivisor(3, 1);

                glBindVertexArray(0);
            }
        }
        
        void CreatingChunksInfo(const std::vector<Block>& blocksAll){
            for(const auto& Block : blocksAll){
                glm::ivec3 chunkPos = blokPosNAChunkPos(Block.pos);
                if(!czyTakiChunkJuzIstnieje(chunkPos, Block)){
                    przypisanieBlokuDoPustegoChunka(chunkPos, Block);
                }
            }
        }

        void CreatingMeshVerticesForEachChunk(const std::unordered_set<glm::ivec3, IVec3Hash>& blockPositions){
            //  for(auto& [chunkPos, Chunk] : chunkPositions){   
            for(auto& Chunk : Chunks){
                CreateMeshVertices(Chunk.chunkBlocks, blockPositions, Chunk);
            }
        }

        void UpdateChunkAfterDestroingBlock(chunkInfo& Chunk, const glm::ivec3& blockPos){
            for (auto it = Chunk.chunkBlocks.begin(); it != Chunk.chunkBlocks.end(); ++it) {
                if (it->pos == blockPos){
                    glm::ivec3 pos = glm::ivec3(it->pos);
                    Chunk.chunkBlockPositions.erase(pos);
                    Chunk.chunkBlocks.erase(it);
                    return;
                }
            }
        }

        void UpdateChunkAfterPuttingNewBlock(chunkInfo& Chunk, const glm::ivec3& blockPos, const std::vector<Block>& blocksAll){
            
            unsigned int BlocksVecSize = blocksAll.size();
            unsigned int lastBlockID = blocksAll[BlocksVecSize - 1].id;
            Chunk.chunkBlocks.emplace_back(blockPos, lastBlockID, 0);
            Chunk.chunkBlockPositions.insert(glm::ivec3(blockPos));
        }

        bool CheckIfBlockPosIsOnChunkBorder(const glm::ivec3& blockPos, glm::ivec3& chunkPos){
            int chunkStartPosX = chunkPos.x * 16;
            int chunkStartPosY = chunkPos.y * 16;
            int chunkStartPosZ = chunkPos.z * 16;
            if(blockPos.x == chunkStartPosX || blockPos.x == (chunkStartPosX + 15) || 
               blockPos.y == chunkStartPosY || blockPos.y == (chunkStartPosY + 15) ||
               blockPos.z == chunkStartPosZ || blockPos.z == (chunkStartPosZ + 15))

               {return true;}

            return false;
        }

        std::vector<glm::ivec3> checkWhatChunkIsToChangeWhenBlockIsOnChunkBorder(const glm::ivec3& blokPos, const glm::ivec3& chunkPos){
            std::vector<glm::ivec3> ChunkPosesToChange;
            for(const auto& face : faces){
                glm::ivec3 neighborPos = blokPos + face;
                glm::ivec3 chunkPosToCheck = blokPosNAChunkPos(neighborPos);
                if(chunkPos != chunkPosToCheck){
                    ChunkPosesToChange.emplace_back(chunkPosToCheck);
                }
            }
            return ChunkPosesToChange;
        }


};
