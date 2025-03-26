#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
public:
    // Constructor takes window dimensions and a title.
    Renderer(int width, int height, const char* title);
    ~Renderer();

    // Initializes GLFW, creates a window, loads GLAD, and sets up OpenGL.
    bool initialize();

    // Clears the frame (and later you might add functions to draw objects).
    void clear();

    // Swaps the buffers to display the rendered frame.
    void swapBuffers();

    // Processes input (e.g., checks if ESC is pressed to close the window).
    void processInput();

    // Returns the GLFW window pointer.
    GLFWwindow* getWindow() const;

private:
    GLFWwindow* window;
    int width, height;
    const char* title;
};

#endif
