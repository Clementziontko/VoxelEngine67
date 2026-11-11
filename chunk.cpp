#include "chunk.h"
#include <cmath>
#include<unordered_map>
#include <cstddef>


ChunkG::ChunkG(std::vector<ChunkG>& chunks, std::unordered_map<glm::ivec3, size_t, IVec3Hash>& chunkPositions){}



    glm::ivec3 ChunkG::blokPosNAChunkPos(const glm::ivec3& blokPos)
    {
        int chunkX = static_cast<int>(std::floor(blokPos.x / 16.0f));
        int chunkY = static_cast<int>(std::floor(blokPos.y / 16.0f));
        int chunkZ = static_cast<int>(std::floor(blokPos.z / 16.0f));
        return glm::ivec3(chunkX, chunkY, chunkZ);
    }

    bool ChunkG::CheckIfBlockPosIsOnChunkBorder(const glm::ivec3& blockPos, glm::ivec3& chunkPos)
{
    int chunkStartPosX = chunkPos.x * 16;
    int chunkStartPosY = chunkPos.y * 16;
    int chunkStartPosZ = chunkPos.z * 16;
    if(blockPos.x == chunkStartPosX || blockPos.x == (chunkStartPosX + 15) || 
       blockPos.y == chunkStartPosY || blockPos.y == (chunkStartPosY + 15) ||
       blockPos.z == chunkStartPosZ || blockPos.z == (chunkStartPosZ + 15))

       {return true;}

    return false;
}

    bool ChunkG::ifHasBlockAt(const glm::ivec3& Pos, const std::unordered_map<glm::ivec3, size_t, IVec3Hash>& blockPositions)
    {return blockPositions.find(glm::ivec3(Pos)) != blockPositions.end();}

    bool ChunkG::sprawdzanieCzyFaceBlokuGraniczyZInnymBlokiem(const glm::ivec3& Face, const glm::ivec3& chunkPos, const glm::ivec3& blokPos, const std::vector<ChunkG>& Chunks, const std::unordered_map<glm::ivec3, size_t, IVec3Hash>& chunkPositions){
        glm::ivec3 PotencjalnyChunkPosToCheck = chunkPos + Face;
        glm::ivec3 PotencjalnyBlokPosToCheck = blokPos + Face;

        auto it = chunkPositions.find(PotencjalnyChunkPosToCheck);
        if(it != chunkPositions.end()){
            auto it2 = Chunks[it->second].chunkBlocksPositions.find(PotencjalnyBlokPosToCheck);
            if(it2 != Chunks[it->second].chunkBlocksPositions.end()){
                return true;
            }

        }
        return false;

    }

void ChunkG::CreateMeshVertices(const std::vector<ChunkG>& Chunks, const std::unordered_map<glm::ivec3, size_t, IVec3Hash>& chunkPositions)
{
    meshVertices.clear();
    
            
    for(const auto& Blok : chunkBlocks){
        std::vector<glm::ivec3> BlockedFaces;
        bool blokJestNaGranicy = false;
        
          
            
        if(CheckIfBlockPosIsOnChunkBorder(Blok.pos, chunkPos)){
            blokJestNaGranicy = true;
        }
        for(const auto& Face : faces){
            if(blokJestNaGranicy){
                glm::ivec3 neighborPos = Blok.pos + Face;
                if(ifHasBlockAt(neighborPos, chunkBlocksPositions))
                {
                    BlockedFaces.push_back(Face);
                }else{
                    if(sprawdzanieCzyFaceBlokuGraniczyZInnymBlokiem(Face, chunkPos, Blok.pos, Chunks, chunkPositions)
                    && blokPosNAChunkPos(Blok.pos) != blokPosNAChunkPos(Blok.pos + Face)){

                        BlockedFaces.push_back(Face);
                    }else{
                        continue;
                    }
                }
                
            }else
                if(ifHasBlockAt(glm::ivec3(Blok.pos + Face), chunkBlocksPositions))
                {
                    BlockedFaces.push_back(Face);
                }else{
                    continue;
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
                VertexG v;
                v.pos = glm::ivec3(vertices[i], vertices[i + 1], vertices[i + 2]) + Blok.pos;
                v.nor = glm::ivec3(vertices[i + 3] , vertices[i + 4] , vertices[i + 5]);
                v.uv = glm::ivec2(vertices[i + 6], vertices[i + 7]);
                meshVertices.emplace_back(v);
            }
        }
    }
}
        

void ChunkG::SettingAttributesForChunk()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &VBOInstance);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(VertexG), meshVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(VertexG, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(VertexG, nor));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(VertexG, uv));

    // glBindBuffer(GL_ARRAY_BUFFER, VBOInstance);
    // glBufferData(GL_ARRAY_BUFFER, offsets.size() * sizeof(glm::vec3), offsets.data(), GL_STATIC_DRAW);
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glVertexAttribDivisor(3, 1
    glBindVertexArray(0);
}


void ChunkG::Draw(ShaderManager& shader, CAM& cam)
{
     shader.uniformSetInt("blinn", 1);
     shader.uniformSetInt("gamma", 0);
     shader.uniformSetInt("StencilTest", 0);
     shader.uniformSetVec3("viewPos", cam.getViewPos());
     glm::mat4 cube(1.0);
     cube = glm::translate(cube, glm::vec3(0.0f));
     shader.uniformSetMat("model", cube);
     glBindVertexArray(VAO);
     glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(meshVertices.size()));
     glBindVertexArray(0);

}
           


        std::vector<glm::ivec3> ChunkG::checkWhatChunkIsToChangeWhenBlockIsOnChunkBorder(const glm::ivec3& blokPos, const glm::ivec3& chunkPos){
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


void ChunkG::UpdateChunk(const glm::ivec3& chunkPos, const glm::ivec3& blockPos, BlockAction action, std::vector<ChunkG>& Chunks
    , std::unordered_map<glm::ivec3, size_t, IVec3Hash>& chunkPositions){
        
        glm::ivec3 chunkPosToChange = chunkPos;
        dirty = true;
        UpdateChunkIfHeIsDirty(Chunks, chunkPositions); 
    
    
    if(CheckIfBlockPosIsOnChunkBorder(blockPos, chunkPosToChange)){
        std::vector<glm::ivec3> ChunkPosesToChange = checkWhatChunkIsToChangeWhenBlockIsOnChunkBorder(blockPos, chunkPosToChange);
        for(const auto& Chunkpos : ChunkPosesToChange) {
            auto it = chunkPositions.find(Chunkpos);
            if(it != chunkPositions.end()) {
                Chunks[it->second].dirty = true; 
                Chunks[it->second].UpdateChunkIfHeIsDirty(Chunks, chunkPositions);
            }
        }
    }

}

void ChunkG::UpdateChunkIfHeIsDirty(const std::vector<ChunkG>& Chunks, const std::unordered_map<glm::ivec3, size_t, IVec3Hash>& chunkPositions){
    CreateMeshVertices(Chunks, chunkPositions);
       if(NowoNarodzonyWTrakcieProgramu){
           glGenVertexArrays(1, &VAO);
           glGenBuffers(1, &VBO);
           glBindVertexArray(VAO);
           glBindBuffer(GL_ARRAY_BUFFER, VBO);
           glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(VertexG), meshVertices.data(), GL_DYNAMIC_DRAW);
           glEnableVertexAttribArray(0);
           glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(VertexG, pos));
           glEnableVertexAttribArray(1);
           glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(VertexG, nor));
           glEnableVertexAttribArray(2);
           glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(VertexG, uv));
           glBindVertexArray(0);
           NowoNarodzonyWTrakcieProgramu = false;
           dirty = false;
       }else{
           glBindBuffer(GL_ARRAY_BUFFER, VBO);
           glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(VertexG), meshVertices.data(), GL_DYNAMIC_DRAW);
           glBindBuffer(GL_ARRAY_BUFFER, 0);
           dirty = false;
       }
}


