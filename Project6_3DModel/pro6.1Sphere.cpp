#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Utils/Utils.h"
#include "Utils/Sphere.h"

using namespace std;

#define numVAOs 1
#define numVBOs 7

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
float pyrLocX, pyrLocY, pyrLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc, tfLoc;
GLuint brickTexture;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat,tMat,rMat;
Sphere mySphere(48);



void setupVertices(void){
   

    std::vector<glm::vec3> vert = mySphere.getVertices();
    std::vector<glm::vec2> tex = mySphere.getTexCoords();
    std::vector<int> ind = mySphere.getIndices();

    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates

    int numIndices = mySphere.getNumIndices();
    for (int i = 0; i < numIndices; i++)
    {
        pvalues.push_back((vert[ind[i]]).x);
        pvalues.push_back((vert[ind[i]]).y);
        pvalues.push_back((vert[ind[i]]).z);

        tvalues.push_back((tex[ind[i]]).s);
        tvalues.push_back((tex[ind[i]]).t);
    }
    


    glGenVertexArrays(numVAOs,vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs,vbo);
    // setup vertices
    glEnableVertexArrayAttrib(vao[0],0);

    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,pvalues.size()*4,&pvalues[0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);
    // setup texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,tvalues.size()*4,&tvalues[0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);
   
}

void init(GLFWwindow* window){
    renderingProgram = Utils::createShaderProgram("GLSL/pro6.1.vs.glsl","GLSL/pro6.1.fs.glsl");
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
    pyrLocY = 0.0f; pyrLocX = 0.0f; pyrLocZ = 0.0f;
    setupVertices();
    // create perspective matrix
    glfwGetFramebufferSize(window,&width,&height);
    aspect = (float)width/(float)height;
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f);// 1.0472 radians = 60 degrees
    brickTexture = Utils::loadTexture("Textures/skybox.jpg");
    
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
    mMat = glm::translate(glm::mat4(1.0f),glm::vec3(pyrLocX,pyrLocY,pyrLocZ));
    // calculate time varying transformation matrix
    tMat = glm::translate(glm::mat4(1.0f),glm::vec3(sin(0.35f*currentTime)*8.0f,cos(0.52f*currentTime)*8.0f,0.0f));
    rMat = glm::rotate(glm::mat4(1.0f),(float)currentTime,glm::vec3(0.0f,1.0f,0.0f));
    // rMat = glm::rotate(rMat,(float)currentTime,glm::vec3(1.0f,0.0f,0.0f));
    // rMat = glm::rotate(rMat,(float)currentTime,glm::vec3(0.0f,0.0f,1.0f));
    // mMat  =  glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
    mMat*=glm::scale(glm::mat4(1.0f),glm::vec3(2.0f,2.0f,2.0f))*rMat;
    mvMat = vMat * mMat;
    // copy perspective and mv matrices to corresponding uniform variables
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(pMat));

    // Texture  
    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,brickTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    // adjust rendering settings and draw model
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES,0,mySphere.getNumIndices());
    // line mode
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
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
    GLFWwindow* window = glfwCreateWindow(600,600,"Project6.1 Sphere",NULL,NULL);
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





