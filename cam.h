#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>



class CAM{
    public: 
        CAM(GLFWwindow* window_c) : window(window_c){
            
        }

        glm::mat4 getView(){
            return view;
        }

        glm::vec3 getViewPos(){
            return cameraPos;
        }

        glm::vec3 getCamFront(){
            return cameraFront;
        }

        void updtating_cam_vec(){
            float currentFrame = static_cast<float>(glfwGetTime());
            dt = currentFrame - lastFrame;
            lastFrame = currentFrame;
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        }

        void getKeyboard(){
            cameraSpeed = static_cast<float>(camSpeed * dt);
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
                cameraPos += cameraSpeed * cameraFront;
            }
            if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
                cameraPos -= cameraSpeed * cameraFront;
            }
            if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
                cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            }
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
                cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            }
            if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
                cameraPos += cameraSpeed * cameraUp;
            }
            if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
                cameraPos -= cameraSpeed * cameraUp;
            }
        }

        void mouse_callback(double xpos, double ypos){
            if(firstMouse){
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }
        
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; 
            lastX = xpos;
            lastY = ypos;

            float sensitivity = 0.1f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw   += xoffset;
            pitch += yoffset;

            if(pitch > 89.0f)
                pitch = 89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;

            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(direction);
        } 
         
    private:
        GLFWwindow* window;
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f,  3.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
        glm::mat4 view;

        float cameraSpeed;
        float camSpeed = 10.5f;
        float dt = 0.0f;	
        float lastFrame = 0.0f;
        bool firstMouse = true;
        float yaw   = -90.0f;
        float pitch =  0.0f;
        float lastX =  800.0f / 2.0;
        float lastY =  600.0 / 2.0;
        float fov   =  45.0f;
        
};




