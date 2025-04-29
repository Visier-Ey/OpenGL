#include <GL/glew.h>
#include <GLFW/glfw3.h> // 假设你还使用了GLFW来创建窗口和OpenGL上下文
#include <iostream>
#include <fstream>
#include <string>
using  namespace std;


#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];
float x = 0.0f;
float inc = 0.01f;
double curTime = 0.0f;
float rotateX = 0.0f;
float rotateY = 0.0f;
float rotateZ = 0.0f;
float scale = 1.0f;
float trans = 0.0f;


string readShaderSource(const char* filePath){
    string content;
    ifstream fileStream(filePath,ios::in);
    string line = "";
    while (!fileStream.eof()){
        getline(fileStream,line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

void printShaderLog(GLuint shader){
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&len);
    if (len > 0){
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader,len,&chWrittn,log);
        cout << "Shader Info Log: " << log << endl;
        free(log);
    }
}

void printProgramLog(int prog){
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(prog,GL_INFO_LOG_LENGTH,&len);
    if (len > 0){
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog,len,&chWrittn,log);
        cout << "Program Info Log: " << log << endl;
        free(log);
    }
}

bool checkOpenGLError(){
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR){
        cout << "glError: " << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

GLuint createShaderProgram(){
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;
    string vshaderStr = readShaderSource("GLSL/vS2.5.glsl");
    string fshaderStr = readShaderSource("GLSL/fS2.5.glsl");
    const char* vshaderSource = vshaderStr.c_str();
    const char* fshaderSource = fshaderStr.c_str();
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vShader,1,&vshaderSource,NULL);
    glShaderSource(fShader,1,&fshaderSource,NULL);
    // ====================================Catch the error during shader compile========================================
    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader,GL_COMPILE_STATUS,&vertCompiled);
    if ( vertCompiled != 1){
        cout << "vertex compilation failed" << endl;
        printShaderLog(vShader);
    }
    
    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader,GL_COMPILE_STATUS,&fragCompiled);
    if (fragCompiled != 1){
        cout << "fragment compilation failed" << endl;
        printShaderLog(fShader);
    }
    // =================================================================================================================
    // ====================================Catch the error during program link========================================
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram,vShader);
    glAttachShader(vfProgram,fShader);
    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram,GL_LINK_STATUS,&linked);
    if (linked != 1){
        cout << "linking failed" << endl;
        printProgramLog(vfProgram);
    }
    // =================================================================================================================
    return vfProgram;
}

void init(GLFWwindow* window){
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs,vao);
    glBindVertexArray(vao[0]);
}
void display(GLFWwindow* window,double currentTime){
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.1, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);
    // if (x > 1.0f || x < -1.0f){
    //     inc = -inc;
    // }
    // if (currentTime - curTime > 0.001f){
    //     curTime = currentTime;
    //     x+=inc;
    // }
    GLuint offsetLoc = glGetUniformLocation(renderingProgram,"offset");
    glProgramUniform1f(renderingProgram,offsetLoc,x);
    GLuint rotateLoc = glGetUniformLocation(renderingProgram,"rotateXAngle");
    glProgramUniform1f(renderingProgram,rotateLoc,rotateX);
    rotateX += 0.01f;
    rotateLoc = glGetUniformLocation(renderingProgram,"rotateYAngle");
    glProgramUniform1f(renderingProgram,rotateLoc,rotateY);
    rotateY += 0.01f;
    rotateLoc = glGetUniformLocation(renderingProgram,"rotateZAngle");
    glProgramUniform1f(renderingProgram,rotateLoc,rotateZ);
    rotateZ += 0.01f;
    GLuint scaleLoc = glGetUniformLocation(renderingProgram,"scaleX");
    glProgramUniform1f(renderingProgram,scaleLoc,scale);
    scaleLoc = glGetUniformLocation(renderingProgram,"scaleY");
    glProgramUniform1f(renderingProgram,scaleLoc,scale);
    scaleLoc = glGetUniformLocation(renderingProgram,"scaleZ");
    glProgramUniform1f(renderingProgram,scaleLoc,scale);
    GLuint transLoc = glGetUniformLocation(renderingProgram,"translateX");
    glProgramUniform1f(renderingProgram,transLoc,trans);
    transLoc = glGetUniformLocation(renderingProgram,"translateY");
    glProgramUniform1f(renderingProgram,transLoc,trans);
    transLoc = glGetUniformLocation(renderingProgram,"translateZ");
    glProgramUniform1f(renderingProgram,transLoc,trans);
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    // line mode
    glDrawArrays(GL_TRIANGLES,0,3);
}

int main(){
    if (!glfwInit()){exit(1);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow* window = glfwCreateWindow(600,600,"Project2.2",NULL,NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) exit(1);
    glfwSwapInterval(1);
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

