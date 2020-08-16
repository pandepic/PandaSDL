#ifndef __dynamictexturemanager_h__
#define __dynamictexturemanager_h__

#include "../general/globals.h"
#include "../graphics/texture_2D.h"

namespace PandaSDL
{
    class DynamicTextureManager
    {
        public:
            DynamicTextureManager();
            ~DynamicTextureManager();

            std::shared_ptr<PandaSDL::Texture2D> CreateTexture2D(unsigned int width, unsigned int height);
            std::shared_ptr<PandaSDL::Texture2D> CreateTexture2D(unsigned int width, unsigned int height, unsigned int &id);
            std::shared_ptr<PandaSDL::Texture2D> GetTexture2D(unsigned int id);
            void FreeTexture2D(unsigned int id);
            void Clear();

        protected:
            unsigned int _texture2DIndexCounter;
            std::map<unsigned int, std::shared_ptr<PandaSDL::Texture2D>> _texture2DCache;
    };
}

#endif
