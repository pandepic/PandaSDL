#ifndef __pandasdl_color_h__
#define __pandasdl_color_h__

#define PANDASDL_COLOR_TRANSPARENT PandaSDL::Color(0, 0, 0, 0)
#define PANDASDL_COLOR_WHITE PandaSDL::Color(255, 255, 255, 255)
#define PANDASDL_COLOR_BLACK PandaSDL::Color(0, 0, 0, 255)
#define PANDASDL_COLOR_RED PandaSDL::Color(255, 0, 0, 255)
#define PANDASDL_COLOR_GREEN PandaSDL::Color(0, 255, 0, 255)
#define PANDASDL_COLOR_BLUE PandaSDL::Color(0, 0, 255, 255)
#define PANDASDL_COLOR_CORNFLOWERBLUE PandaSDL::Color::FromHexRGBA(0x6495edff)
#define PANDASDL_COLOR_PURPLE PandaSDL::Color::FromHexRGBA(0x800080ff)
#define PANDASDL_COLOR_YELLOW PandaSDL::Color::FromHexRGBA(0xffff00ff)
#define PANDASDL_COLOR_PINK PandaSDL::Color::FromHexRGBA(0xffc0cbff)
#define PANDASDL_COLOR_ORANGE PandaSDL::Color::FromHexRGBA(0xffa500ff)
#define PANDASDL_COLOR_GREY PandaSDL::Color::FromHexRGBA(0x808080ff)
#define PANDASDL_COLOR_GRAY PandaSDL::Color::FromHexRGBA(0x808080ff)

#include "../../lib_include/glm/glm.hpp"
#include "../../lib_include/glm/gtc/type_ptr.hpp"

namespace PandaSDL
{
    struct Color
    {
        public:
            Color();
            Color(float r, float g, float b, float a);
            Color(int r, int g, int b, int a);
            ~Color();

            glm::vec4 ToGLMVec4() const;

            static Color FromHexRGB(int hex);
            static Color FromHexRGBA(int hex);

            float R, G, B, A;

        protected:
    };
}

#endif
