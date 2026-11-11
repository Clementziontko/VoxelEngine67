#include "Display.h"

// Constructor: Initialize GLFW, configure context, create window, load GLAD
Display::Display(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT) {
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

// Destructor: Clean up and terminate GLFW
Display::~Display() {
    glfwTerminate();
}

// Clear the color buffer with given RGBA values
void Display::rys_tlo(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Framebuffer size callback to update OpenGL viewport on window resize
void Display::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}