cmake_minimum_required(VERSION 3.10)
project(SolarSystemSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/external/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/external/glm)  # Adjust if using GLM from an external location.
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add your source files. Here we assume your sources are in the src/ folder.
add_executable(SolarSystemSimulation
    src/main.cpp
    external/glad/src/glad.c
    # src/Renderer.cpp
    # src/Simulation.cpp
    # src/other_files.cpp
)

set_source_files_properties(${CMAKE_SOURCE_DIR}/external/glad/src/glad.c PROPERTIES LANGUAGE C)

# Link OpenGL, GLFW, and any other libraries (like GLAD if needed)
target_link_libraries(SolarSystemSimulation
    ${OPENGL_LIBRARIES}  # OpenGL libraries found by CMake
    glfw                 # GLFW library
    # Add GLAD here if you're using a compiled version of it, e.g.:
    # glad
)

