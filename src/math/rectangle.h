#ifndef __pandasdl_rectangle_h__
#define __pandasdl_rectangle_h__

#include "vector2.h"

namespace PandaSDL
{
    struct Rectangle
    {
        public:
            Rectangle();
            Rectangle(int x, int y, int width, int height);
            ~Rectangle();

            int X, Y, Width, Height;

            bool IsEmpty();
            std::string ToString();
            
            // operators
            PandaSDL::Rectangle operator+(const PandaSDL::Rectangle &r);
            PandaSDL::Rectangle operator-(const PandaSDL::Rectangle &r);
            PandaSDL::Rectangle operator*(const PandaSDL::Rectangle &r);
            PandaSDL::Rectangle operator/(const PandaSDL::Rectangle &r);
            
            PandaSDL::Rectangle operator+(const PandaSDL::Vector2 &v);
            PandaSDL::Rectangle operator-(const PandaSDL::Vector2 &v);
            PandaSDL::Rectangle operator*(const PandaSDL::Vector2 &v);
            PandaSDL::Rectangle operator/(const PandaSDL::Vector2 &v);

            static Rectangle Empty();

        protected:
    };
}

#endif
