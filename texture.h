#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "stb_image.h"



class TextureSingle{
    public:
        TextureSingle(const char* texturPath_c, int textureNumber_c, bool gamma_c = false) 
        : texturPath(texturPath_c), textureNumber(textureNumber_c), gamma(gamma_c){
            inicjalizacja();
            // load();
        }

        TextureSingle(const TextureSingle&) = delete;
        TextureSingle& operator=(const TextureSingle&) = delete;

        void inicjalizacja(){
            glGenTextures(1, &ID);
            glBindTexture(GL_TEXTURE_2D, ID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        void load(){
            data = stbi_load(texturPath, &width, &height, &nrChannels, 0);
            if (data){
                std::cout<<nrChannels<<std::endl;
                GLenum format1 = GL_RGB;
                GLenum format2 = GL_RGB;
                if (nrChannels == 4) format2 = GL_RGBA;
                else if (nrChannels == 1) format2 = GL_RED;

                if(gamma) format1 = GL_SRGB8;
                else if (nrChannels == 4) format1 = GL_RGBA;
                else if (nrChannels == 1) format1 = GL_RED;

                textureBind();

                glTexImage2D(GL_TEXTURE_2D, 0, format1, width, height, 0, format2, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                stbi_image_free(data);
            }
            else{
                std::cout << "Failed to load texture" << std::endl;
            }
        }

        void textureBind(){
            glActiveTexture(GL_TEXTURE0 + textureNumber);
            glBindTexture(GL_TEXTURE_2D, ID);
        }

        ~TextureSingle(){  
            glDeleteTextures(1, &ID);
        }

    private:
        const char* texturPath;
        unsigned int ID;
        int width, height, nrChannels, textureNumber;
        unsigned char *data;
        bool gamma;
};