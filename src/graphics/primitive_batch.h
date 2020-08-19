#ifndef __pandasdl_spritebatch_h__
#define __pandasdl_spritebatch_h__

#include "../../lib_include/GLEW/glew.h"
#include <GL/gl.h>

#include "../../lib_include/glm/glm.hpp"
#include "../../lib_include/glm/gtc/type_ptr.hpp"

#include "../general/globals.h"
#include "../math/rectangle.h"
#include "../math/vector2.h"
#include "shader.h"
#include "color.h"

namespace PandaSDL
{
    class PrimitiveBatch
    {
        public:
            PrimitiveBatch();
            ~PrimitiveBatch();
            
        protected:
    };
}

#endif