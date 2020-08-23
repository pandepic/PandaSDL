#ifndef __pandasdl_point_h__
#define __pandasdl_point_h__

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
            
            double Distance(const PandaSDL::Vector2 &vec) const;
            
            // operators
            PandaSDL::Vector2 operator+(const PandaSDL::Vector2 &vec);
            PandaSDL::Vector2 operator-(const PandaSDL::Vector2 &vec);
            PandaSDL::Vector2 operator*(const PandaSDL::Vector2 &vec);
            PandaSDL::Vector2 operator/(const PandaSDL::Vector2 &vec);
            
            PandaSDL::Vector2 operator*(const int &i);
            PandaSDL::Vector2 operator*(const float &f);
            PandaSDL::Vector2 operator*(const double &d);
            
            PandaSDL::Vector2 operator/(const int &i);
            PandaSDL::Vector2 operator/(const float &f);
            PandaSDL::Vector2 operator/(const double &d);
            
            PandaSDL::Vector2& operator+=(const PandaSDL::Vector2 &vec);
            PandaSDL::Vector2& operator-=(const PandaSDL::Vector2 &vec);
            PandaSDL::Vector2& operator*=(const PandaSDL::Vector2 &vec);
            PandaSDL::Vector2& operator/=(const PandaSDL::Vector2 &vec);

            static PandaSDL::Vector2 Zero();
            static double Distance(const PandaSDL::Vector2 &vec1, const PandaSDL::Vector2 &vec2);

        protected:
    };
}

#endif
