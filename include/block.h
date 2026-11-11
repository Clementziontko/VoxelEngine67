#pragma once

#include <glm/glm.hpp>
#include <functional>

// Forward declarations
class ChunkG;

// Defines types of blocks available in the voxel world
enum class BlockType {
    Air,
    Grass,
    Dirt,
    Stone
};

// Defines actions that can be performed on blocks
enum class BlockAction {
    Broken, // Block has been destroyed
    Placed  // New block has been placed
};

// Represents a single block in the world grid
struct Block {
    glm::ivec3 pos{0, 0, 0};
    unsigned int id{0};
    BlockType type{BlockType::Air};
    unsigned int texId{0};

    Block() = default;
    Block(glm::ivec3 position, unsigned int blockID, BlockType blockType)
        : pos(position), id(blockID), type(blockType) {}
};

// Stores raycasting intersection data for block selection and interaction
struct RaycastHit {
    ChunkG* chunk = nullptr;
    Block* block = nullptr;
    glm::ivec3 blockPosition{0, 0, 0};
    glm::ivec3 hitFace{0, 0, 0}; // Normal or face identifier of the hit block
    glm::vec3 hitPoint{0.0f, 0.0f, 0.0f};
    float distance{0.0f};
    bool hit = false;
};

// Represents a point light source in the scene
struct PointLight {
    glm::vec3 position = glm::vec3(0.0f, 3.0f, 0.0f);
    glm::vec3 ambient  = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 diffuse  = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

// Represents directional sunlight for global scene illumination
struct Sun {
    glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f); // Direction vector from the light source
    glm::vec3 ambient   = glm::vec3(0.15f, 0.18f, 0.22f); // Sky blue ambient light
    glm::vec3 diffuse   = glm::vec3(1.0f, 0.98f, 0.9f);    // Warm sunlight diffuse color
    glm::vec3 specular  = glm::vec3(1.0f, 1.0f, 1.0f);    // Pure white specular highlights
};

// Custom hash function for glm::ivec3 to enable usage in unordered containers (e.g., std::unordered_map)
struct IVec3Hash {
    std::size_t operator()(const glm::ivec3& p) const {
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        std::size_t h3 = std::hash<int>{}(p.z);

        std::size_t h = h1;
        h ^= h2 + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= h3 + 0x9e3779b9 + (h << 6) + (h >> 2);

        return h;
    }
};