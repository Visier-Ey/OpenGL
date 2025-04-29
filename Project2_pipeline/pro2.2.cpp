#include <GL/glew.h>
#include <GLFW/glfw3.h> // 假设你还使用了GLFW来创建窗口和OpenGL上下文
#include <iostream>
using  namespace std;


#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];
float scale=0.0f;
float inc = 0.01f;
GLuint createShaderProgram(){
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
    glCompileShader(vShader);
    glCompileShader(fShader);
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram,vShader);
    glAttachShader(vfProgram,fShader);
    glLinkProgram(vfProgram);

    return vfProgram;
}

void init(GLFWwindow* window){
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs,vao);
    glBindVertexArray(vao[0]);
}
void display(GLFWwindow* window,double currentTime){
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);
    if (scale > 1.0f) inc = -0.01f;
    if (scale <= 0.0f) inc = 0.01f;
    scale += inc;
    // printf("scale = %f\n",scale);
    // printf("inc = %f\n",inc);
    glPointSize(scale*100.0f);
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

