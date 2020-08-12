#include "framebuffer.h"

PandaSDL::Framebuffer::Framebuffer()
    : _deleted(false)
{
}

PandaSDL::Framebuffer::~Framebuffer()
{
    Free();
}

void PandaSDL::Framebuffer::Start(PandaSDL::Game* game, std::shared_ptr<PandaSDL::Texture2D> target)
{
    _target = target;
    _game = game;

    glGenFramebuffers(1, &_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _target->GetTextureID(), 0);

    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::FRAMEBUFFER_START_FAILED);
    
    glViewport(0, 0, _target->GetWidth(), _target->GetHeight());
}

void PandaSDL::Framebuffer::End()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    _game->ResetViewport();
    Free();
}

void PandaSDL::Framebuffer::Free()
{
    if (_deleted)
        return;

    glDeleteFramebuffers(1, &_id);
    _deleted = true;
}
