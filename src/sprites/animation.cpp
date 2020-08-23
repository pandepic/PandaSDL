#include "animation.h"

PandaSDL::Animation::Animation()
    : EndFrame(PANDASDL_ANIMATION_NOENDFRAME), Flip(PandaSDL::eSpriteFlip::NONE)
{
}

PandaSDL::Animation::~Animation()
{
}

void PandaSDL::Animation::FrameSetRange(int min, int max)
{
    if (max < min)
        return;
    
    Frames.clear();
    FrameAddRange(min, max);
}

void PandaSDL::Animation::FrameAddRange(int min, int max)
{
    for (int i = min; i <= max; i++)
    {
        Frames.push_back(i);
    }
}