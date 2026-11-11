#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Display {
public:
    Display(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT);

    // Delete copy constructor and copy assignment operator to prevent multiple window instances
    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;

    bool running = true;
    GLFWwindow* window = nullptr;

    void rys_tlo(float r, float g, float b, float a);

    ~Display();
    
private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};