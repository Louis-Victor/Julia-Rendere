#include "Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glHelper.h"
#include <iostream>
#include <glm/glm.hpp>


Window::Window(): width(800),height(600),label("Window"){}

Window::Window(int p_width, int p_height, const char* p_label): width(p_width), height(p_height), label(p_label){
    _close = false;
    window = glfwCreateWindow(width,height,label,NULL,NULL);
    if(window == NULL){
        std::cout << "Failed to create " << label << "\n";
    }else{
        std::cout << label << " created successfully\n";
        glfwMakeContextCurrent(window);
        glViewport(0,0,width,height);
        glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowPosCallback(window, windowPosCallback);
	glfwGetWindowPos(window, &posX, &posY);
}

// New constructor with context sharing
Window::Window(int p_width, int p_height, const char* p_label, GLFWwindow* share)
    : width(p_width), height(p_height), label(p_label), _close(false) {
    window = glfwCreateWindow(width, height, label, NULL, share);
    if(window == NULL){
        std::cout << "Failed to create " << label << "\n";
    }else{
        std::cout << label << " created successfully (shared context)\n";
        glfwMakeContextCurrent(window);
        glViewport(0,0,width,height);
        glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowPosCallback(window, windowPosCallback);
	glfwGetWindowPos(window, &posX, &posY);
}

Window::~Window(){
    this->close();
}

void Window::render(){
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Window::isClosed(){
    return glfwWindowShouldClose(window);
}

GLFWwindow* Window::getWindow(){
    return window;
}

int Window::getWidth(){
    return width;
}

int Window::getHeight(){
    return height;
}

const char* Window::getLabel(){
    return label;
}

void Window::close(){
    if(_close) return;  // added missing semicolon
    glfwSetWindowShouldClose(window,true);
    std::cout << "Window " << label << " " << this << " closed\n";
    _close = true;
}

int Window::getInput(int key) const{
    return glfwGetKey(window,key);
}

void Window::clear(){
    clear(0.0f,0.0f,0.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::clear(float r, float g, float b){
    clear(r,g,b,1.0f);
}

void Window::clear(float r, float g, float b, float alpha){
    glClearColor(r,g,b,alpha);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// New method: switch to this window's context
void Window::makeCurrent(){
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width,height);
}


void Window::windowPosCallback(GLFWwindow* window, int xpos, int ypos) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->posX = xpos;
        win->posY = ypos;
    }
}

void Window::getPosition(int& x, int& y) const {
    x = posX;
    y = posY;
}

glm::dvec2 Window::getCursorPosition() const {
	double x, y;
    glfwGetCursorPos(window, &x, &y);
    return glm::dvec2(x, y);

}
