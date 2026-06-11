#ifndef __TEXTURE__
#define __TEXTURE__


#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Shader.h"

class Texture{
    private:
        GLuint ID;
        int _width, _height;
        int _channels;
        std::string _path;
        bool _loaded;

    public:
        Texture(const std::string& path);
        ~Texture();

        //void load (const std::string& path);
        void bind () const;
        //void setWrapping(GLenum mode);
        //void setFiltering(GLenum minFilter, GLenum magFilter);
        //void genMipmaps();
        //int getWidth() const;
        //int getHeight() const;
        //GLuint id() const;
};

#endif
