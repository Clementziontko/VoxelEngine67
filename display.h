#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Display{
    public:
        Display(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT){
            if (!glfwInit()) {
                std::cerr << "Nie udalo sie zainicjalizowac GLFW\n";
                running = false;
            }
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Test", NULL, NULL);
            if (window == NULL) {
                std::cerr << "Nie udalo sie utworzyc okna GLFW\n";
                glfwTerminate();
                running = false;
            }

            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            
            // glfwSetScrollCallback(window, scroll_callback);
            
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cerr << "Nie udalo sie zainicjalizowac GLAD\n";
                running = false;
            }
            
        }

        Display(const Display&) = delete;
        Display& operator=(const Display&) = delete;

        bool running = true;
        GLFWwindow* window;

        void rys_tlo(float r, float g, float b, float a){
            glClearColor(r, g, b, a);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        ~Display(){
            glfwTerminate();
        }
        
    private:
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        }

};