#include "primitive_batch_types.h"

PandaSDL::RectanglePrimitive2D::RectanglePrimitive2D(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    PandaSDL::Vector2 vertexTemplate[] = {
        { 0, 0 }, // top left
        { 0, 0 }, // top right
        { 0, 0 },
        { 0, 0 },
    };
}
