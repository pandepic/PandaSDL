#ifndef __pandasdl_framebuffer_h__
#define __pandasdl_framebuffer_h__

#include "../../lib_include/GLEW/glew.h"
#include <GL/gl.h>

#include "texture_2D.h"
#include "../general/game.h"
#include "shader.h"

namespace PandaSDL
{
    // forward declarations
    class Game;
    
    class FrameBuffer
    {
        public:
            FrameBuffer();
            ~FrameBuffer();

            void Start(PandaSDL::Game* game, std::shared_ptr<PandaSDL::Texture2D> target);
            void End();

        protected:
            bool _deleted;
            unsigned int _id;
            std::shared_ptr<PandaSDL::Texture2D> _target;
            PandaSDL::Game* _game;
            
            bool _depthEnabled;
            bool _depthBufferDeleted;
            unsigned int _depthBufferID;

            void Free();
    };
}

#endif
