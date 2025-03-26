#include "Renderer.h"
#include "Sphere.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
    }
    return shader;
}

unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

int main() {
    Renderer renderer(800, 600, "Solar System Simulation");
    if (!renderer.initialize()) {
        std::cerr << "Renderer initialization failed." << std::endl;
        return -1;
    }

    // Vertex and fragment shader sources.
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        // Uniforms for transformation matrices.
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(0.8, 0.5, 0.2, 1.0);
        }
    )";

    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    Sphere sphere(1.0f, 36, 18);  // Radius 1.0, with 36 sectors and 18 stacks.

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.f/600.f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    while (!glfwWindowShouldClose(renderer.getWindow())) {
        renderer.processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        renderer.clear();

        glUseProgram(shaderProgram);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        sphere.draw();

        renderer.swapBuffers();
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    return 0;
}
