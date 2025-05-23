#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
    static string readShaderSource(const char* filePath);
    static void printShaderLog(GLuint shader);
    static void printProgramLog(int prog);
    static bool checkOpenGLError();
    static GLuint createShaderProgram(string vshaderStr, string fshaderStr);
};

#endif