#ifndef __pandasdl_animation_h__
#define __pandasdl_animation_h__

#include "../general/globals.h"

#define PANDASDL_ANIMATION_NOENDFRAME -1

namespace PandaSDL
{
    class Animation
    {
        public:
            Animation();
            ~Animation();

            void FrameSetRange(int min, int max);
            void FrameAddRange(int min, int max);

            double Duration;
            std::string Name;
            std::vector<int> Frames;
            int EndFrame;
            PandaSDL::eSpriteFlip Flip;

        protected:
    };
}

#endif