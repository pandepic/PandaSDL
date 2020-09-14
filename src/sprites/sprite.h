#ifndef __pandasdl_sprite_h__
#define __pandasdl_sprite_h__

#include "../general/globals.h"
#include "../assets/asset_manager.h"
#include "../graphics/texture_2D.h"
#include "../graphics/sprite_batch.h"
#include "../graphics/color.h"
#include "../math/rectangle.h"
#include "../math/vector2.h"
#include "../general/timer.h"

namespace PandaSDL
{
    class Sprite
    {
        public:
            Sprite();
            ~Sprite();

            int GetWidth();
            int GetHeight();

            void LoadTexture(std::shared_ptr<PandaSDL::Texture2D> texture);

            virtual void Draw(std::shared_ptr<PandaSDL::SpriteBatch> spriteBatch, PandaSDL::Vector2 position);
            virtual void Update(const PandaSDL::Timer &gameTimer);

            std::shared_ptr<PandaSDL::Texture2D> Texture;

            bool Visible;
            float Scale;
            float Rotation;
            PandaSDL::eSpriteFlip Flip;
            PandaSDL::Color Colour;
            PandaSDL::Vector2 Origin;
            PandaSDL::Rectangle SourceRect;

        protected:
    };
}

#endif