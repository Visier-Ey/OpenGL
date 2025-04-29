#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Utils/Utils.h"

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc, tfLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat,tMat,rMat;

/**
 * @brief Sets up the vertex positions for a 3D cube and initializes the vertex buffer objects (VBOs) and vertex array objects (VAOs).
 * 
 * This function defines the vertex positions for a 3D cube with 36 vertices (12 triangles) and initializes the necessary OpenGL buffers.
 * 
 * Notes:
 * - The vertex positions are defined in a counter-clockwise order.
 * - The function generates and binds VAOs and VBOs.
 * - The vertex data is uploaded to the GPU using glBufferData with GL_STATIC_DRAW usage.
 */

void setupVertices(void){
    float vertexPositions[108]={
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,  
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
    };
    glGenVertexArrays(numVAOs,vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs,vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertexPositions),vertexPositions,GL_STATIC_DRAW);
}

void init(GLFWwindow* window){
    renderingProgram = Utils::createShaderProgram("GLSL/pro4.1.vs.glsl","GLSL/pro4.1.fs.glsl");
    cameraX = 2.0f; cameraY = 0.0f; cameraZ = 16.0f;
    cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
    setupVertices();
    // create perspective matrix
    glfwGetFramebufferSize(window,&width,&height);
    aspect = (float)width/(float)height;
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f);// 1.0472 radians = 60 degrees
}

void display(GLFWwindow* window,double currentTime){
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    mvLoc = glGetUniformLocation(renderingProgram,"mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram,"proj_matrix");

    // create view matrix, mv_matrix, and pass into vertex shader
    vMat = glm::translate(glm::mat4(1.0f),glm::vec3(-cameraX,-cameraY,-cameraZ));
    mMat = glm::translate(glm::mat4(1.0f),glm::vec3(cubeLocX,cubeLocY,cubeLocZ));
    // calculate time varying transformation matrix
    tMat = glm::translate(glm::mat4(1.0f),glm::vec3(sin(0.35f*currentTime)*8.0f,cos(0.52f*currentTime)*8.0f,0.0f));
    rMat = glm::rotate(glm::mat4(1.0f),(float)currentTime,glm::vec3(0.0f,1.0f,0.0f));
    rMat = glm::rotate(rMat,(float)currentTime,glm::vec3(1.0f,0.0f,0.0f));
    rMat = glm::rotate(rMat,(float)currentTime,glm::vec3(0.0f,0.0f,1.0f));
    mMat  = tMat* rMat;
    mvMat = vMat * mMat;
    // copy perspective and mv matrices to corresponding uniform variables
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(pMat));
    // associate VBO with the corresponding vertex attribute in the vertex shader
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);

    // adjust rendering settings and draw model
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT_AND_BACK);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES,0,36);
}

void window_reshape_callback(GLFWwindow* window,int newWidth,int newHeight){
    glViewport(0,0,newWidth,newHeight);
    aspect = (float)newWidth/(float)newHeight;
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f);
}

int main(){
    if (!glfwInit()){exit(1);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow* window = glfwCreateWindow(600,600,"pro4.5PerformanceOptimize",NULL,NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) exit(1);
    glfwSetWindowSizeCallback(window,window_reshape_callback);
    init(window);

    while (!glfwWindowShouldClose(window)){
        display(window,glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(0);
}





