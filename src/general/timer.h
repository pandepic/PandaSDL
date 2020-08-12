#ifndef __pandasdl_timer_h__
#define __pandasdl_timer_h__

#include <iostream>
#include "../../lib_include/SDL2/SDL.h"

namespace PandaSDL
{
    class Timer
    {
        public:
            Timer();
            ~Timer();

            void Start();
            void Tick();
            unsigned int GetTicks() const;
            double GetDelta() const;

        protected:
            unsigned int _prevTicks = 0;
    };
}

#endif
