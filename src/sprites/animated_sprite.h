#ifndef __pandasdl_animatedsprite_h__
#define __pandasdl_animatedsprite_h__

#include <iostream>
#include <memory>

#include "sprite.h"
#include "animation.h"

#define PANDASDL_LOOPFOREVER -1

namespace PandaSDL
{
    class AnimatedSprite: public Sprite
    {
        public:
            AnimatedSprite();
            ~AnimatedSprite();

            void Init(std::shared_ptr<PandaSDL::Texture2D> texture, int frameWidth, int frameHeight, int defaultFrame = 1);
            void PlayAnimation(std::shared_ptr<PandaSDL::Animation> animation, int loopCount = PANDASDL_LOOPFOREVER);
            void StopAnimation();
            void SetFrame(int frame);

            virtual void Draw(std::shared_ptr<PandaSDL::Spritebatch> spriteBatch, PandaSDL::Vector2 position) override;
            virtual void Update(const PandaSDL::Timer& gameTimer) override;

            int FrameWidth;
            int FrameHeight;
            int TotalFrames;

            std::shared_ptr<PandaSDL::Animation> CurrentAnimation;
            int CurrentFrame;
            int CurrentFrameIndex;
        
        protected:
            double _currentFrameTime;
            double _timePerFrame;
            int _animationLoopCount;

            PandaSDL::eSpriteFlip _baseFlip;
    };
}

#endif