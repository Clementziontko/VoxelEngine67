#include "World.h"
#include "ShaderManager.h"
#include "chunk.h"
#include <cmath>
#include<unordered_map>



World::World(unsigned int nrB_temp) : nrB(nrB_temp)
{
    Chunks.reserve(chunkReserved);
    chunkPositions.max_load_factor(max_load_factor);
    chunkPositions.reserve(chunkReserved);

    GenerateWorld();
}


glm::ivec3 World::blokPosNAChunkPos(const glm::ivec3& blokPos)
{
    int chunkX = static_cast<int>(std::floor(blokPos.x / 16.0f));
    int chunkY = static_cast<int>(std::floor(blokPos.y / 16.0f));
    int chunkZ = static_cast<int>(std::floor(blokPos.z / 16.0f));
    return glm::ivec3(chunkX, chunkY, chunkZ);
}

bool World::czyTakiChunkJuzIstnieje(const glm::ivec3& chunkPos, const Block& block)
{ 
    auto it = chunkPositions.find(chunkPos);
    if(it != chunkPositions.end()){
        Chunks[it->second].GetChunkBlocks().emplace_back(std::move(block));                     //DODANIE DANEGO BLOKU DO WEKTORA BLOKOW W DANYM CHUNKU
        Chunks[it->second].GetChunkBlockPositions()[glm::ivec3(block.pos)] = Chunks[it->second].GetChunkBlocks().size() - 1;  //DODANIE POZYCJI DANEGO BLOKU DO UORDERED SETA POZYCJI BLOKOW W DANYM CHUNKU
        return true;

    }
    return false;
}

void World::przypisanieBlokuDoPustegoChunka(const glm::ivec3& chunkPos, const Block& block)
{
    ChunkG Chunk;
    Chunk.GetChunkBlocks().emplace_back(std::move(block));       //DODANIE BLOKU DO WEKTORA BLOKOW W DANYM CHUNKU
    Chunk.GetChunkBlockPositions()[glm::ivec3(block.pos)] = Chunk.GetChunkBlocks().size() - 1;    //DODANIE POZYCJI DANEGO BLOKU DO UORDERED SETA POZYCJI BLOKOW W DANYM CHUNKU
    Chunk.getChunkpos() = chunkPos;
    Chunks.emplace_back(std::move(Chunk));                  //DODANIE CHUNKA DO WEKTORA CHUNKOW
    chunkPositions[chunkPos] = Chunks.size() - 1; 
}

void World::addBlock(const glm::ivec3& blockPos, const unsigned int blockID, BlockType type)
{   
    Block block;
    block.pos = blockPos;
    block.id = blockID;
    block.type = type;
    glm::ivec3 chunkPos = blokPosNAChunkPos(blockPos);

    if(!czyTakiChunkJuzIstnieje(chunkPos, block)){
        przypisanieBlokuDoPustegoChunka(chunkPos, block);
    }
    
}



void World::GenerateWorld()
{
    for(unsigned int x = 0 ; x < nrB ; x++){
        for(unsigned int z = 0 ; z < nrB ; z++){
            addBlock(glm::ivec3(x, 0, z), 0, BlockType::Grass);
        }
    }
}

void World::setUniformsForShaders(ShaderManager& shader)
{
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
}

    
                void World::swapAndpop(const size_t indexToDelate, const size_t chunkIndex, const glm::ivec3& blockPos){
        auto& blocks = Chunks[chunkIndex].GetChunkBlocks();
        auto& blockPositions = Chunks[chunkIndex].GetChunkBlockPositions();

        blockPositions.erase(blockPos);

        if(indexToDelate != blocks.size() - 1){

            blocks[indexToDelate] = std::move(blocks.back());
            blockPositions[blocks[indexToDelate].pos] = indexToDelate;

        }

        blocks.pop_back();
        
    }
                void World::KurwiDołek(const std::vector<Block>& vec) {
    std::cout << "[\n";
        for (const auto& nigger : vec) {
            std::cout << "  (" << nigger.pos.x << ", " << nigger.pos.y << ", " << nigger.pos.z << ")\n";
        }
        std::cout << "]\n";

}           
void World::destroyBlock(const glm::ivec3& chunkPos, const glm::ivec3& blockPos, BlockAction action) 
{

    auto itChunk = chunkPositions.find(chunkPos);
    
    if(itChunk != chunkPositions.end()){
        auto itBlock = Chunks[itChunk->second].GetChunkBlockPositions().find(blockPos);
        if(itBlock != Chunks[itChunk->second].GetChunkBlockPositions().end()){
            swapAndpop(itBlock->second, itChunk->second, blockPos);
            Chunks[itChunk->second].UpdateChunk(chunkPos, blockPos, action, Chunks, chunkPositions);

            return;
        }

        std::cout<<"gger 2"<<std::endl;
        return;
    }

}



                 void World::addBlockWTrakcieProgramu(const size_t chunkIndex, const glm::ivec3& blockPos, unsigned int ID, BlockType type){
                        Block block;
                        block.pos = blockPos;
                        block.id = ID;
                        block.type = type;
                        
                        Chunks[chunkIndex].GetChunkBlocks().emplace_back(std::move(block));                     //DODANIE DANEGO BLOKU DO WEKTORA BLOKOW W DANYM CHUNKU
                        Chunks[chunkIndex].GetChunkBlockPositions()[block.pos] = Chunks[chunkIndex].GetChunkBlocks().size() - 1;

                 }
void World::putaBlock(const glm::ivec3& potentialChunkPos, const glm::ivec3& blockPosToPutOnBlock, BlockAction action)
{
        unsigned int idPosssible;
        glm::ivec3 RealChunkPos = blokPosNAChunkPos(blockPosToPutOnBlock);
        auto itChunk = chunkPositions.find(RealChunkPos);
        
        if(itChunk != chunkPositions.end()){
            idPosssible = Chunks[itChunk->second].GetChunkBlocks().size();
            addBlockWTrakcieProgramu(itChunk-> second, blockPosToPutOnBlock, idPosssible, BlockType::Grass);
            Chunks[itChunk->second].UpdateChunk(potentialChunkPos, blockPosToPutOnBlock, action, Chunks, chunkPositions);
        }else{
            idPosssible = 0;
            Chunks.emplace_back();
            auto& c = Chunks.back();
            c.NowoNarodzonyWTrakcieProgramu = true;
            c.dirty = true;
            c.getChunkpos() = RealChunkPos;
            chunkPositions[RealChunkPos] = Chunks.size() - 1;
            addBlockWTrakcieProgramu(Chunks.size() - 1, blockPosToPutOnBlock, idPosssible, BlockType::Grass);
            c.UpdateChunk(potentialChunkPos, blockPosToPutOnBlock, action, Chunks, chunkPositions);
        }


        




}


void World::RayCast(RayCastCollision& rayCast, CAM& cam, ChunkG& Chunk, float& tChunks, float& tBlocks){
    RayAABBResultChunk chunkResults = rayCast.rayAABBChunk(cam.getViewPos(), cam.getCamFront(), Chunk.getChunkpos());
        if(chunkResults.hit){
            if(chunkResults.distance < tChunks){
                for(auto& Block : Chunk.GetChunkBlocks()){
                    RayAABBResultBlock blockResults = rayCast.rayAABBBlock(cam.getViewPos(), cam.getCamFront(), Block.pos);
                    if(blockResults.hit){
                        if(blockResults.distance < tBlocks){
                            rayCastHit.hit = true;
                            rayCastHit.chunk = &Chunk;
                            rayCastHit.block = &Block;
                            rayCastHit.blockPosition = Block.pos;
                            rayCastHit.distance = blockResults.distance;
                            rayCastHit.hitPoint = blockResults.hitPos;
                            rayCastHit.hitFace = blockResults.ktoraŚciana;
                            tBlocks = blockResults.distance;
                        }
                    }
                }
            }
        }
}

void World::DrawChunk(ShaderManager& shader, CAM& cam, ChunkG& Chunk){
    Chunk.Draw(shader, cam);
}


