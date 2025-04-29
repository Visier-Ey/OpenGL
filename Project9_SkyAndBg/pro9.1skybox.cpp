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

#define numVAOs 3
#define numVBOs 10
#define WIDTH 800
#define HEIGHT 800

double cameraX, cameraY, cameraZ;
float sphLocX, sphLocY, sphLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc, tfLoc, nLoc;
GLuint skyboxTexture;
GLuint globalAmbLoc, ambLoc, diffLoc, speLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;
float cameraRotateAlpha = 0.0f;
float cameraRotateBeta = 0.0f;
float amt = 0.0f;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat, invTrMat, cameraRotateMat;
glm::vec3 currentLightPos, lightPos;
glm::vec3 eye, at, up;
float eyeToAtDistance;
bool mvRotate = true;
float lightPosv[3];
glm::vec3 initialLightLoc = glm::vec3(5.0f, 2.0f, 2.0f);
// white light
float globalAmbient[4] = {0.7f, 0.7f, 0.7f, 1.0f};
float lightAmbient[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float lightSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
// material
float *matAmb = Utils::goldAmbient();
float *matDif = Utils::goldDiffuse();
float *matSpe = Utils::goldSpecular();
float matShi = Utils::goldShininess();
Sphere mySphere(24);
Torus myTorus(1.7f, 0.4f, 24);

float toRadians(float degrees)
{
    return degrees * (3.14159f / 180.0f);
}

void installLights(glm::mat4 v_matrix)
{
    lightPos = glm::vec3(v_matrix * glm::vec4(currentLightPos, 1.0)); // light position in eye coordinates
    lightPosv[0] = lightPos.x;
    lightPosv[1] = lightPos.y;
    lightPosv[2] = lightPos.z;

    globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
    ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
    diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
    speLoc = glGetUniformLocation(renderingProgram, "light.specular");
    posLoc = glGetUniformLocation(renderingProgram, "light.position");
    mAmbLoc = glGetUniformLocation(renderingProgram, "material.ambient");
    mDiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
    mSpecLoc = glGetUniformLocation(renderingProgram, "material.specular");
    mShiLoc = glGetUniformLocation(renderingProgram, "material.shininess");
    // TODO : set the light and material properties in the shader
    glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
    glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
    glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
    glProgramUniform4fv(renderingProgram, speLoc, 1, lightSpecular);
    glProgramUniform3fv(renderingProgram, posLoc, 1, lightPosv);
    glProgramUniform4fv(renderingProgram, mAmbLoc, 1, matAmb);
    glProgramUniform4fv(renderingProgram, mDiffLoc, 1, matDif);
    glProgramUniform4fv(renderingProgram, mSpecLoc, 1, matSpe);
    glProgramUniform1f(renderingProgram, mShiLoc, matShi);
}

void setupVertices(void)
{
    //TODO : SKYBOX
    float cubeVertexPositions[108] =
	{	-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};
	float cubeTextureCoord[72] =
	{	1.00f, 0.6666666f, 1.00f, 0.3333333f, 0.75f, 0.3333333f,	// back face lower right
		0.75f, 0.3333333f, 0.75f, 0.6666666f, 1.00f, 0.6666666f,	// back face upper left
		0.75f, 0.3333333f, 0.50f, 0.3333333f, 0.75f, 0.6666666f,	// right face lower right
		0.50f, 0.3333333f, 0.50f, 0.6666666f, 0.75f, 0.6666666f,	// right face upper left
		0.50f, 0.3333333f, 0.25f, 0.3333333f, 0.50f, 0.6666666f,	// front face lower right
		0.25f, 0.3333333f, 0.25f, 0.6666666f, 0.50f, 0.6666666f,	// front face upper left
		0.25f, 0.3333333f, 0.00f, 0.3333333f, 0.25f, 0.6666666f,	// left face lower right
		0.00f, 0.3333333f, 0.00f, 0.6666666f, 0.25f, 0.6666666f,	// left face upper left
		0.25f, 0.3333333f, 0.50f, 0.3333333f, 0.50f, 0.0000000f,	// bottom face upper right
		0.50f, 0.0000000f, 0.25f, 0.0000000f, 0.25f, 0.3333333f,	// bottom face lower left
		0.25f, 1.0000000f, 0.50f, 1.0000000f, 0.50f, 0.6666666f,	// top face upper right
		0.50f, 0.6666666f, 0.25f, 0.6666666f, 0.25f, 1.0000000f		// top face lower left
	};

    glGenVertexArrays(numVAOs, vao);
    glGenBuffers(numVBOs, vbo);

    if (mySphere.getIndices().size() > 0)
    {
        std::vector<glm::vec3> vert = mySphere.getVertices();
        std::vector<glm::vec2> tex = mySphere.getTexCoords();
        std::vector<glm::vec3> norm = mySphere.getNormals();
        std::vector<int> ind = mySphere.getIndices();

        std::vector<float> pvalues; // vertex positions
        std::vector<float> tvalues; // texture coordinates
        std::vector<float> nvalues; // normal vectors
        for (int i = 0; i < mySphere.getNumVertices(); i++)
        {
            pvalues.push_back(vert[i].x);
            pvalues.push_back(vert[i].y);
            pvalues.push_back(vert[i].z);

            tvalues.push_back(tex[i].s);
            tvalues.push_back(tex[i].t);

            nvalues.push_back(norm[i].x);
            nvalues.push_back(norm[i].y);
            nvalues.push_back(norm[i].z);
        }

        glBindVertexArray(vao[0]);
        // vertex positions
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
        // setup texture coordinates
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
        // setup normals
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
        // setup indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
    }
    
    if (myTorus.getIndices().size() > 0)
    {
        std::vector<glm::vec3> vert = myTorus.getVertices();
        std::vector<glm::vec2> tex = myTorus.getTexCoords();
        std::vector<glm::vec3> norm = myTorus.getNormals();
        std::vector<int> ind = myTorus.getIndices();

        std::vector<float> pvalues; // vertex positions
        std::vector<float> tvalues; // texture coordinates
        std::vector<float> nvalues; // normal vectors

        for (int i = 0; i < myTorus.getNumVertices(); i++)
        {
            pvalues.push_back(vert[i].x);
            pvalues.push_back(vert[i].y);
            pvalues.push_back(vert[i].z);
            tvalues.push_back(tex[i].s);
            tvalues.push_back(tex[i].t);
            nvalues.push_back(norm[i].x);
            nvalues.push_back(norm[i].y);
            nvalues.push_back(norm[i].z);
        }
        glBindVertexArray(vao[1]);
        // setup vertices
        glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
        glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
        // setup texture coordinates
        glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
        glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
        // setup normals
        glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
        glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
        // setup indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[7]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
    }
    //TODO : SKYBOX
    glBindVertexArray(vao[2]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions) * 4, cubeVertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTextureCoord) * 4, cubeTextureCoord, GL_STATIC_DRAW);
}

void init(GLFWwindow *window)
{
    renderingProgram = Utils::createShaderProgram("GLSL/BlinnPhongShaders/vertShader.glsl", "GLSL/BlinnPhongShaders/fragShader.glsl");
    cameraX = 0.0f;
    cameraY = 0.0f;
    cameraZ = 8.0f;
    sphLocX = 0.0f;
    sphLocY = 0.0f;
    sphLocZ = 0.0f;
    eye = glm::vec3(cameraX, cameraY, cameraZ);
    at = glm::vec3(sphLocX, sphLocY, sphLocZ);
    up = glm::vec3(0.0f, -1.0f, 0.0f);
    eyeToAtDistance = glm::distance(eye, at);
    setupVertices();
    skyboxTexture = Utils::loadTexture("Textures/image.png");
    // create perspective matrix
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

void display(GLFWwindow *window, double currentTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");

    vMat = glm::lookAt(eye, at, up);
   

    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cameraX, cameraY, cameraZ));
  

    // currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
    // amt = currentTime * 100.0f;
    // rMat = glm::rotate(glm::mat4(1.0f), toRadians(amt), glm::vec3(0.0f, 0.0f, 1.0f));
    // currentLightPos = glm::vec3(glm::vec4(currentLightPos, 1.0f));

    // TODO : set the light position

    // installLights(vMat);
    mvMat = vMat * mMat;
    // invTrMat = glm::transpose(glm::inverse(mMat)); // for normal matrix
    // copy perspective and mv matrices to corresponding uniform variables
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    // glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

    // TODO : Skybox
    glBindVertexArray(vao[2]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyboxTexture);

    glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	// cube is CW, but we are viewing the inside
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// glEnable(GL_DEPTH_TEST);
    // glUseProgram(renderingProgram);
    // // TODO : Shader
    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CCW);
    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
    // // draw the sphere
    // glBindVertexArray(vao[0]);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(1);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    // glDrawElements(GL_TRIANGLES, mySphere.getNumIndices(), GL_UNSIGNED_INT, 0);

    // // draw the torus
    // glBindVertexArray(vao[1]);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(1);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[7]);
    // glDrawElements(GL_TRIANGLES, myTorus.getNumIndices(), GL_UNSIGNED_INT, 0);

    // // TODO : draw the light source as a small white sphere
    // mMat = glm::translate(glm::mat4(1.0f), currentLightPos);
    // mMat = glm::scale(mMat, glm::vec3(0.6f, 0.6f, 0.6f));
    // mvMat = vMat * mMat;
    // glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    // glBindVertexArray(vao[0]);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(1);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    // glDrawElements(GL_TRIANGLES, mySphere.getNumIndices(), GL_UNSIGNED_INT, 0);

    // // line mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void window_reshape_callback(GLFWwindow *window, int newWidth, int newHeight)
{
    glViewport(0, 0, newWidth, newHeight);
    aspect = (float)newWidth / (float)newHeight;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}
void window_scroll_callback(GLFWwindow *window, double dx, double dy)
{
    // due to the rotated camera, we need to adjust the cameraZ , cameraX and cameraY
    cameraX = (eyeToAtDistance - 0.1f * dy) * sin(cameraRotateAlpha) * cos(cameraRotateBeta);
    cameraY = (eyeToAtDistance - 0.1f * dy) * sin(cameraRotateAlpha) * sin(cameraRotateBeta);
    cameraZ = (eyeToAtDistance - 0.1f * dy) * cos(cameraRotateAlpha);
    eye = glm::vec3(cameraX, cameraY, cameraZ);
    eyeToAtDistance = glm::distance(eye, at);
}
void window_CursorPos_callback(GLFWwindow *window, double x, double y)
{
    static bool isRightMouseDown = false;
    static bool isLeftMouseDown = false;
    static double lastX = x, lastY = y;
    static double threshold = std::numeric_limits<double>::epsilon();
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (!isRightMouseDown)
        {
            isRightMouseDown = true;
            lastX = x;
            lastY = y;
        }
        else
        {
            if (std::abs(x - lastX) > threshold || std::abs(y - lastY) > threshold)
            {
                cameraRotateAlpha -= (y-lastY) * 0.01f;
                cameraRotateBeta += (x-lastX) * 0.01f;

                cameraX = eyeToAtDistance * sin(cameraRotateAlpha) * cos(cameraRotateBeta);
                cameraY = eyeToAtDistance * sin(cameraRotateAlpha) * sin(cameraRotateBeta);
                cameraZ = eyeToAtDistance * cos(cameraRotateAlpha);
                eye = glm::vec3(cameraX, cameraY, cameraZ);
                lastX = x;
                lastY = y;
            }
        }
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (!isLeftMouseDown)
        {
            isLeftMouseDown = true;
            lastX = x;
            lastY = y;
        }
        else
        {
            if (std::abs(x - lastX) > threshold || std::abs(y - lastY) > threshold)
            {
                lastX = x;
                lastY = y;
            }
        }
    }
    else if (isRightMouseDown)
    {
        isRightMouseDown = false;
    }
    else if (isLeftMouseDown)
    {
        isLeftMouseDown = false;
    }
}
void window_Key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    // if space key is pressed, stop the rotation
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        mvRotate = !mvRotate;
    }
}

int main()
{
    if (!glfwInit())
    {
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Project7.3.BlinnPhong Light", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        exit(1);
    glfwSetWindowSizeCallback(window, window_reshape_callback);
    glfwSetScrollCallback(window, window_scroll_callback);
    glfwSetCursorPosCallback(window, window_CursorPos_callback);
    glfwSetKeyCallback(window, window_Key_callback);

    init(window);

    while (!glfwWindowShouldClose(window))
    {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(0);
}
