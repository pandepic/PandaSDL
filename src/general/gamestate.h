#ifndef __pandasdl_gamestate_h__
#define __pandasdl_gamestate_h__

#include "game.h"

namespace PandaSDL
{
    // forward declarations
    class Game;
    
    class Gamestate
    {
        public:
            Gamestate();
            virtual ~Gamestate();

            virtual void Setup(PandaSDL::Game *game);
            virtual void Close() = 0;
            
            virtual void Update(const PandaSDL::Timer &gameTimer) = 0;
            virtual void Draw(const PandaSDL::Timer &gameTimer) = 0;

        protected:
            PandaSDL::Game* _game;
    };
}

#endif
