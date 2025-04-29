#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include "Utils/Utils.h"

using namespace std;

#define numVAOs 2
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
float pyrLocX, pyrLocY, pyrLocZ;
GLuint TextureSolor;
GLuint TextureEarth;
GLuint TextureMoon;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc, tfLoc;
int width, height;
float aspect;
float timeFactor;   
glm::mat4 pMat, vMat, mMat, mvMat,tMat,rMat;
stack<glm::mat4> mvStack;
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
    float cubePositions[108]={
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
    float pyramidPositions[54] =  { 
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
        // 前面   
        1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,      
        // 右面   
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,   
        // 后面  
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,    
        // 左面   
        -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,   
        // 底面左前 
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f     
        // 底面右后
    };
    float pyrTexCoords[36] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,   
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(numVAOs,vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs,vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(pyramidPositions),pyramidPositions,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(pyrTexCoords),pyrTexCoords,GL_STATIC_DRAW);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);

}

void init(GLFWwindow* window){
    renderingProgram = Utils::createShaderProgram("GLSL/pro5.1.vs.glsl","GLSL/pro5.1.fs.glsl");
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
    cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
    pyrLocX = 0.0f; pyrLocY = 2.0f; pyrLocZ = 0.0f;
    setupVertices();
    TextureEarth=Utils::loadTexture("Textures/brick1.jpg");
    TextureMoon=Utils::loadTexture("Textures/squareMoonBump.jpg");
    TextureSolor=Utils::loadTexture("Textures/castleroof.jpg");
}

void display(GLFWwindow* window,double currentTime){
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);
    mvLoc = glGetUniformLocation(renderingProgram,"mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram,"proj_matrix");
    // set projection matrix
    glfwGetFramebufferSize(window,&width,&height);
    aspect = (float)width/(float)height;
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f);
    glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(pMat));
    // pass the projection matrix to the shader
    //TODO: use pyr as the solar
    vMat = glm::lookAt(glm::vec3(cameraX,cameraY,cameraZ),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
    // push the view matrix onto the stack
    mvStack.push(vMat);
    mvStack.top()*=glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
    mvStack.top()*=glm::rotate(glm::mat4(1.0f),float(currentTime),glm::vec3(1.0f,0.0f,0.0f));
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvStack.top()));
    
    // attach the texture to the shader
    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,TextureSolor);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    // associate VBO with the corresponding vertex attribute in the vertex shader
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    // adjust rendering settings and draw model
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArraysInstanced(GL_TRIANGLES,0,36,1);
    // TODO: use cube as the earth
    mMat = glm::translate(glm::mat4(1.0f),glm::vec3(sin((float)currentTime)*4.0,0.0f,cos((float)currentTime)*4.0));
    mvStack.push(mvStack.top()*mMat);
    mvStack.top()*=glm::rotate(glm::mat4(1.0f),float(currentTime),glm::vec3(0.0f,1.0f,0.0f));
    mvStack.top()*=glm::scale(glm::mat4(1.0f),glm::vec3(0.25f,0.25f,0.25f));
    // 
    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,TextureMoon);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    // 
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArraysInstanced(GL_TRIANGLES,0,36,1);
    // TODO: use cube as the moon
    mMat = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,sin((float)currentTime)*2.0f,cos((float)currentTime)*2.0f));
    mvStack.push(mvStack.top()*mMat);
    mvStack.top()*=glm::rotate(glm::mat4(1.0f),float(currentTime),glm::vec3(0.0f,0.0f,1.0f));
    mvStack.top()*=glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f));
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArraysInstanced(GL_TRIANGLES,0,36,1);
    // pop the matrix stack to erase the moon transformation
    mvStack.pop();
    // pop the matrix stack to erase the earth transformation
    mvStack.pop();
    // pop the matrix stack to erase the solar transformation
    mvStack.pop();
}

int main(){
    if (!glfwInit()){exit(1);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow* window = glfwCreateWindow(600,600,"pro5.2MatrixStackEasySolarGalaxyTexture",NULL,NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) exit(1);

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





