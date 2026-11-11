#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class InputManager {
public:
    bool LPMClicked(GLFWwindow* window);
    bool PPMClicked(GLFWwindow* window);

private:
    bool leftMouseWasPressed = false;
    bool rightMouseWasPressed = false;
};