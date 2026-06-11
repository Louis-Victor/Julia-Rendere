#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include "stb_image.h"

// GLFW & GLEW include
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include </usr/include/GLFW/glfw3.h>
//#include <GLES2/gl2.h>
//#include <EGL/egl.h>
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "glHelper.h"
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Controller.h"
#include "Texture.h"


using namespace std;
const float screen = 1920.0f/1080.0f;


const unsigned int shaderCount = 2;
const char* juliaShaderList[shaderCount] = {
    "main.vert",
    "julia.frag"
};
const char* mandelbrotShaderList[shaderCount] = {
    "main.vert",
    "mandelbrot.frag"
};


int steps = 1;
int count = 0;
double zoom = 0.6f;
double zoomSpeed = 0.01f;
double zoomAcc = 0.01f;
glm::dvec2 shift(0.0,0.0);
float radius = 0.5f;
//glm::vec2 shift(-0.761574,-0.0847596);
//(-1.62917,-0.0203968);

bool start = false;
bool rotateAngle = true;
glm::dvec2 point(0.0f,0.0f);

int power = 2;

bool colour = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        //start = !start;
		//rotateAngle = !rotateAngle;
		zoom *= 2;
		std::cout << "Zoom " << zoom << "\n";
    }
	if(key == GLFW_KEY_ENTER && action == GLFW_PRESS){
		steps *= 2;
	}
	if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS && steps > 1){
		steps /= 2;
	}
    if(key == GLFW_KEY_UP && action == GLFW_PRESS){
		shift += 0.6*glm::dvec2(0.0f,0.5f)/zoom;
		std::cout << "Shift " << glm::to_string(shift) << "\n";
        //steps *= 2;
    }
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		shift += 0.6*glm::dvec2(0.0f,-0.5f)/zoom;
		std::cout << "Shift " << glm::to_string(shift) << "\n";
        //steps /= 2;
    }
    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
		shift += 0.6*glm::dvec2(0.5f,0.0f)/zoom;
		std::cout << "Shift " << glm::to_string(shift) << "\n";
        //radius += 0.1;
    }
    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
		shift += 0.6*glm::dvec2(-0.5f,0.0f)/zoom;
		std::cout << "Shift " << glm::to_string(shift) << "\n";
        //radius -= 0.1;
		//radius = std::max(0.0f,radius);
    }
	if(key ==  GLFW_KEY_W && action == GLFW_PRESS){
		point += 0.6*glm::dvec2(0.0f,0.05f)/zoom;
		std::cout << glm::to_string(point) << "\n";
	}
	if(key ==  GLFW_KEY_S && action == GLFW_PRESS){
		point += 0.6*glm::dvec2(0.0f,-0.05f)/zoom;
		std::cout << glm::to_string(point) << "\n";
	}
	if(key ==  GLFW_KEY_A && action == GLFW_PRESS){
		point += 0.6*glm::dvec2(-0.05f,0.0f)/zoom;
		std::cout << glm::to_string(point) << "\n";
	}
	if(key ==  GLFW_KEY_D && action == GLFW_PRESS){
		point += 0.6*glm::dvec2(0.05f,0.0f)/zoom;
		std::cout << glm::to_string(point) << "\n";
	}
	if(key == GLFW_KEY_R && action == GLFW_PRESS){
		zoom = 0.6;
		zoomSpeed = 0.01f;
		zoomAcc = 0.01f;
	}
	if(key == GLFW_KEY_E && action == GLFW_PRESS){
		shift = glm::dvec2(0.0,0.0);
	}
	if(key == GLFW_KEY_T && action == GLFW_PRESS){
		point = glm::dvec2(0.0,0.0);
	}
	if(key > 49 && key < 58&& action == GLFW_PRESS){
		power = key - 48;
	}
	if(key == GLFW_KEY_TAB && action == GLFW_PRESS){
		colour = !colour;
	}
}



int main(int argc, char** argv){
    _width = 1920/4;
    _height = 1080/4;

    // Starts glfw
    if(setUpGL()){
        cout << "INIT SUCCESSFUL\n"; 
    }else{
        cout << "FAILED INIT\n"; 
        return 1;
    }

    // Create Window object
    // 1920x1080
	Window juliaWindow((float)_width, (float)_height, "Julia Window");
    if(juliaWindow.getWindow() == NULL){
        glfwTerminate();
        return 1;
    }
    juliaWindow.makeCurrent();   // ensure its context is current before GLEW init
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW\n";
        return 1;
    }

    // Create second window (Mandelbrot) – SHARE context with Julia window
    Window mandelbrotWindow((float)_width, (float)_height, "Mandelbrot Window", juliaWindow.getWindow());
    if(mandelbrotWindow.getWindow() == NULL){
        glfwTerminate();
        return 1;
    }

    // Set key callbacks (no change)
    glfwSetKeyCallback(juliaWindow.getWindow(), key_callback);
    glfwSetKeyCallback(mandelbrotWindow.getWindow(), key_callback);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;


    // Set up quad
    float vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f,
        -1.0f, -1.0f
    };
    // Create buffer
    unsigned int triangleVBO;
    glGenBuffers(1,&triangleVBO);

    // Create Vertex Array Object (VAO)
    unsigned int triangleVAO;
    glGenVertexArrays(1,&triangleVAO);

    // Copy data to buffer
    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER,triangleVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    // Triangle Shader
    Shader juliaShader(juliaShaderList, shaderCount);
    Shader mandelbrotShader(mandelbrotShaderList, shaderCount);

    // Link data buffer -> shader
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	unsigned int juliaVAO;
	juliaWindow.makeCurrent();
	glGenVertexArrays(1, &juliaVAO);
	glBindVertexArray(juliaVAO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);   // VBO IS shared, this works
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	float angle = 0;
	//float deltaTime = 0;
	//float lastTime = glfwGetTime();

    /* Render loop */
    // Check if window has closed
    while(!juliaWindow.isClosed()){
		//deltaTime = glfwGetTime() - lastTime;
		//lastTime = glfwGetTime();

        // input processing
        if(juliaWindow.getInput(GLFW_KEY_ESCAPE) == GLFW_PRESS || mandelbrotWindow.getInput(GLFW_KEY_ESCAPE) == GLFW_PRESS){
            juliaWindow.close();
            cout << "Closed\n";
            break;
        }
        

		// DRAW JULIA
		juliaWindow.makeCurrent();
		
		glBindVertexArray(juliaVAO);
		//glBindVertexArray(triangleVAO);

        juliaWindow.clear(0.2f,0.3f,0.3f);
        juliaShader.use();

        juliaShader.use();
        juliaShader.setDouble("width",(double)_width);
        juliaShader.setDouble("height",(double)_height);
        juliaShader.setInt("steps", steps);
        juliaShader.setDouble("zoom",zoom);
        juliaShader.setDVec2("shift",shift);
        juliaShader.setFloat("angle",angle);
        juliaShader.setFloat("radius",radius);
		juliaShader.setDVec2("constPoint",point);
		juliaShader.setInt("power",power);
		juliaShader.setBool("colour",colour);

		glDrawArrays(GL_TRIANGLES, 0, 6);

        juliaWindow.render();


		// MANDELBROT
		mandelbrotWindow.makeCurrent();

		glBindVertexArray(triangleVAO);

        mandelbrotWindow.clear(0.2f,0.3f,0.3f);
        mandelbrotShader.use();

        mandelbrotShader.use();
        mandelbrotShader.setDouble("width",(double)_width);
        mandelbrotShader.setDouble("height",(double)_height);
        mandelbrotShader.setInt("steps", steps);
        mandelbrotShader.setDouble("zoom",zoom);
        mandelbrotShader.setDVec2("shift",shift);
        mandelbrotShader.setFloat("angle",angle);
        mandelbrotShader.setFloat("radius",radius);
		mandelbrotShader.setDVec2("constPoint",point);
		mandelbrotShader.setInt("power",power);
		mandelbrotShader.setBool("colour",colour);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        if(start){
            zoom += zoomSpeed;
            zoomSpeed += zoomAcc;
            zoomAcc *= 1.05f;
        }
        mandelbrotWindow.render();

    }

    // Clean glfw resources
    cout << "Closing GLFW\n";
    glfwTerminate();
    cout << "GLFW Closed\n";

    return 0;
}
