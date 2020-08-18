#include "framebuffer.h"

PandaSDL::Framebuffer::Framebuffer()
    : _id(0), _deleted(false), _game(nullptr), _target(nullptr), _depthBufferDeleted(false), _depthEnabled(false), _depthBufferID(0)
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
    _depthEnabled = false;PandaSDL::Game::GameInstance->GetDepthEnabled();

    glGenFramebuffers(1, &_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _target->GetTextureID(), 0);
    
    if (_depthEnabled)
    {
        glGenRenderbuffers(1, &_depthBufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, _depthBufferID); 
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _target->GetWidth(), _target->GetHeight());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        
        _depthBufferDeleted = false;
    }

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
    
    if (!_depthBufferDeleted && _depthEnabled)
    {
        glDeleteRenderbuffers(1, &_depthBufferID);
        _depthBufferDeleted = true;
    }

    glDeleteFramebuffers(1, &_id);
    _deleted = true;
}
