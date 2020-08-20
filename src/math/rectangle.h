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
            
            bool Contains(const PandaSDL::Vector2 &vec) const;
            bool Contains(const PandaSDL::Rectangle &rect) const;
            bool Intersects(const PandaSDL::Rectangle &rect) const;
            
            // operators
            PandaSDL::Rectangle operator+(const PandaSDL::Rectangle &r);
            PandaSDL::Rectangle operator-(const PandaSDL::Rectangle &r);
            PandaSDL::Rectangle operator*(const PandaSDL::Rectangle &r);
            PandaSDL::Rectangle operator/(const PandaSDL::Rectangle &r);
            
            PandaSDL::Rectangle operator+(const PandaSDL::Vector2 &v);
            PandaSDL::Rectangle operator-(const PandaSDL::Vector2 &v);
            PandaSDL::Rectangle operator*(const PandaSDL::Vector2 &v);
            PandaSDL::Rectangle operator/(const PandaSDL::Vector2 &v);

            static PandaSDL::Rectangle Empty();
            
            static PandaSDL::Rectangle Intersects(const PandaSDL::Rectangle &rect1, const PandaSDL::Rectangle &rect2);

        protected:
    };
}

#endif
