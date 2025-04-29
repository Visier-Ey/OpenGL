#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Utils
{
private:
    /* data */
public:
    Utils(/* args */);
    ~Utils();
    // add the declarations of the functions here
    static string readShaderSource(const char *filePath);
    static void printShaderLog(GLuint shader);
    static void printProgramLog(int prog);
    static bool checkOpenGLError();
    static GLuint createShaderProgram(string vshaderStr, string fshaderStr);
    static GLuint loadTexture(const char *textureFilePath);
    static float normalize(float value, float min, float max, float newMin, float newMax);
    static GLuint loadCubeMap(const char *cubeMapDir);
    // TODO : add the Ambient, Diffuse, Specular, Shininess for materials
    // gold
    static float* goldAmbient();
    static float* goldDiffuse();
    static float* goldSpecular();
    static float goldShininess();
    // silver
    static float* silverAmbient();
    static float* silverDiffuse();
    static float* silverSpecular();
    static float silverShininess();
    // bronze
    static float* bronzeAmbient();
    static float* bronzeDiffuse();
    static float* bronzeSpecular();
    static float bronzeShininess();
};

#endif