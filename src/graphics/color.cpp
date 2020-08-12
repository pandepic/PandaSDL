#include "color.h"

PandaSDL::Color::Color()
: R(0.0f), G(0.0f), B(0.0f), A(0.0f)
{
}

PandaSDL::Color::Color(float r, float g, float b, float a)
{
    R = r;
    G = g;
    B = b;
    A = a;
}

PandaSDL::Color::Color(int r, int g, int b, int a)
{
    R = ((float)r)/255.0f;
    G = ((float)g)/255.0f;
    B = ((float)b)/255.0f;
    A = ((float)a)/255.0f;
}

PandaSDL::Color::~Color()
{
}

PandaSDL::Color PandaSDL::Color::FromHexRGB(int hex)
{
    float R = ((hex >> 16) & 0xFF) / 255.0f;
    float G = ((hex >> 8) & 0xFF) / 255.0f;
    float B = (hex & 0xFF) / 255.0f;

    return PandaSDL::Color(R, G, B, 1.0f);
}

PandaSDL::Color PandaSDL::Color::FromHexRGBA(int hex)
{
    float R = ((hex >> 24) & 0xFF) / 255.0f;
    float G = ((hex >> 16) & 0xFF) / 255.0f;
    float B = ((hex >> 8) & 0xFF) / 255.0f;
    float A = (hex & 0xFF) / 255.0f;

    return PandaSDL::Color(R, G, B, A);
}

glm::vec4 PandaSDL::Color::ToGLMVec4() const
{
    glm::vec4 colorVec4;
    colorVec4.x = R;
    colorVec4.y = G;
    colorVec4.z = B;
    colorVec4.w = A;

    return colorVec4;
}
