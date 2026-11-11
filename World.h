#pragma once 

#include <glm/glm.hpp>
#include <unordered_map>
#include "block.h"
#include "chunk.h"
#include"frustum.h"
#include "rayCastCollisionAABB.h"

constexpr unsigned int chunkReserved = 1000;
constexpr float max_load_factor = 0.75f;

class ShaderManager;
class CAM;

class World{

    public:
        World(unsigned int nrB_temp);

        void GenerateWorld();


        std::vector<ChunkG>& GetChunksVec(){ return Chunks; }
        std::unordered_map<glm::ivec3, size_t, IVec3Hash>& GetChunksVecPos(){ return chunkPositions; }
        RaycastHit& getRayCastResults(){ return rayCastHit; }
        const Sun& getSun()const { return sun; }
        const PointLight& getPointLight()const { return pointLight; }


        void setUniformsForShaders(ShaderManager& shader);


             void destroyBlock(const glm::ivec3& chunkPos, const glm::ivec3& blockPos, BlockAction action);
             void putaBlock(const glm::ivec3& potentialChunkPos, const glm::ivec3& blockPosToPutOnBlock, BlockAction action);


                     void RayCast(RayCastCollision& rayCast, CAM& cam, ChunkG& Chunk, float& tChunks, float& tBlocks);
                     void DrawChunk(ShaderManager& shader, CAM& cam, ChunkG& Chunk);



    private:
        std::vector<ChunkG> Chunks;
        std::unordered_map<glm::ivec3, size_t, IVec3Hash> chunkPositions;
        unsigned int nrB;
        Sun sun;
        PointLight pointLight;
        RaycastHit rayCastHit;

                     glm::ivec3 blokPosNAChunkPos(const glm::ivec3& blockPos);
                     bool czyTakiChunkJuzIstnieje(const glm::ivec3& chunkPos, const Block& block);
                     void przypisanieBlokuDoPustegoChunka(const glm::ivec3& chunkPos, const Block& block);
        void addBlock(const glm::ivec3& blockPos, const unsigned int blockID, BlockType type);

        void KurwiDołek(const std::vector<Block>& vec);



        void swapAndpop(const size_t indexToDelate, const size_t chunkIndex, const glm::ivec3& blockPos);
        void addBlockWTrakcieProgramu(const size_t chunkIndex, const glm::ivec3& blockPos, unsigned int ID, BlockType type);
        

    
};