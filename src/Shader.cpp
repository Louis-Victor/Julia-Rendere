#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include  "glHelper.h"
#include <Shader.h>
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char** shaderList, const unsigned int shaderCount){
    unsigned int shaders[shaderCount];
    for(unsigned int i=0;i<shaderCount;i++){
        shaders[i] = compileShaderFromSource(shaderList[i]);
    }

    ID = glCreateProgram();
    for(unsigned int i=0;i<shaderCount;i++){
        glAttachShader(ID,shaders[i]);
    }
    glLinkProgram(ID);

    // Check for errors
    int success;
    char infoLog[512];
    glGetProgramiv(ID,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(ID,512,NULL,infoLog);
        std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
    }

    for(unsigned int i=0;i<shaderCount;i++){
        glDeleteShader(shaders[i]);
        std::cout << "Shader " << shaderList[i] << " done\n";
    }
}

void Shader::use(){
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4f(const std::string& name, glm::mat4& value) const{
    glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()),1,GL_FALSE,glm::value_ptr(value));
}
void Shader::setVec2(const std::string& name, glm::vec2& value) const{
    glUniform2fv(glGetUniformLocation(ID,name.c_str()),1,glm::value_ptr(value));
}

void Shader::setDouble(const std::string& name, double value) const {
    glUniform1d(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setDVec2(const std::string& name, glm::dvec2 value) const {
    glUniform2dv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}


unsigned int Shader::get(){
    return ID;
}
