#ifndef __pandasdl_shader_h__
#define __pandasdl_shader_h__

#include "../../lib_include/GLEW/glew.h"
#include <GL/gl.h>

#include "../../lib_include/glm/glm.hpp"
#include "../../lib_include/glm/gtc/type_ptr.hpp"

namespace PandaSDL
{
    class Shader
    {
        public:
            Shader();
            ~Shader();

            unsigned int ID; 
            
            Shader &Use();

            void Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr); // note: geometry source code is optional 

            void SetFloat (const char *name, float value, bool useShader = false);
            void SetInteger (const char *name, int value, bool useShader = false);
            void SetVector2f (const char *name, float x, float y, bool useShader = false);
            void SetVector2f (const char *name, const glm::vec2 &value, bool useShader = false);
            void SetVector3f (const char *name, float x, float y, float z, bool useShader = false);
            void SetVector3f (const char *name, const glm::vec3 &value, bool useShader = false);
            void SetVector4f (const char *name, float x, float y, float z, float w, bool useShader = false);
            void SetVector4f (const char *name, const glm::vec4 &value, bool useShader = false);
            void SetMatrix4 (const char *name, const glm::mat4 &matrix, bool useShader = false);

        protected:
            void CheckCompileErrors(unsigned int object, std::string type); 
    };
}

#endif
