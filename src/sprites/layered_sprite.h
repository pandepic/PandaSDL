#ifndef __pandasdl_layeredsprite_h__
#define __pandasdl_layeredsprite_h__

#include "../general/game.h"
#include "../graphics/frame_buffer.h"
#include "../graphics/sprite_batch.h"
#include "animated_sprite.h"

namespace PandaSDL
{
    class LayeredSprite: public AnimatedSprite
    {
        public:
            LayeredSprite();
            ~LayeredSprite();

            void Create(std::vector<std::string> layerAssets, int frameWidth, int frameHeight, int defaultFrame = 1);

        protected:
            std::vector<std::string> _layerAssets;
    };
}

#endif
