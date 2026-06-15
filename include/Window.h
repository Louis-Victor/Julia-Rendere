#ifndef __WINDOW__
#define __WINDOW__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

// Window class
class Window{
    private:
        GLFWwindow* window;
        int width, height;
        const char* label;
        bool _close;
		int posX, posY; 
		static void windowPosCallback(GLFWwindow* window, int xpos, int ypos);

    public:
        Window();
        Window(int p_width, int p_height, const char* p_label);
        // New constructor: share context with another window
        Window(int p_width, int p_height, const char* p_label, GLFWwindow* share);
        ~Window();
        void render();
        bool isClosed();
        GLFWwindow* getWindow();
        int getWidth();
        int getHeight();
        const char* getLabel();
        void close();
        int getInput(int key) const;
        void clear();
        void clear(float r, float g, float b);
        void clear(float r, float g, float b, float alpha);
        // New method: make this window's OpenGL context current
        void makeCurrent();
		void getPosition(int& x, int& y) const;
		int getPosX() const { return posX; }
		int getPosY() const { return posY; }
		glm::dvec2 getCursorPosition() const;

};

#endif
