#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include  "glHelper.h"
#include "Texture.h"
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "stb_image.h"

Texture::Texture(const std::string& path){
    unsigned char* data = stbi_load(path.c_str(), &_width, &_height, &_channels, 0);
    if(!data){
        std::cout << "Failed to load texture\n";
        _loaded = false;
    }else{
        _loaded = true;
        GLenum format = (_channels == 4) ? GL_RGBA : GL_RGB;

        glGenTextures(1,&ID);

        glBindTexture(GL_TEXTURE_2D,ID);

        glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &ID);
}

void Texture::bind() const{
    glBindTexture(GL_TEXTURE_2D, ID);
}
