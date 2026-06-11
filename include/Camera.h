#ifndef __CAMERA__
#define __CAMERA__

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class CameraModes{
    relative,
    relativeOrtho
};

class Camera{
    private:
        glm::vec3 _pos;
        glm::vec3 _target;
        glm::vec3 _up, _right, _front;
        glm::mat4 _viewMat;
        float _yaw, _pitch;
        bool _hasChanged;
        CameraModes _mode;

    public:
        // Target-free constructors
        Camera();
        Camera(const glm::vec3& pos);
        Camera(float x, float y, float z);

        // Target-specified constructors
        Camera(const glm::vec3& pos, const glm::vec3& target);
        Camera(float x, float y, float z, const glm::vec3& target);
        Camera(const glm::vec3& pos, float tx, float ty, float tz);
        Camera(float x, float y, float z, float tx, float ty, float tz);

        void setMode(CameraModes mode);


        glm::mat4& getViewMat();

        void move(const glm::vec3& shift);
        void move(float x, float y, float z);

        void rotate(float yaw, float pitch);

        const glm::vec3& getPos() const;

};
#endif
