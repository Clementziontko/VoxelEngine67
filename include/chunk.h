#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstddef>

#include "block.h"
#include "camera.h"

// Forward declarations
class ShaderManager;

class ChunkG {
public:
    // Represents a single vertex in the chunk mesh with position, normal, and UV coordinates
    struct VertexG {
        glm::vec3 pos;
        glm::vec3 nor;
        glm::vec2 uv;
    };

    ChunkG() = default;
    ChunkG(std::vector<ChunkG>& chunks, std::unordered_map<glm::ivec3, size_t, IVec3Hash>& chunkPositions);
    ~ChunkG(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    // Getters
    std::vector<Block>& GetChunkBlocks() { return chunkBlocks; }
    std::vector<VertexG>& GetChunkMeshVertices() { return meshVertices; }
    std::unordered_map<glm::ivec3, size_t, IVec3Hash>& GetChunkBlockPositions() { return chunkBlocksPositions; }
    glm::ivec3& getChunkpos() { return chunkPos; }

    // Core chunk methods
    void UpdateChunk(const glm::ivec3& chunkPos, const glm::ivec3& blockPos, BlockAction action, 
                     std::vector<ChunkG>& Chunks, std::unordered_map<glm::ivec3, size_t, IVec3Hash>& chunkPositions);
    void Draw(ShaderManager& shader, CAM& cam);
    void CreateMeshVertices(const std::vector<ChunkG>& Chunks, const std::unordered_map<glm::ivec3, size_t, IVec3Hash>& chunkPositions);
    void SettingAttributesForChunk();

    // Public flags and OpenGL identifiers
    bool dirty = false;
    bool NowoNarodzonyWTrakcieProgramu = false;
    unsigned int VBO = 0;
    unsigned int VAO = 0;

private:
    std::vector<Block> chunkBlocks;
    std::unordered_map<glm::ivec3, size_t, IVec3Hash> chunkBlocksPositions;
    std::vector<VertexG> meshVertices;
    glm::ivec3 chunkPos{0, 0, 0};
    
    // Base template vertices for block faces
    std::vector<float> vertices = {
        // position         normal         UV
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
    
    // Face normals/directions
    std::vector<glm::ivec3> faces = {
        glm::ivec3(1.0f, 0.0f, 0.0f), glm::ivec3(0.0f, 1.0f, 0.0f), glm::ivec3(0.0f, 0.0f, 1.0f), 
        glm::ivec3(-1.0f, 0.0f, 0.0f), glm::ivec3(0.0f, -1.0f, 0.0f), glm::ivec3(0.0f, 0.0f, -1.0f)
    };

    // Private helper methods
    glm::ivec3 blokPosNAChunkPos(const glm::ivec3& blockPos);
    bool sprawdzanieCzyFaceBlokuGraniczyZInnymBlokiem(const glm::ivec3& Face, const glm::ivec3& chunkPos, const glm::ivec3& blokPos, const std::vector<ChunkG>& Chunks, const std::unordered_map<glm::ivec3, size_t, IVec3Hash>& chunkPositions);
    bool CheckIfBlockPosIsOnChunkBorder(const glm::ivec3& blockPos, glm::ivec3& chunkPos);
    bool ifHasBlockAt(const glm::ivec3& Pos, const std::unordered_map<glm::ivec3, size_t, IVec3Hash>& blockPositions);
    std::vector<glm::ivec3> checkWhatChunkIsToChangeWhenBlockIsOnChunkBorder(const glm::ivec3& blokPos, const glm::ivec3& chunkPos);
    void UpdateChunkIfHeIsDirty(const std::vector<ChunkG>& Chunks, const std::unordered_map<glm::ivec3, size_t, IVec3Hash>& chunkPositions);
};


