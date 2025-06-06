#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shader.h"

#define width 1920
#define height 1080

glm::mat4 rotate(float angle);

int main(void)
{
    // Setup
    GLFWwindow* window;

    if (!glfwInit())
        return -1;


    window = glfwCreateWindow(width, height, "4D Renderer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Hypercube vertex data
    std::vector<float> g_vertex_buffer_data = {
        -0.5f, -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f, -0.5f,
         0.5f, -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,  0.5f,

        -0.5f, -0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,  0.5f
    };

	// Hypercube index data
    std::vector<unsigned int> g_index_buffer_data = {
        // Cell 0
        4,  5,  6,   4,  6,  7,  
        0,  2,  1,   0,  3,  2,  
        1,  5,  6,   1,  6,  2,  
        0,  7,  4,   0,  3,  7,  
        3,  2,  6,   3,  6,  7,  
        0,  5,  1,   0,  4,  5,  

        // Cell 1
        12, 13, 14,  12, 14, 15,
        8,  10,  9,   8, 11, 10,
        9,  13, 14,   9, 14, 10,
        8, 15, 12,   8, 11, 15,
        11, 10, 14,  11, 14, 15,
        8, 13,  9,   8, 12, 13,

        // Cell 2
        8,   9,  10,   8,  10, 11, 
        0,   2,   1,   0,   3,  2, 
        1,   9,  10,   1,  10,  2, 
        0,  11,   8,   0,   3, 11, 
        3,   2,  10,   3,  10, 11, 
        0,   9,   1,   0,   8,  9, 

        // Cell 3
        12, 13, 14,  12, 14, 15,  
        4,   6,   5,   4,   7,  6, 
        5,  13,  14,   5,  14,  6, 
        4,  15,  12,   4,   7, 15, 
        7,   6,  14,   7,  14, 15, 
        4,  13,   5,   4,  12, 13, 

        // Cell 4
        9,  10,  14,   9,  14, 13, 
        1,   6,   2,   1,   5,  6, 
        2,  10,  14,   2,  14,  6, 
        1,  13,   9,   1,   5, 13, 
        5,   6,  14,   5,  14, 13, 
        1,  10,   2,   1,   9, 10, 

        // Cell 5
        8,  11,  15,   8, 15, 12,  
        0,   7,   3,   0,   4,  7,  
        3,  11,  15,   3,  15,  7,  
        0,  12,   8,   0,   4, 12,  
        4,   7,  15,   4,  15, 12,  
        0,  11,   3,   0,   8, 11,  

        // Cell 6
        11, 10,  14,  11,  14, 15,  
        3,   6,   2,   3,   7,  6,  
        2,  10,  14,   2,  14,  6,  
        3,  15,  11,   3,   7, 15,  
        7,   6,  14,   7,  14, 15,  
        3,  10,   2,   3,  11, 10,  

        // Cell 7
        8,   9,  13,   8,  13, 12, 
        0,   5,   1,   0,   4,  5, 
        1,   9,  13,   1,  13,  5, 
        0,  12,   8,   0,   4, 12, 
        4,   5,  13,   4,  13, 12, 
        0,   9,   1,   0,   8,  9  
    };




    // Vertex buffer
    unsigned int vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(float), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

    // Vertex array
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // Index buffer
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_data.size() * sizeof(unsigned int), &g_index_buffer_data[0], GL_STATIC_DRAW);


    // Matrices
    glm::mat4 Projection = glm::perspective(glm::radians(30.0f), (float)width / (float)height, 0.1f, 100.0f);

    glm::mat4 View = glm::lookAt(
        glm::vec3(4, 3, 3), 
        glm::vec3(0, 0, 0), 
        glm::vec3(0, 1, 0)  
    );

    glm::mat4 Model = glm::mat4(1.0f);


    Shader shader(RES_DIR "/shaders/default_vert.glsl", RES_DIR"/shaders/default_frag.glsl");

    // Set the shader uniforms
    shader.bind();
    shader.setUniformMat4f("uModel", Model);
    shader.setUniformMat4f("uView", View);
    shader.setUniformMat4f("uProj", Projection);
    
	// arbitary camera w value
    float cameraW = 2.0f;
    shader.setUniform1f("uCameraW", cameraW);

     // Enable depth 
     glEnable(GL_DEPTH_TEST);
     glDepthFunc(GL_LESS);
    
    // Enable antialiasing
    glEnable(GL_MULTISAMPLE);
    
    // Wireframe mode 
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // Loop
    while (!glfwWindowShouldClose(window))
    {
        // Rotation angle
        float t = (float)glfwGetTime();     
        float angle = t * glm::radians(45.0f); 

		// new model matrix with rotation 
        glm::mat4 model = rotate(angle);

        // uniform update
        shader.bind();
        shader.setUniformMat4f("uModel", model);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, g_index_buffer_data.size(), GL_UNSIGNED_INT, nullptr);

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Helper function to create a rotation matrix
glm::mat4 rotate(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    glm::mat4 R(1.0f);

    R[0][0] = c;   R[0][3] = -s;
    R[3][0] = s;   R[3][3] = c;

    return R;
}