#include "texture_2D.h"

PandaSDL::Texture2D::Texture2D(unsigned int format, unsigned int filter, unsigned int wrap)
    : _deleted(false), _wrapS(wrap), _wrapT(wrap), _imageFormat(format), _internalFormat(format), _filterMin(filter), _filterMax(filter)
{
    glGenTextures(1, &_id);
}

PandaSDL::Texture2D::~Texture2D()
{
    Free();
}

void PandaSDL::Texture2D::Create(unsigned int width, unsigned int height, unsigned char *data, std::string name)
{
    _width = width;
    _height = height;
    _name = name;

    _texelWidth = 1.0f / (float)_width;
    _texelHeight = 1.0f / (float)_height;

    Bind();

        glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height, 0, _imageFormat, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filterMax);

    Unbind();
}

void PandaSDL::Texture2D::Create(unsigned int width, unsigned int height)
{
    _width = width;
    _height = height;

    _texelWidth = 1.0f / (float)_width;
    _texelHeight = 1.0f / (float)_height;

    Bind();

        glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height, 0, _imageFormat, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filterMax);

    Unbind();
}

void PandaSDL::Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, _id);
}

void PandaSDL::Texture2D::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void PandaSDL::Texture2D::Free()
{
    if (_deleted)
        return;

    glDeleteTextures(1, &_id);
    _deleted = true;
}

unsigned int PandaSDL::Texture2D::GetTextureID()
{
    return _id;
}

unsigned int PandaSDL::Texture2D::GetWidth()
{
    return _width;
}

unsigned int PandaSDL::Texture2D::GetHeight()
{
    return _height;
}

float PandaSDL::Texture2D::GetTexelWidth()
{
    return _texelWidth;
}

float PandaSDL::Texture2D::GetTexelHeight()
{
    return _texelHeight;
}

std::string PandaSDL::Texture2D::GetName()
{
    return _name;
}
