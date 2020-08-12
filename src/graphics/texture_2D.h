#ifndef __pandasdl_texture2d_h__
#define __pandasdl_texture2d_h__

#include <iostream>

#include "../../lib_include/GLEW/glew.h"
#include <GL/gl.h>

#include "../../lib_include/stb/stb_image.h"

// filter - wrap
// point clamp = GL_NEAREST and GL_CLAMP_TO_EDGE
// linear clamp = GL_LINEAR and GL_CLAMP_TO_EDGE
// point wrap = GL_NEAREST and GL_REPEAT
// linear wrap = GL_LINEAR and GL_REPEAT

namespace PandaSDL
{
    class Texture2D
    {
        public:
            Texture2D(unsigned int format = GL_RGBA, unsigned int filter = GL_NEAREST, unsigned int wrap = GL_CLAMP_TO_EDGE);
            ~Texture2D();

            void Create(unsigned int width, unsigned int height, unsigned char* data, std::string name = "");
            void Create(unsigned int width, unsigned int height);
            void Bind() const;
            void Unbind() const;
            void Free();

            unsigned int GetTextureID();
            unsigned int GetWidth();
            unsigned int GetHeight();
            float GetTexelWidth();
            float GetTexelHeight();
            std::string GetName();

        protected:
            bool _deleted;
            unsigned int _id;
            std::string _name;

            unsigned int _width, _height;
            float _texelWidth, _texelHeight;

            unsigned int _wrapS;
            unsigned int _wrapT;
            unsigned int _imageFormat;
            unsigned int _internalFormat;
            unsigned int _filterMin;
            unsigned int _filterMax;
    };
}

#endif
