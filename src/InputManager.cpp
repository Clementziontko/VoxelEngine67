#include "InputManager.h"

// Check if the left mouse button was just clicked (single-press detection)
bool InputManager::LPMClicked(GLFWwindow* window) {
    bool leftMouseIsPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool leftMouseClicked = leftMouseIsPressed && !leftMouseWasPressed;
    if (leftMouseClicked) {
        leftMouseWasPressed = leftMouseIsPressed;
        return true;
    }
    else {
        leftMouseWasPressed = leftMouseIsPressed;
        return false;
    }
}

// Check if the right mouse button was just clicked (single-press detection)
bool InputManager::PPMClicked(GLFWwindow* window) {
    bool rightMouseIsPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    bool rightMouseClicked = rightMouseIsPressed && !rightMouseWasPressed;
    if (rightMouseClicked) {
        rightMouseWasPressed = rightMouseIsPressed;
        return true;
    }
    else {
        rightMouseWasPressed = rightMouseIsPressed;
        return false;
    }
}

