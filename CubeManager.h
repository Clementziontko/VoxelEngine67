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



class CubeManager{
    public:
        CubeManager(const std::vector<Block>& blocks, const std::unordered_set<glm::ivec3, IVec3Hash>& blockPositions){
            //CreateMeshVertices(blocks, blockPositions);
            // UpdateOffsets(blocks);
           // setAttributes();
        }

        unsigned int getVAO(){
            return VAO;
        }

        void Draw(ShaderManager& shader, CAM& cam, const Sun& sun, const PointLight& pointLight){
            //SUN
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
                shader.uniformSetInt("StencilTest", 0);
                shader.uniformSetVec3("viewPos", cam.getViewPos());
                glm::mat4 cube(1.0);
                cube = glm::translate(cube, glm::vec3(0.0f));
                shader.uniformSetMat("model", cube);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(meshVertices.size()));
            glBindVertexArray(0);

        }


        void UpdateInstances(const std::vector<Block>& blocks_f){
            UpdateOffsets(blocks_f);

            glBindBuffer(GL_ARRAY_BUFFER, VBOInstance);
            glBufferData(GL_ARRAY_BUFFER, offsets.size() * sizeof(glm::vec3), offsets.data(), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

      

        void UpdateAttributes(){
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(Vertex), meshVertices.data(), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        
        ~CubeManager(){
            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBOInstance);
        }

    private:
        unsigned int VBO, VBOInstance, VAO;
       


        std::vector<glm::vec3> offsets;
        std::vector<Vertex> meshVertices;
        
        void UpdateOffsets(const std::vector<Block>& blocks_f){
            offsets.clear();
            offsets.reserve(blocks_f.size());
            for(const auto& blok : blocks_f){
                offsets.push_back(blok.pos);
            }
        }






        // void setAttributes(){
        //     glGenVertexArrays(1, &VAO);
        //     glGenBuffers(1, &VBO);
        //     // glGenBuffers(1, &VBOInstance);

        //     glBindVertexArray(VAO);
        //     glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //     glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(Vertex), meshVertices.data(), GL_STATIC_DRAW);

        //     glEnableVertexAttribArray(0);
        //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, pos));
        //     glEnableVertexAttribArray(1);
        //     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, nor));
        //     glEnableVertexAttribArray(2);
        //     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, uv));

        //     // glBindBuffer(GL_ARRAY_BUFFER, VBOInstance);
        //     // glBufferData(GL_ARRAY_BUFFER, offsets.size() * sizeof(glm::vec3), offsets.data(), GL_STATIC_DRAW);
        //     // glEnableVertexAttribArray(3);
        //     // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        //     // glVertexAttribDivisor(3, 1);


        //     glBindVertexArray(0);
        // }

};