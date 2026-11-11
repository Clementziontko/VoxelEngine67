#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CAM {
public: 
    CAM(GLFWwindow* window_c);

    glm::mat4 getView();
    glm::vec3 getViewPos();
    glm::vec3 getCamFront();

    float currentFps = 0.0f;

    bool cameraMoved();
    bool cameraRotated();

    void updatingPreviousFramesCamVariables();
    void updtating_cam_vec();
    float getSmoothedFPS();
    void getKeyboard();
    void mouse_callback(double xpos, double ypos);
         
private:
    GLFWwindow* window;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view;

    glm::vec3 previousFrameCamPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 previousFrameCamFront = glm::vec3(0.0f, 0.0f, -1.0f);

    float cameraSpeed;
    float camSpeed = 10.5f;
    float dt = 0.0f;    
    float lastFrame = 0.0f;
    bool firstMouse = true;
    float yaw   = -90.0f;
    float pitch =  0.0f;
    float lastX =  800.0f / 2.0f;
    float lastY =  600.0f / 2.0f;
    float fov   =  45.0f;
    
    float timeAccumulator = 0.0f;
    int frameCount = 0;
};

