#include "glHelper.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

int _width = 400;
int _height = 400;

float fov = 45.0f;
float aspect = (float)_width/(float)_height;
float near = 0.1f;
float far = 100.0f;


const char* shaderDir = "shaders/";

float xoffset = 0.0f;
float yoffset = 0.0f;

// mouse positions
double lastX = 960.0f;
double lastY = 540.0f;

// first camera movement
bool firstMove = true;

// Window resize function
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    std::cout << "(" << width << "," << height << ")\n";
    glViewport(0,0,width,height);
    _width = width;
    _height = height;
    aspect = (float)_width/(float)_height;
}

bool setUpGL(){
    bool succ = glfwInit();

    if(succ){
        // Major and minor opengl versions
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);

        // Sets up opengl profile
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    }

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return succ;
}

std::string importShader(const char* filename){
    std::string fullPath = std::string(shaderDir) + std::string(filename);

    std::fstream shaderSource(fullPath.c_str());
    if(!shaderSource){
        std::cout << "Failed to open " << fullPath << "\n";
        exit(1);
    }

    std::string source;
    char buffer[256];
    while(shaderSource.getline(buffer,256)){
        source += buffer;
        source += "\n";
    }
    shaderSource.close();


    return source;
}

unsigned int compileShader(const char* sourceCode, GLenum shaderType){
    unsigned int shader;
    shader = glCreateShader(shaderType);
    glShaderSource(shader,1,&sourceCode,NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(shader,512,NULL,infoLog);
        std::cout << "error::shader::compilation_failed\n" << infoLog << "\n";

        exit(1);
    }

    return shader;
}

/*
 * GL_COMPUTE_SHADER -> .comp
 * GL_VERTEX_SHADER -> .vert
 * GL_TESS_CONTROL_SHADER -> .tesc
 * GL_TESS_EVALUATION_SHADER -> .tese
 * GL_GEOMETRY_SHADER -> .geom
 * GL_FRAGMENT_SHADER -> .frag
 */
unsigned int compileShaderFromSource(const char* filename){
    std::string sfl = filename;
    sfl = sfl.substr(sfl.length()-4,4);

    GLenum shaderType;
    if(sfl == "comp") shaderType = GL_COMPUTE_SHADER;
    else if(sfl == "vert") shaderType = GL_VERTEX_SHADER;
    else if(sfl == "tesc") shaderType = GL_TESS_CONTROL_SHADER;
    else if(sfl == "tese") shaderType = GL_TESS_EVALUATION_SHADER;
    else if(sfl == "geom") shaderType = GL_GEOMETRY_SHADER;
    else if(sfl == "frag") shaderType = GL_FRAGMENT_SHADER;
    else{
        std::cout << "Invalid file extensnsion, use any of the following\n\t.comp\n\t.vert\n\t.tesc\n\t.tese\n\t.geom\n\t.frag\n";
        exit(1);
    }

    unsigned int shader = compileShader(importShader(filename).c_str(),shaderType);

    return shader;
}

bool createShaderProgram(unsigned int& program, const unsigned int* shaders, const unsigned int shaderCount){
    program = glCreateProgram();
    for(unsigned int i=0;i<shaderCount;i++){
        glAttachShader(program,shaders[i]);
    }
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(program,512,NULL,infoLog);
        std::cout << "error::shader::program::linking_failed\n" << infoLog << "\n";
        return false;
    }
    return true;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMove){
        lastX = xpos;
        lastY = ypos;

        firstMove = false;
    }
    xoffset = xpos - lastX;
    yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (abs(xoffset) < 0.001f) xoffset = 0;
    if (abs(yoffset) < 0.001f) yoffset = 0;
}

