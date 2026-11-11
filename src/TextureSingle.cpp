#include "TextureSingle.h"
#include "stb_image.h"

// Constructor: initialize texture parameters and path settings
TextureSingle::TextureSingle(const char* texturPath_c, int textureNumber_c, bool gamma_c) 
    : texturPath(texturPath_c), textureNumber(textureNumber_c), gamma(gamma_c), ID(0), width(0), height(0), nrChannels(0), data(nullptr) {
    inicjalizacja();
    // load();
}

// Destructor: delete OpenGL texture object
TextureSingle::~TextureSingle() {  
    if (ID != 0) {
        glDeleteTextures(1, &ID);
    }
}

// Initialize OpenGL texture generation and wrap/filter parameters
void TextureSingle::inicjalizacja() {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// Load image from disk using stb_image and upload texture data to GPU
void TextureSingle::load() {
    data = stbi_load(texturPath, &width, &height, &nrChannels, 0);
    if (data) {
        std::cout << nrChannels << std::endl;
        GLenum format1 = GL_RGB;
        GLenum format2 = GL_RGB;
        
        if (nrChannels == 4) format2 = GL_RGBA;
        else if (nrChannels == 1) format2 = GL_RED;

        if (gamma) format1 = GL_SRGB8;
        else if (nrChannels == 4) format1 = GL_RGBA;
        else if (nrChannels == 1) format1 = GL_RED;

        textureBind();

        glTexImage2D(GL_TEXTURE_2D, 0, format1, width, height, 0, format2, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture: " << texturPath << std::endl;
    }
}

// Bind texture to the specified texture unit
void TextureSingle::textureBind() {
    glActiveTexture(GL_TEXTURE0 + textureNumber);
    glBindTexture(GL_TEXTURE_2D, ID);
}

