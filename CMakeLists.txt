cmake_minimum_required(VERSION 3.10)
project(SolarSystemSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
# find_package(glfw3 3.3 REQUIRED)

# file(GLOB_RECURSE SOURCES src/*.cpp)
# add_executable(SolarSystemSimulation
#     ${SOURCES}
#     external/glad/src/glad.c
# )

# set_source_files_properties(${CMAKE_SOURCE_DIR}/external/glad/src/glad.c PROPERTIES LANGUAGE C)

# # Link OpenGL, GLFW, and any other libraries (like GLAD if needed)
# target_link_libraries(SolarSystemSimulation
#     ${OPENGL_LIBRARIES}  # OpenGL libraries found by CMake
#     glfw                 # GLFW library
#     # Add GLAD here if you're using a compiled version of it, e.g.:
#     # glad
# )


include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/glad/include
    ${PROJECT_SOURCE_DIR}/external/glm
)

# source files
file(GLOB SOURCES 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/external/glad/src/glad.c
)

# executable
add_executable(${PROJECT_NAME} ${SOURCES})

# link libraries
target_link_libraries(${PROJECT_NAME} 
    ${OPENGL_LIBRARIES}
    glfw
)

# copy shaders to build directory
file(COPY ${PROJECT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

