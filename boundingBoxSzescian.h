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

#include "mesh.h"
#include "meshEasy.h"
#include "shaderManager.h"


class BoundingBox{
    public:
        BoundingBox(){
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glGenVertexArrays(1, &geometryVAO);


            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);   
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

            glBindVertexArray(0);
        
        }
    
        void DrawGeometryShader(ShaderManager& shader,const glm::vec3& pos){
            shader.shaderRun();
            glm::mat4 model(1.0f);
            model = glm::translate(model, pos);
            model = glm::scale(model, glm::vec3(1.02f));
            shader.uniformSetMat("model", model);

            glBindVertexArray(geometryVAO);
            glDrawArrays(GL_POINTS, 0, 1);
            glBindVertexArray(0);
        }

        void DrawStencilTest(ShaderManager& shader, ShaderManager& shaderBoundingBox, const glm::vec3& pos, unsigned int VAO){
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); 



            glm::mat4 cube(1.0);
            cube = glm::translate(cube, pos);
            shader.uniformSetMat("model", cube);
            shader.uniformSetInt("StencilTest", 1);
            
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            glStencilMask(0x00);
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);



            glm::mat4 model(1.0f);
            model = glm::translate(model, pos);
            model = glm::translate(model, glm::vec3(0.5f));
            model = glm::scale(model, glm::vec3(1.09f));
            model = glm::translate(model, glm::vec3(-0.5f));
            shader.uniformSetMat("model", model);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            glDisable(GL_STENCIL_TEST);
        }

        void Draw(ShaderManager& shader,const glm::vec3& pos){
            shader.shaderRun();
            glm::mat4 cube(1.0);
            cube = glm::translate(cube, pos);
            cube = glm::scale(cube, glm::vec3(1.02f));
            shader.uniformSetMat("model", cube);
            shader.uniformSetVec3("boxColor", glm::vec3(1.0f, 0.0f, 0.0f));


            glBindVertexArray(VAO);
            glDrawElements(GL_LINES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
            // glDrawElements(GL_LINES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        ~BoundingBox(){
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &geometryVAO);
        }
    
    private:
        unsigned int VBO, VAO, EBO, geometryVAO;
        glm::vec3 blockPos;
        glm::vec3 MinVec;
        glm::vec3 MaxVec;
        std::vector<float> vertices = {
    0.0f, 0.0f, 0.0f, // 0: dół-tył-lewa
    1.0f, 0.0f, 0.0f, // 1: dół-tył-prawa
    1.0f, 0.0f, 1.0f, // 2: dół-przód-prawa
    0.0f, 0.0f, 1.0f, // 3: dół-przód-lewa
    0.0f, 1.0f, 0.0f, // 4: góra-tył-lewa
    1.0f, 1.0f, 0.0f, // 5: góra-tył-prawa
    1.0f, 1.0f, 1.0f, // 6: góra-przód-prawa
    0.0f, 1.0f, 1.0f  // 7: góra-przód-lewa
};
        
        std::vector<unsigned int> indices = {
    // Dolna podstawa
    0, 1,  1, 2,  2, 3,  3, 0,
    // Górna podstawa
    4, 5,  5, 6,  6, 7,  7, 4,
    // Pionowe łączniki między podstawami
    0, 4,  1, 5,  2, 6,  3, 7
};
    

};
