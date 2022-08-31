#if !defined(LUNA_SHADER_HPP)
#define LUNA_SHADER_HPP

#include <iostream>

#include "mini/miniGL.h"
#include "LunaDefines.hpp"

namespace luna {
    class Shader {
    public:
        Shader(const GLenum type, const char* source, const SourceText source_type = SourceText::file);
        ~Shader();
        
        char *read_file(const char *filepath);
        GLuint create_shader(const GLenum type, const char *source);

        GLuint shader_id;
    };
}

#endif // LUNA_SHADER_HPP
