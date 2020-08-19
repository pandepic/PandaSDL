#ifndef __pandasdl_primitivebatchtypes_h__
#define __pandasdl_primitivebatchtypes_h__

#include "../math/vector2.h"

namespace PandaSDL
{
    struct Primitive2D
    {
        std::vector<PandaSDL::Vector2> Vertices;
        std::vector<unsigned int> Indices;
    };
    
    struct RectanglePrimitive2D : public Primitive2D
    {
        RectanglePrimitive2D(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    };
}

#endif