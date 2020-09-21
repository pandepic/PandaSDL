#ifndef __pandasdl_puiwimagebox_h__
#define __pandasdl_puiwimagebox_h__

#include "../puiwidget.h"
#include "../../graphics/texture_2D.h"

namespace PandaSDL
{
    class PUIWImageBox: public PUIWidget
    {
        public:
            PUIWImageBox();
            ~PUIWImageBox();
            
        protected:
            std::shared_ptr<Texture2D> _texture;
    };
}

#endif
