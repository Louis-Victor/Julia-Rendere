#ifndef __SHADER__
#define __SHADER__

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
    private:
        unsigned int ID;

    public:
        Shader(const char** shaderList, const unsigned int shaderCount);
        // use shader
        void use();
        // uniform utils
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setMat4f(const std::string& name, glm::mat4& value) const;
        void setVec2(const std::string& name, glm::vec2& value) const;
		void setDouble(const std::string& name, double value) const;
		void setDVec2(const std::string& name, glm::dvec2 value) const;
        // get shader
        unsigned int get();
};

#endif
