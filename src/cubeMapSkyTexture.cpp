#include "cubeMapSkyTexture.h"
#include "stb_image.h"



CubeMapTexture::CubeMapTexture(const char* texturPath_c, int textureNumber_c, bool gamma_c) 
    : texturPath(texturPath_c), textureNumber(textureNumber_c), gamma(gamma_c), ID(0), width(0), height(0), nrChannels(0), data(nullptr) {
    inicjalizacja();
}

void CubeMapTexture::inicjalizacja()
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMapTexture::load(){
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    stbi_set_flip_vertically_on_load(false);
    data = stbi_load(texturPath, &width, &height, &nrChannels, 0);

    if(data){
        int faceSize = width / 4;
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
       
        glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
        for (unsigned int i = 0; i < 6; i++) {
            int xOffset = offsets[i][0] * faceSize;
            int yOffset = offsets[i][1] * faceSize;

          
            unsigned char* facePtr = data + (yOffset * width + xOffset) * nrChannels;

            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, faceSize, faceSize, 0, format, GL_UNSIGNED_BYTE, facePtr
            );
        }

        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        stbi_image_free(data);
    }

}

void CubeMapTexture::textureBind(){
    glActiveTexture(GL_TEXTURE0 + textureNumber);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}


CubeMapTexture::~CubeMapTexture() {  
    if (ID != 0) {
        glDeleteTextures(1, &ID);
    }
}