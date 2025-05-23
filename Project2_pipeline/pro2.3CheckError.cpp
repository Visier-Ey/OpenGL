#include <GL/glew.h>
#include <GLFW/glfw3.h> // 假设你还使用了GLFW来创建窗口和OpenGL上下文
#include <iostream>
using  namespace std;


#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];

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
    
    const char* vshaderSource = "#version 430\n"
    "void main(void)\n"
    "{gl_Position = vec4(0.0,0.0,0.0,1.0);}";
    const char* fshaderSource = "#version 430\n"
    "out vec4 color;\n"
    "void main(void)\n"
    "{ if(gl_FragCoord.x < 300) color = vec4(1.0,0.0,0.0,1.0);"
    "else color = vec4(1.0,0.0,1.0,1.0);}";
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
    glUseProgram(renderingProgram);
    glPointSize(30.0f);
    glDrawArrays(GL_POINTS,0,1);
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

