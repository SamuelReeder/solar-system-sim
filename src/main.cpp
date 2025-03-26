#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// adjust the viewport when the window is resized.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// close the window when ESC is pressed
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(){
    std::cout << "Hello, World!" << std::endl;
}
