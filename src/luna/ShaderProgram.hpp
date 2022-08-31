#if !defined(LUNA_SHADER_PROGRAM_HPP)
#define LUNA_SHADER_PROGRAM_HPP

#include <iostream>
#include <string>

#include "mini/miniGL.h"
#include "Shader.hpp"
#include "LunaDefines.hpp"

namespace luna {
    class ShaderProgram {
    public:
        ShaderProgram(Shader& vertex_shader, Shader& fragment_shader);
        ~ShaderProgram();
        void bind();
        
        GLuint program_id;
    };
}

#endif // LUNA_SHADER_PROGRAM_HPP
