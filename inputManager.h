#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>


class InputManager{
    public:


    bool LPMClicked(GLFWwindow* window){
        bool leftMouseIsPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        bool leftMouseClicked = leftMouseIsPressed && !leftMouseWasPressed;
        if(leftMouseClicked){
            leftMouseWasPressed = leftMouseIsPressed;
            return true;
        }
        else{
            leftMouseWasPressed = leftMouseIsPressed;
            return false;
        }
    }

    bool PPMClicked(GLFWwindow* window){
        bool rightMouseIsPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
        bool rightMouseClicked = rightMouseIsPressed && !rightMouseWasPressed;
        if(rightMouseClicked){
            rightMouseWasPressed = rightMouseIsPressed;
            return true;
        }
        else{
            rightMouseWasPressed = rightMouseIsPressed;
            return false;
        }
    }

    private:
    bool leftMouseWasPressed = false;
    bool rightMouseWasPressed = false;
    
};