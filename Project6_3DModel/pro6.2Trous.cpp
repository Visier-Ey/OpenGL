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
#include "Utils/Torus.h"

using namespace std;

#define numVAOs 4
#define numVBOs 7
#define WIDTH 800
#define HEIGHT 800

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
float pyrLocX, pyrLocY, pyrLocZ;
float torLocX, torLocY, torLocZ;
float sphLocX, sphLocY, sphLocZ;
float uslLocX, uslLocY, uslLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc, tfLoc;
GLuint TextureSolor;
GLuint TextureEarth;
GLuint TextureMoon;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat,tMat,rMat;
glm::mat4 mvSolarMat,mvEarthMat,mvTorusMat,mvMoonMat;
Sphere mySphere(48);
Torus myTorus(48,0.5f,0.05f);

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void setupVertices(void){
    std::vector<glm::vec3> vertTorus = myTorus.getVertices();
    std::vector<glm::vec2> texTorus = myTorus.getTexCoords();
    std::vector<glm::vec3> normTorus = myTorus.getNormals();
    std::vector<int> indTorus = myTorus.getIndices();

    std::vector<glm::vec3> vertSphere = mySphere.getVertices();
    std::vector<glm::vec2> texSphere = mySphere.getTexCoords();
    std::vector<glm::vec3> normSphere = mySphere.getNormals();
    std::vector<int> indSphere = mySphere.getIndices();

    int TroNumVertices = myTorus.getNumVertices();
    std::vector<float> troPvalues; // vertex positions
    std::vector<float> troTvalues; // texture coordinates
    std::vector<float> troNvalues; // normal vectors
    for (int i = 0; i < TroNumVertices; i++)
    {
        troPvalues.push_back(vertTorus[i].x);
        troPvalues.push_back(vertTorus[i].y);
        troPvalues.push_back(vertTorus[i].z);

        troTvalues.push_back(texTorus[i].s);
        troTvalues.push_back(texTorus[i].t);

        troNvalues.push_back(normTorus[i].x);
        troNvalues.push_back(normTorus[i].y);
        troNvalues.push_back(normTorus[i].z);
    }

    int SphNumIndices = mySphere.getNumIndices();
    std::vector<float> sphPvalues; // vertex positions
    std::vector<float> sphTvalues; // texture coordinates
    std::vector<float> sphNvalues; // normal vectors
    for (int i = 0; i < SphNumIndices; i++)
    {
        sphPvalues.push_back(vertSphere[indSphere[i]].x);
        sphPvalues.push_back(vertSphere[indSphere[i]].y);
        sphPvalues.push_back(vertSphere[indSphere[i]].z);

        sphTvalues.push_back(texSphere[indSphere[i]].s);
        sphTvalues.push_back(texSphere[indSphere[i]].t);

        sphNvalues.push_back(normSphere[indSphere[i]].x);
        sphNvalues.push_back(normSphere[indSphere[i]].y);
        sphNvalues.push_back(normSphere[indSphere[i]].z);
    }
    

    glGenVertexArrays(numVAOs,vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs,vbo);
    // setup vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, troPvalues.size() * 4, &troPvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);
    glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, troTvalues.size() * 4, &troTvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1,2,GL_FLOAT,false,0,0);
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, troNvalues.size() * 4, &troNvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2,3,GL_FLOAT,false,0,0);
    glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indTorus.size() * 4, &indTorus[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    // sphere
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    glBufferData(GL_ARRAY_BUFFER, sphPvalues.size() * 4, &sphPvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
    glBufferData(GL_ARRAY_BUFFER, sphTvalues.size() * 4, &sphTvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1,2,GL_FLOAT,false,0,0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
    glBufferData(GL_ARRAY_BUFFER, sphNvalues.size() * 4, &sphNvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2,3,GL_FLOAT,false,0,0);
    glEnableVertexAttribArray(2);
}

void init(GLFWwindow* window){
    renderingProgram = Utils::createShaderProgram("GLSL/pro6.2.vs.glsl","GLSL/pro6.2.fs.glsl");
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
    uslLocX = 0.0f; uslLocY = 0.0f; uslLocZ = 0.0f;
    setupVertices();
    // create perspective matrix
    glfwGetFramebufferSize(window,&width,&height);
    aspect = (float)width/(float)height;
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f);// 1.0472 radians = 60 degrees
    TextureSolor = Utils::loadTexture("Textures/sun.jpg");
    TextureEarth = Utils::loadTexture("Textures/earth.jpg");
    TextureMoon = Utils::loadTexture("Textures/moon.jpg");
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
    mMat = glm::translate(glm::mat4(1.0f),glm::vec3(uslLocX,uslLocY,uslLocZ));
    // calculate time varying transformation matrix
    tMat = glm::translate(glm::mat4(1.0f),glm::vec3(sin(0.35f*currentTime)*2.0f,cos(0.52f*currentTime)*2.0f,0.0f));
    rMat = glm::rotate(glm::mat4(1.0f),(float)currentTime,glm::vec3(0.0f,1.0f,0.0f));
    rMat = glm::rotate(rMat,(float)currentTime,glm::vec3(1.0f,0.0f,0.0f));
    rMat = glm::rotate(rMat,(float)currentTime,glm::vec3(0.0f,0.0f,1.0f));
    // mMat  =  glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
    mMat*=glm::scale(glm::mat4(1.0f),glm::vec3(2.0f,2.0f,2.0f));
    mvMat = vMat * mMat;
    // copy perspective and mv matrices to corresponding uniform variables
    glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(pMat));
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    // TODO : set the Solar 
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
    glEnableVertexAttribArray(1);
    glBindTexture(GL_TEXTURE_2D,TextureSolor);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    mvSolarMat = mvMat;
    // mvSolarMat*=glm::rotate(glm::mat4(1.0f),(float)currentTime,glm::vec3(0.0f,1.0f,0.0f));
    // mvSolarMat = glm::rotate(mvSolarMat,(float)currentTime,glm::vec3(1.0f,0.0f,0.0f));
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(glm::rotate(mvSolarMat,(float)currentTime*0.3f,glm::vec3(0.0f,1.0f,0.0f))));
    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
    // line mode
    // // TODO : set the trous
    // mvTorusMat = glm::rotate(mvSolarMat,(float)currentTime,glm::vec3(0.0f,1.0f,0.0f));
    // mvTorusMat = glm::translate(mvTorusMat,glm::vec3(0.0f,0.0f,0.2f));
    // mvTorusMat = glm::rotate(mvTorusMat,(float)currentTime,glm::vec3(1.0f,0.0f,0.0f));
    // mvTorusMat*=glm::scale(glm::mat4(1.0f),glm::vec3(4.0f,4.0f,4.0f));
    // glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvTorusMat));
    // glBindVertexArray(vao[0]);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	// glDrawElements(GL_TRIANGLES, myTorus.getIndices().size(), GL_UNSIGNED_INT, 0);

    // TODO : set the earth
    mvEarthMat = mvSolarMat * tMat;
    // mvEarthMat = glm::rotate(mvEarthMat,(float)currentTime,glm::vec3(0.0f,1.0f,0.0f));
    mvEarthMat = glm::translate(mvEarthMat,glm::vec3(0.0f,0.0f,2.0f));
    mvEarthMat = glm::scale(mvEarthMat,glm::vec3(0.25f,0.25f,0.25f));
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvEarthMat));
    glBindVertexArray(vao[1]);
    glEnableVertexAttribArray(1);
    glBindTexture(GL_TEXTURE_2D,TextureEarth);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

    // TODO : set the moon
    mvMoonMat = mvEarthMat;
    mvMoonMat = glm::rotate(mvMoonMat,(float)currentTime,glm::vec3(0.0f,1.0f,0.0f));
    mvMoonMat = glm::translate(mvMoonMat,glm::vec3(0.0f,0.0f,2.0f));
    mvMoonMat = glm::scale(mvMoonMat,glm::vec3(0.25f,0.25f,0.25f));
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvMoonMat));
    glBindVertexArray(vao[1]);
    glEnableVertexAttribArray(1);
    glBindTexture(GL_TEXTURE_2D,TextureMoon);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
}

void window_reshape_callback(GLFWwindow* window,int newWidth,int newHeight){
    glViewport(0,0,newWidth,newHeight);
    aspect = (float)newWidth/(float)newHeight;
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f);
}

void window_scroll_callback(GLFWwindow* window,double dx,double dy){
    cameraZ += dy*0.1f;
}


int main(){
    if (!glfwInit()){exit(1);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,"Project6.2 Trous",NULL,NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) exit(1);
    glfwSetWindowSizeCallback(window,window_reshape_callback);
    glfwSetScrollCallback(window,window_scroll_callback);
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





