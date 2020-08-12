#ifndef __pandasdl_vector3_h__
#define __pandasdl_vector3_h__

#include <string>

namespace PandaSDL
{
    struct Vector3
    {
        public:
            Vector3();
            Vector3(double x, double y, double z);
            Vector3(int x, int y, int z);
            Vector3(unsigned int x, unsigned int y, unsigned int z);
            Vector3(float f);
            ~Vector3();

            double X, Y, Z;
            
            bool IsZero();
            std::string ToString();
            
            // operators
            PandaSDL::Vector3 operator+(const PandaSDL::Vector3 &v);
            PandaSDL::Vector3 operator-(const PandaSDL::Vector3 &v);
            PandaSDL::Vector3 operator*(const PandaSDL::Vector3 &v);
            PandaSDL::Vector3 operator/(const PandaSDL::Vector3 &v);
            
            PandaSDL::Vector3 operator*(const int &i);
            PandaSDL::Vector3 operator*(const float &f);
            PandaSDL::Vector3 operator*(const double &d);
            
            PandaSDL::Vector3& operator+=(const PandaSDL::Vector3 &v);
            PandaSDL::Vector3& operator-=(const PandaSDL::Vector3 &v);
            PandaSDL::Vector3& operator*=(const PandaSDL::Vector3 &v);
            PandaSDL::Vector3& operator/=(const PandaSDL::Vector3 &v);

            static Vector3 Zero();

        protected:
    };
}

#endif
