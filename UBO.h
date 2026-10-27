#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class UBOBuffer{
    public:
        UBOBuffer(unsigned int BufferSize_c, unsigned int BufferBase_c) : BufferSize(BufferSize_c), BufferBase(BufferBase_c)
        {
            glGenBuffers(1, &UBO);
            glBindBuffer(GL_UNIFORM_BUFFER, UBO);
            glBufferData(GL_UNIFORM_BUFFER, BufferSize, NULL, GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            glBindBufferBase(GL_UNIFORM_BUFFER, BufferBase, UBO);
        }

        UBOBuffer(const UBOBuffer&) = delete;
        UBOBuffer& operator=(const UBOBuffer&) = delete;


        unsigned int getUBO(){
            return UBO;
        }

        ~UBOBuffer(){
            glDeleteBuffers(1, &UBO);
        }

    private:
        unsigned int UBO, BufferSize, BufferBase;
};