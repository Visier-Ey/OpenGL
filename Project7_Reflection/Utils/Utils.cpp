#include "Utils.h"
#include <malloc.h>

Utils::Utils(/* args */)
{
}

Utils::~Utils()
{
}
float Utils::normalize(float value, float min, float max, float newMin, float newMax)
{
    return (value - min) / (max - min) * (newMax - newMin) + newMin;
}

string Utils::readShaderSource(const char *filePath)
{
    string content;
    ifstream fileStream(filePath, ios::in);
    string line = "";
    while (!fileStream.eof())
    {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

void Utils::printShaderLog(GLuint shader)
{
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = (char *)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        cout << "Shader Info Log: " << log << endl;
        free(log);
    }
}

void Utils::printProgramLog(int prog)
{
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = (char *)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cout << "Program Info Log: " << log << endl;
        free(log);
    }
}

bool Utils::checkOpenGLError()
{
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        cout << "glError: " << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

GLuint Utils::createShaderProgram(string vshaderStr, string fshaderStr)
{
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;
    vshaderStr = readShaderSource(vshaderStr.c_str());
    fshaderStr = readShaderSource(fshaderStr.c_str());
    const char *vshaderSource = vshaderStr.c_str();
    const char *fshaderSource = fshaderStr.c_str();
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);
    // ====================================Catch the error during shader compile========================================
    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1)
    {
        cout << "vertex compilation failed" << endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1)
    {
        cout << "fragment compilation failed" << endl;
        printShaderLog(fShader);
    }
    // =================================================================================================================
    // ====================================Catch the error during program link========================================
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1)
    {
        cout << "linking failed" << endl;
        printProgramLog(vfProgram);
    }
    // =================================================================================================================
    return vfProgram;
}

GLuint Utils::loadTexture(const char *textureFilePath)
{
    GLuint textureID;
    textureID = SOIL_load_OGL_texture(textureFilePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    if (textureID == 0)
    {
        cout << "SOIL loading error: " << SOIL_last_result() << endl;
    }
    return textureID;
}
// TODO : add the Ambient, Diffuse, Specular, Shininess for materials
// gold
float* Utils::goldAmbient()
{
    static float ambient[] = {0.24725f, 0.1995f, 0.0745f, 1.0f};
    return (float *)ambient;
}
float* Utils::goldDiffuse()
{
    static float diffuse[] = {0.75164f, 0.60648f, 0.22648f, 1.0f};
    return (float *)diffuse;
}
float* Utils::goldSpecular()
{
    static float specular[] = {0.628281f, 0.555802f, 0.366065f, 1.0f};
    return (float *)specular;
}
float Utils::goldShininess()
{
    static float shininess = 51.2f;
    return shininess;
}
// silver
float* Utils::silverAmbient()
{
    static float ambient[] = {0.19225f, 0.19225f, 0.19225f, 1.0f};
    return (float *)ambient;
}
float* Utils::silverDiffuse()
{
    static float diffuse[] = {0.50754f, 0.50754f, 0.50754f, 1.0f};
    return (float *)diffuse;
}
float* Utils::silverSpecular()
{
    static float specular[] = {0.508273f, 0.508273f, 0.508273f, 1.0f};
    return (float *)specular;
}
float Utils::silverShininess()
{
    static float shininess = 51.2f;
    return shininess;
}
// bronze
float* Utils::bronzeAmbient()
{
    static float ambient[] = {0.2125f, 0.1275f, 0.054f, 1.0f};
    return (float *)ambient;
}
float* Utils::bronzeDiffuse()
{
    static float diffuse[] = {0.714f, 0.4284f, 0.18144f, 1.0f};
    return (float *)diffuse;
}
float* Utils::bronzeSpecular()
{
    static float specular[] = {0.393548f, 0.271906f, 0.166721f, 1.0f};
    return (float *)specular;
}
float Utils::bronzeShininess()
{
    static float shininess = 25.6f;
    return shininess;
}