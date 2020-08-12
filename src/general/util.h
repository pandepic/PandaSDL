#ifndef __pandasdl_util_h__
#define __pandasdl_util_h__

#include <memory>
#include "../../lib_include/SDL2/SDL.h"

namespace PandaSDL
{
    static void SDL_DelRes(SDL_Window *r) { SDL_DestroyWindow(r); }
    static void SDL_DelRes(SDL_Renderer *r) { SDL_DestroyRenderer(r); }
    static void SDL_DelRes(SDL_Texture *r) { SDL_DestroyTexture(r); }
    static void SDL_DelRes(SDL_Surface *r) { SDL_FreeSurface(r); }

    template <typename T>
    std::shared_ptr<T> SDLSharedPtr(T *t)
    {
        return std::shared_ptr<T>(t, [](T *t) { SDL_DelRes(t); });
    }
} // namespace PandaSDL

#endif
