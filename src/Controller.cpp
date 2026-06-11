#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include  "glHelper.h"
#include "Window.h"
#include "Camera.h"
#include "Controller.h"
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float runVal = 2.0f;

std::unordered_map<int, Actions> keyAction ={
    {'W', Actions::forward},
    {'A', Actions::left},
    {'S', Actions::backward},
    {'D', Actions::right},
    {GLFW_KEY_RIGHT,Actions::yawR},
    {GLFW_KEY_LEFT,Actions::yawL},
    {GLFW_KEY_UP,Actions::pitchU},
    {GLFW_KEY_DOWN,Actions::pitchD},
    {GLFW_KEY_LEFT_SHIFT,Actions::run}
};


Controller::Controller(){
    _speed = 0.1f;
    _sensitivity = 1.0f;
    _invertY = false;
}

Controller::Controller(Camera* camera):Controller(){
    _camera = camera;
}

void Controller::setCamera(Camera* camera){
    _camera = camera;
}

bool Controller::setSpeed(float speed){
    if(speed <= 0) return false;
    _speed = speed;
    return true;
}
bool Controller::setSensitivity(float sensitivity){
    if(sensitivity <= 0) return false;
    _sensitivity = sensitivity;
    return true;
}

bool Controller::setInvertY(bool invertY){
    _invertY = invertY;
    return _invertY;
}
bool Controller::toggleInvY(){
    _invertY = !_invertY;
    return _invertY;
}

void Controller::processInput(const Window& window){
    for(const auto& [key, action] : keyAction){
        if(window.getInput(key) == GLFW_PRESS && key != GLFW_KEY_LEFT_SHIFT){
            switch(action){
                case Actions::forward:
                    _camera->move(_speed*glm::vec3(0.0f,0.0f,1.0f) * (window.getInput(GLFW_KEY_LEFT_SHIFT)== GLFW_PRESS? runVal:1.0f));
                    break;
                case Actions::backward:
                    _camera->move(_speed*glm::vec3(0.0f,0.0f,-1.0f) * (window.getInput(GLFW_KEY_LEFT_SHIFT)== GLFW_PRESS? runVal:1.0f));
                    break;
                case Actions::right:
                    _camera->move(_speed*glm::vec3(-1.0f,0.0f,0.0f) * (window.getInput(GLFW_KEY_LEFT_SHIFT)== GLFW_PRESS? runVal:1.0f));
                    break;
                case Actions::left:
                    _camera->move(_speed*glm::vec3(1.0f,0.0f,0.0f) * (window.getInput(GLFW_KEY_LEFT_SHIFT)== GLFW_PRESS? runVal:1.0f));
                    break;
                case Actions::yawR:
                    _camera->rotate(_sensitivity * (window.getInput(GLFW_KEY_LEFT_SHIFT)== GLFW_PRESS? runVal:1.0f),0);
                    break;
                case Actions::yawL:
                    _camera->rotate(-_sensitivity * (window.getInput(GLFW_KEY_LEFT_SHIFT)== GLFW_PRESS? runVal:1.0f),0);
                    break;
                case Actions::pitchU:
                    _camera->rotate(0,_sensitivity * (window.getInput(GLFW_KEY_LEFT_SHIFT)== GLFW_PRESS? runVal:1.0f) * (_invertY ? -1 : 1));
                    break;
                case Actions::pitchD:
                    _camera->rotate(0,-_sensitivity * (window.getInput(GLFW_KEY_LEFT_SHIFT)== GLFW_PRESS? runVal:1.0f) * (_invertY ? -1 : 1));
                    break;
                case Actions::run:
                    std::cout << "Error, run not supposed to enter switch\n";
                    break;
            }
        } 
    }
}
