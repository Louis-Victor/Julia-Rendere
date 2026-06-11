#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include  "glHelper.h"
#include "Camera.h"
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Camera::Camera(const glm::vec3& pos, const glm::vec3& target){
    _pos = pos;
    _target = target;

    _yaw = -90.0f;
    _pitch = 0.0f;

    glm::vec3 up(0.0f, 1.0f, 0.0f);
    _front = glm::normalize(_target-_pos);
    _right = glm::normalize(glm::cross(up,_front));
    _up = glm::cross(_front,_right);
    _hasChanged = true;
    
    _mode = CameraModes::relativeOrtho;
}

Camera::Camera(float x, float y, float z, const glm::vec3& target):Camera(glm::vec3(x,y,z),target){};

Camera::Camera(const glm::vec3& pos, float tx, float ty, float tz):Camera(pos,glm::vec3(tx,ty,tz)){};

Camera::Camera(float x, float y, float z, float tx, float ty, float tz):Camera(glm::vec3(x,y,z),glm::vec3(tx,ty,tz)){};

Camera::Camera(const glm::vec3& pos):Camera(pos, 0.0f, 0.0f, 0.0f){};

Camera::Camera(float x, float y, float z):Camera(glm::vec3(x,y,z)){};

Camera::Camera():Camera(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f){};

void Camera::setMode(CameraModes mode){
    _mode = mode;
}

glm::mat4& Camera::getViewMat(){
    if(_hasChanged){
        _hasChanged = false;
        _viewMat = glm::lookAt(_pos,_pos+_front,_up);
    }
    return _viewMat;
};

void Camera::move(float x, float y, float z){
    _hasChanged = true;
    if(_mode == CameraModes::relative){
        _pos += x*_right + y*_up + z*_front;
    }else if(_mode == CameraModes::relativeOrtho){
        // worldUp
        glm::vec3 wUp(0.0f,1.0f,0.0f);
        _pos += x*glm::normalize(_right - glm::dot(_right,wUp)*wUp) 
                + y*wUp
                + z*glm::normalize(_front - glm::dot(_front, wUp)*wUp);
    }
}
void Camera::move(const glm::vec3& shift){
    move(shift.x,shift.y,shift.z);
}

void Camera::rotate(float yaw, float pitch) {
    _yaw += yaw;
    _pitch += pitch;
    if(_pitch < -89) _pitch = -89;
    else if(_pitch > 89) _pitch = 89;
    float yawRad = glm::radians(_yaw);
    float pitchRad = glm::radians(_pitch);

    _front.x = cos(yawRad) * cos(pitchRad);
    _front.y = sin(pitchRad);
    _front.z = sin(yawRad) * cos(pitchRad);
    _front = glm::normalize(_front);

    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    _right = glm::normalize(glm::cross(worldUp, _front));
    _up = glm::cross(_front, _right);

    _hasChanged = true;
}

const glm::vec3& Camera::getPos() const{
    return _pos;
}

