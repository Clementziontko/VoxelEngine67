#include "camera.h"

// Constructor: initialize camera with window reference
CAM::CAM(GLFWwindow* window_c) : window(window_c) {}

// Return the current view matrix
glm::mat4 CAM::getView() {
    return view;
}

// Return current camera position in world space
glm::vec3 CAM::getViewPos() {
    return cameraPos;
}

// Return current camera front vector (look direction)
glm::vec3 CAM::getCamFront() {
    return cameraFront;
}

// Check if camera position has changed since last frame
bool CAM::cameraMoved() {
    return (previousFrameCamPos != cameraPos);
}

// Check if camera orientation/rotation has changed since last frame
bool CAM::cameraRotated() {
    return (previousFrameCamFront != cameraFront);
}

// Update previous frame position and orientation trackers
void CAM::updatingPreviousFramesCamVariables() {
    previousFrameCamPos = cameraPos;
    previousFrameCamFront = cameraFront;
}

// Update delta time, calculate FPS, and refresh the view matrix
void CAM::updtating_cam_vec() {
    float currentFrame = static_cast<float>(glfwGetTime());
    dt = currentFrame - lastFrame;
    lastFrame = currentFrame;

    getSmoothedFPS();
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

// Calculate smoothed frames per second over a 0.2s interval
float CAM::getSmoothedFPS() {
    frameCount++;
    timeAccumulator += dt;

    if (timeAccumulator >= 0.2f) {
        currentFps = static_cast<float>(frameCount) / timeAccumulator;
        frameCount = 0;
        timeAccumulator = 0.0f;
    }

    return currentFps;
}

// Process keyboard input for free-roam camera movement
void CAM::getKeyboard() {
    cameraSpeed = static_cast<float>(camSpeed * dt);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraUp;
    }
}

// Process mouse movement callbacks for camera look direction (Yaw & Pitch)
void CAM::mouse_callback(double xpos, double ypos) {
    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos); 
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // Clamp pitch to prevent screen flipping
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Calculate new front vector from updated yaw and pitch angles
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}