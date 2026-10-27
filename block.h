#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <limits>
#include <algorithm>


#include "boundingBoxSzescian.h"
#include "shaderManager.h"
#include "meshEasy.h"


enum class BlockType
{
    Air,
    Grass,
    Dirt,
    Stone
};

enum class BlockAction {
    Broken,  // Blok został rozwalony
    Placed   // Nowy blok został postawiony
};

struct Block{
        glm::ivec3 pos;
        unsigned int id;
        unsigned type;
        unsigned int texId;
        Block();
        Block(glm::vec3 position, unsigned int blockID, unsigned int blockType) : pos(position), id(blockID), type(blockType){}
};


struct PointLight{
    glm::vec3 position = glm::vec3(0.0f, 3.0f, 0.0f);
    glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct Sun {
    glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f); // Światło padające z góry i pod kątem
    
    glm::vec3 ambient = glm::vec3(0.15f, 0.18f, 0.22f);  // Delikatny błękit nieba jako światło otoczenia
    glm::vec3 diffuse = glm::vec3(1.0f, 0.98f, 0.9f);    // Ciepłe, naturalne światło słoneczne (lekko żółtawe)
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);   // Pełne, białe odblaski
};



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