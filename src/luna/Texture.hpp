#if !defined(TEXTURE_HPP)
#define TEXTURE_HPP

#include <iostream>
#include <cstdint>
#include <vector>

#include "mini/miniGL.h"
#include "stb_image.h"
#include "luna/LunaDefines.hpp"

namespace luna {
    class Texture {
    public:
        Texture(const uint32_t units = 1);
        ~Texture();

        void bind(const uint32_t texture_unit = 0);
        void unbind(const uint32_t texture_unit = 0);

        void set_unit_from_file(const char *filename, int32_t width, int32_t height, const uint32_t texture_unit = 0, const GLint internalformat = GL_RGBA, const GLenum format = GL_RGBA, const GLenum type = GL_UNSIGNED_BYTE, int32_t channels = 0);
        void set_unit_from_buffer(void* buffer, int32_t width, int32_t height, const uint32_t texture_unit = 0, const GLint internalformat = GL_RGBA, const GLenum format = GL_RGBA, const GLenum type = GL_UNSIGNED_BYTE, int32_t channels = 0);
        void set_filters(const GLint minify_filter = GL_NEAREST_MIPMAP_LINEAR, const GLint magnify_filter = GL_LINEAR, const uint32_t texture_unit = 0);
        void set_wrapping(const GLint wrap_s = GL_REPEAT, const GLint wrap_t = GL_REPEAT, const GLint wrap_r = GL_REPEAT, const uint32_t texture_unit = 0);

    private:
        GLuint texture_id;
        std::vector<GLenum> texture_type;
        uint32_t total_units;
    };
} // luna

#endif // TEXTURE_HPP
