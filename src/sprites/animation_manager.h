#ifndef __pandasdl_animationmanager_h__
#define __pandasdl_animationmanager_h__

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <fstream>

#include "../../lib_include/pugixml/pugixml.hpp"

#include "../general/globals.h"
#include "../general/extensions.h"
#include "animation.h"

namespace PandaSDL
{
    class AnimationManager
    {
        public:
            AnimationManager();
            ~AnimationManager();

            void LoadAnimations(std::string animationsFile);
            std::shared_ptr<PandaSDL::Animation> GetAnimation(std::string name);

        protected:
            std::map<std::string, std::shared_ptr<PandaSDL::Animation>> _animations;
    };
}

#endif