#ifndef __CONTROLLER__
#define __CONTROLLER__

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

enum class Actions {
    forward,
    backward,
    left,
    right,
    yawR,
    yawL,
    pitchU,
    pitchD,
    run
};
extern std::unordered_map<int, Actions> keyAction;

class Controller{
    private:
        Camera* _camera;
        float _speed, _sensitivity;
        bool _invertY;
        
    public:
        Controller();
        Controller(Camera* camera);
        void setCamera(Camera* camera);
        bool setSpeed(float speed);
        bool setSensitivity(float sensitivity);
        bool setInvertY(bool invertY);
        bool toggleInvY();
        void processInput(const Window& window);
};
#endif
