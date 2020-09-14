#include "animated_sprite.h"

PandaSDL::AnimatedSprite::AnimatedSprite()
    : FrameWidth(0), FrameHeight(0), TotalFrames(0), CurrentFrame(1), CurrentFrameIndex(0), _currentFrameTime(0.0), _timePerFrame(0.0), _animationLoopCount(0)
{
}

PandaSDL::AnimatedSprite::~AnimatedSprite()
{
}

void PandaSDL::AnimatedSprite::Init(std::shared_ptr<PandaSDL::Texture2D> texture, int frameWidth, int frameHeight, int defaultFrame)
{
    PandaSDL::Sprite::LoadTexture(texture);

    FrameWidth = frameWidth;
    FrameHeight = frameHeight;
    
    if (frameWidth == -1 && frameHeight == -1)
    {
        FrameWidth = texture->GetWidth();
        FrameHeight = texture->GetHeight();
    }

    Origin = PandaSDL::Vector2::Zero();

    TotalFrames = (Texture->GetWidth() / FrameWidth) * (Texture->GetHeight() / FrameHeight);

    SetFrame(defaultFrame);
}

void PandaSDL::AnimatedSprite::PlayAnimation(std::shared_ptr<PandaSDL::Animation> animation, int loopCount)
{
    if (animation == nullptr)
        return;

    if (animation->Frames.size() <= 0)
        return;

    CurrentAnimation = animation;

    CurrentFrameIndex = 0;
    _currentFrameTime = 0.0f;
    _timePerFrame = animation->Duration / (double)animation->Frames.size();

    _animationLoopCount = loopCount;

    if (_animationLoopCount != PANDASDL_LOOPFOREVER)
        _animationLoopCount -= 1;

    SetFrame((int)CurrentAnimation->Frames[CurrentFrameIndex]);

    _baseFlip = Flip;
    if (animation->Flip != PandaSDL::eSpriteFlip::NONE)
        Flip = animation->Flip;
}

void PandaSDL::AnimatedSprite::StopAnimation()
{
    if (CurrentAnimation->EndFrame != PANDASDL_ANIMATION_NOENDFRAME)
        SetFrame(CurrentAnimation->EndFrame);

    CurrentAnimation = nullptr;
    Flip = _baseFlip;
}

void PandaSDL::AnimatedSprite::SetFrame(int frame)
{
    if (frame < 1 || frame > TotalFrames)
        return;

    SourceRect.X = ((frame - 1) % (Texture->GetWidth() / FrameWidth)) * FrameWidth;
    SourceRect.Y = ((frame - 1) / (Texture->GetWidth() / FrameWidth)) * FrameHeight;
    SourceRect.Width = FrameWidth;
    SourceRect.Height = FrameHeight;

    CurrentFrame = frame;
}

void PandaSDL::AnimatedSprite::Draw(std::shared_ptr<PandaSDL::SpriteBatch> spriteBatch, PandaSDL::Vector2 position)
{
    PandaSDL::Sprite::Draw(spriteBatch, position);
}

void PandaSDL::AnimatedSprite::Update(const PandaSDL::Timer &gameTimer)
{
    PandaSDL::Sprite::Update(gameTimer);

    if (CurrentAnimation == nullptr)
        return;
    
    _currentFrameTime += gameTimer.GetTicks();

    if (_currentFrameTime >= _timePerFrame)
    {
        _currentFrameTime -= _timePerFrame;
        CurrentFrameIndex++;

        if (CurrentFrameIndex >= CurrentAnimation->Frames.size())
        {
            if (_animationLoopCount == PANDASDL_LOOPFOREVER || _animationLoopCount > 0)
            {
                CurrentFrameIndex = 0;

                if (_animationLoopCount != PANDASDL_LOOPFOREVER)
                    _animationLoopCount--;

                SetFrame((int)CurrentAnimation->Frames[CurrentFrameIndex]);
            }
            else if (_animationLoopCount <= 0)
            {
                StopAnimation();
            }
        }
        else
        {
            SetFrame((int)CurrentAnimation->Frames[CurrentFrameIndex]);
        }
    }
} // Update
