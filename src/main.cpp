#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

#include "Camera.h"
#include "Display.h"
#include "mesh.h"
#include "Model.h"
#include "ShaderManager.h"
#include "meshEasy.h"
#include "BoundingBox.h"
#include "TextureSingle.h"
#include "BlockBoundingBox.h"
#include "UBO.h"
// #include "CubeManager.h"
#include "InputManager.h"
#include "chunk.h"
#include "frustum.h"
#include "World.h"
#include "rayCastCollisionAABB.h"
#include "cubeMapSkyTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



CAM* globalnaKamera = nullptr;

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos){
    if (globalnaKamera != nullptr) {
        globalnaKamera->mouse_callback(xpos, ypos);
    }
}


constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;


int main(){
    Display display(SCR_WIDTH, SCR_HEIGHT);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    
/////Shaders Creating ------------------------------------------------------------------------------
ShaderManager CubeShader(
    "assets/shaders/cubeShader.vert",
    "assets/shaders/cubeShader.frag"
);

ShaderManager boxHighlighterShader(
    "assets/shaders/boxHighlighterShader.vert",
    "assets/shaders/boxHighlighterShader.frag"
);
ShaderManager faceHighlighterShader(
    "assets/shaders/faceHighlighterShader.vert",
    "assets/shaders/faceHighlighterShader.frag", 
    "assets/shaders/faceHighlighterShader.geom"
);


ShaderManager collisionBoxShaderGS(
    "assets/shaders/gsBoundingBoxshader.vert",
    "assets/shaders/gsBoundingBoxshader.frag",
    "assets/shaders/gsBoundingBoxshader.geom"
);

ShaderManager cubeMapShader(
    "assets/shaders/cubeMapShader.vert",
    "assets/shaders/cubeMapShader.frag"
);
//-------------------------------------------------------------------------------------------------------


    CAM cam(display.window);
    globalnaKamera = &cam;
    glfwSetCursorPosCallback(display.window, mouse_callback_wrapper);
    glfwSetInputMode(display.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    //glEnable(GL_FRAMEBUFFER_SRGB);  

    std::vector<float> vertices = {
        // position           normal           UV
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
    
    std::vector<float> skyboxVertices = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    

    MeshEasy skyBox(skyboxVertices, false, false, true);
    stbi_set_flip_vertically_on_load(true);
    constexpr float inf = std::numeric_limits<float>::infinity();
    unsigned int nigger = 0;
    bool czyZmianaSwiata = false;


////World Generation --------------------------------------------------------------------------------------
    World world(200);
    for(auto& chunk : world.GetChunksVec())
    {
        chunk.CreateMeshVertices(world.GetChunksVec(), world.GetChunksVecPos());
        chunk.SettingAttributesForChunk();
    }
//------------------------------------------------------------------------------------------------------------


////Needed objects Creating ---------------------------------------------------------------------------------
    Frustum frustum;
    RayCastCollision RayCast;
    BlockBoundingBox CollisionBox;
    InputManager Input;
//------------------------------------------------------------------------------------------------------------


////Creating textures ---------------------------------------------------------------------------------------
    TextureSingle tx0("assets/textures/DMdirt.png", 0, false);
    TextureSingle tx1("assets/textures/SMdirt.png", 1, false); 
    CubeMapTexture cubeMapSkytx2("assets/textures/Cubemap_Sky_21-512x512.png", 2, false); //21
    tx0.load();
    tx1.load();
    cubeMapSkytx2.load();

    CubeShader.shaderRun();
    CubeShader.uniformSetInt("texture_diffuse1", 0); 
    CubeShader.uniformSetInt("texture_specular1", 1);
    cubeMapShader.shaderRun();
    cubeMapShader.uniformSetInt("skybox", 2);
//------------------------------------------------------------------------------------------------------------

    glm::mat4 projection  = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

////UBO- Projection, CamView ------------------------------------------------------------------------------------------------------------
    UBOBuffer UBOMatrices(128, 0);

    unsigned int blockIndex1 = glGetUniformBlockIndex(CubeShader.shaderProgram, "Matrices");
    unsigned int blockIndex3 = glGetUniformBlockIndex(boxHighlighterShader.shaderProgram, "Matrices");
    unsigned int blockIndex4 = glGetUniformBlockIndex(collisionBoxShaderGS.shaderProgram, "Matrices");
    unsigned int blockIndex5 = glGetUniformBlockIndex(cubeMapShader.shaderProgram, "Matrices");
    unsigned int blockIndex6 = glGetUniformBlockIndex(faceHighlighterShader.shaderProgram, "Matrices");

    glUniformBlockBinding(CubeShader.shaderProgram, blockIndex1, 0);
    glUniformBlockBinding(boxHighlighterShader.shaderProgram, blockIndex3, 0);
    glUniformBlockBinding(collisionBoxShaderGS.shaderProgram, blockIndex4, 0);
    glUniformBlockBinding(cubeMapShader.shaderProgram, blockIndex5, 0);
    glUniformBlockBinding(faceHighlighterShader.shaderProgram, blockIndex6, 0);

    CubeShader.shaderRun();
    world.setUniformsForShaders(CubeShader);
    glBindBuffer(GL_UNIFORM_BUFFER, UBOMatrices.getUBO());
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
//-----------------------------------------------------------------------------------------------------------------------------------------
    

    while (!glfwWindowShouldClose(display.window) && display.running){
        processInput(display.window);

///////Ustawienia Świata-------------------------------------------------------------------------------------------------------------
        cam.getKeyboard();
        cam.updtating_cam_vec();
        std::string title = "Voxel Engine | FPS: " + std::to_string(static_cast<int>(cam.currentFps));
        glfwSetWindowTitle(display.window, title.c_str());


        display.rys_tlo(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glBindBuffer(GL_UNIFORM_BUFFER, UBOMatrices.getUBO());
       
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.getView()));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glStencilMask(0x00);


//////////Cube Model =  Settings, Drawing World, RayCast, Drawing Bounding Box--------------------------------------------------------------------------

        CubeShader.shaderRun();    
        tx0.textureBind();
        tx1.textureBind();
        bool CzyRobicRayCast = false;
       
        if(cam.cameraMoved() || cam.cameraRotated() || czyZmianaSwiata){ CzyRobicRayCast = true; }
        czyZmianaSwiata = false;
        frustum.UpdatePV(projection, cam.getView());
        if(CzyRobicRayCast)
        {
            if(world.getRayCastResults().block != nullptr && world.getRayCastResults().chunk != nullptr)
            {
                world.getRayCastResults().block = nullptr;
                world.getRayCastResults().chunk = nullptr; 
            }
        }
            unsigned int chunkiWszystkie = world.GetChunksVec().size();
            unsigned int chunkiRenderowane = 0;
            size_t totalVertices = 0;
            float tChunks = +(std::numeric_limits<float>::infinity());
            float tBlocks = +(std::numeric_limits<float>::infinity());
        
       cam.updatingPreviousFramesCamVariables();
        for(auto& Chunk : world.GetChunksVec()){

            if(frustum.CheckIfChunkIsToCull(Chunk.getChunkpos())){
                continue;
            }
            chunkiRenderowane++;
            totalVertices += Chunk.GetChunkMeshVertices().size();

            world.DrawChunk(CubeShader, cam, Chunk);

            if(CzyRobicRayCast){ world.RayCast(RayCast, cam, Chunk, tChunks, tBlocks); }
      
        }

//////////Sky Box Shader Setting---------------------------------------------------------------------------------------------------------------
        cubeMapShader.shaderRun();
        cubeMapSkytx2.textureBind();
        glDepthFunc(GL_LEQUAL); 
        // glDepthMask(GL_FALSE);

        skyBox.Draw();

        // glBindVertexArray(skyboxVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);



        if(world.getRayCastResults().block != nullptr && world.getRayCastResults().chunk != nullptr){
            CollisionBox.Draw(boxHighlighterShader, faceHighlighterShader, world.getRayCastResults().block->pos, world.getRayCastResults().hitFace);
            // std::cout<<world.getRayCastResults().hitFace.x<<" "<<world.getRayCastResults().hitFace.y<<" "<<world.getRayCastResults().hitFace.z<<std::endl;
            if(Input.LPMClicked(display.window)){
                glm::ivec3 chunkPosToChange = world.getRayCastResults().chunk->getChunkpos();
                glm::ivec3 blockPosToChange = world.getRayCastResults().block->pos;
                world.destroyBlock(chunkPosToChange, blockPosToChange, BlockAction::Broken);
                czyZmianaSwiata = true;

                world.getRayCastResults().block = nullptr;
                world.getRayCastResults().chunk = nullptr;
            }
            else if(Input.PPMClicked(display.window)){
                glm::ivec3 potentialChunkPos = world.getRayCastResults().chunk->getChunkpos();
                glm::ivec3 blockPosToPutOnBlock = world.getRayCastResults().block->pos + world.getRayCastResults().hitFace;   
                world.putaBlock(potentialChunkPos, blockPosToPutOnBlock, BlockAction::Placed);
                czyZmianaSwiata = true;

                world.getRayCastResults().block = nullptr;
                world.getRayCastResults().chunk = nullptr;
            }
        }

        glfwSwapBuffers(display.window);
        glfwPollEvents();
    }
    return 0;
}
