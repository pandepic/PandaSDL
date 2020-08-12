#ifndef __pandasdl_point_h__
#define __pandasdl_point_h__

#include <string>

namespace PandaSDL
{
    struct Vector2
    {
        public:
            Vector2();
            Vector2(double x, double y);
            Vector2(int x, int y);
            Vector2(unsigned int x, unsigned int y);
            Vector2(float f);
            ~Vector2();

            double X, Y;

            bool IsZero();
            std::string ToString();
            
            // operators
            PandaSDL::Vector2 operator+(const PandaSDL::Vector2 &v);
            PandaSDL::Vector2 operator-(const PandaSDL::Vector2 &v);
            PandaSDL::Vector2 operator*(const PandaSDL::Vector2 &v);
            PandaSDL::Vector2 operator/(const PandaSDL::Vector2 &v);
            
            PandaSDL::Vector2 operator*(const int &i);
            PandaSDL::Vector2 operator*(const float &f);
            PandaSDL::Vector2 operator*(const double &d);
            
            PandaSDL::Vector2& operator+=(const PandaSDL::Vector2 &v);
            PandaSDL::Vector2& operator-=(const PandaSDL::Vector2 &v);
            PandaSDL::Vector2& operator*=(const PandaSDL::Vector2 &v);
            PandaSDL::Vector2& operator/=(const PandaSDL::Vector2 &v);

            static Vector2 Zero();

        protected:
    };
}

#endif
