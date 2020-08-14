#ifndef __pandasdl_boxbatch_h__
#define __pandasdl_boxbatch_h__

/*
	Batch render 3D textured boxes
*/

#include <iostream>
#include <vector>
#include <memory>
#include <map>

#include "../../lib_include/GLEW/glew.h"
#include <GL/gl.h>

#include "../../lib_include/glm/glm.hpp"
#include "../../lib_include/glm/gtc/type_ptr.hpp"

#include "../general/globals.h"
#include "../math/vector3.h"
#include "../math/rectangle.h"
#include "shader.h"
#include "color.h"
#include "texture_2D.h"

#define PANDASDL_BOX_SIDES 6

namespace PandaSDL
{
	enum class eBoxSide
	{
		TOP,
		BOTTOM,
		FRONT,
		BACK,
		LEFT,
		RIGHT
	};
	
	struct BoxBatchItem
    {
        std::shared_ptr<PandaSDL::Texture2D> Texture[PANDASDL_BOX_SIDES];
        //Rectangle SourceRect;
        Vector3 Position;
        Color Color;
        Vector3 Scale;
        Vector2 Rotation;
    };
	
	class Boxbatch
	{
		public:
			Boxbatch();
			~Boxbatch();
			
			static std::shared_ptr<Shader> DefaultBoxShader;
			
            static std::string DefaultBoxShaderVertexCode;
            static std::string DefaultBoxShaderFragmentCode;
			
		protected:
	};
}

#endif
