#include "Texture.hpp"

using namespace luna;

Texture::Texture(const uint32_t units)
    :   total_units(units) {
    glGenTextures(1, &texture_id);
    for (int i = 0; i < total_units; ++i) {
        texture_type.push_back(GL_TEXTURE_2D);
    }
}

Texture::~Texture() {
}

void Texture::bind(const uint32_t texture_unit) {
    glBindTexture(texture_type[texture_unit], texture_id);
}

void Texture::unbind(const uint32_t texture_unit) {
    glBindTexture(texture_type[texture_unit], 0);
}

void Texture::set_unit_from_file(const char *filename, int32_t width, int32_t height, const uint32_t texture_unit, const GLint internalformat, const GLenum format, const GLenum type, int32_t channels) {  
    if (texture_unit < total_units) {
        glActiveTexture(texture_unit + GL_TEXTURE0);
        bind(texture_unit);
        // stbi_uc *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
        // channels_in_file: Force the number of channels in the file. 0 will auto detect.
        stbi_uc *image = stbi_load(filename, &width, &height, &channels, 0);
        if (image == NULL) {
            printf(LUNA_FILE_ERROR "Failed to load image in the location: %s\n", filename);
        }
        /* void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * data);
         * level:          Level-of-detail, specifying the 'n'th mipmap reduction.
         * internalformat: View documentation for all avaliable types. Base types {DEPTH_COMPONENT, DEPTH_STENCIL, RED, RG, RGB, RGBA} 
         * border:         Must be 0.
         * format:         ({RED, RG, RGB, BGR, RGBA} - []_INTEGER) or {STENCIL_INDEX, DEPTH_COMPONENT, DEPTH_STENCIL}
         * type:           View documentation for avaliable types. Typical types (UNSIGNED_[] - {BYTE, SHORT, INT}) or {HALF_FLOAT, FLOAT}
         * data:           A pointer to the data.
         */
        glTexImage2D(texture_type[texture_unit], 0, internalformat, width, height, 0, format, type, image);
        glGenerateMipmap(texture_type[texture_unit]);
        stbi_image_free(image);
        unbind(texture_unit);
    }
    // TODO: When total units are exceeded
}

void Texture::set_unit_from_buffer(void* buffer, int32_t width, int32_t height, const uint32_t texture_unit, const GLint internalformat, const GLenum format, const GLenum type, int32_t channels) {  
    if (texture_unit < total_units) {
        glActiveTexture(texture_unit + GL_TEXTURE0);
        bind(texture_unit);
        glTexImage2D(texture_type[texture_unit], 0, internalformat, width, height, 0, format, type, buffer);
        glGenerateMipmap(texture_type[texture_unit]);
        unbind(texture_unit);
    }
    // TODO: When total units are exceeded
}

void Texture::set_filters(const GLint minify_filter, const GLint magnify_filter, const uint32_t texture_unit) {
    /* GL_NEAREST:           Value/Texture element nearest to the specified coordinate.
     * GL_LINEAR:            Weighted average of four closest values/texture elements.
     * GL_[]_MIPMAP_NEAREST: Chooses the mipmap that most closely matches the size of the pixel.
     * GL_[]_MIPMAP_LINEAR:  Chooses two mipmaps that most closely matches the size of the pixel. Final value is the weighted average of both values.
     * MAG_FILTER is unable to produce mipmaps. Default: LINEAR
     * MIN_FILTER has a total of six options.   Default: GL_NEAREST_MIPMAP_LINEAR
     * In terms of performance, the order is:
     * {NEAREST, LINEAR} - {[]_MIPMAP_NEAREST, []_MIPMAP_LINEAR}
     */
    if (texture_unit < total_units) {
        bind(texture_unit);
        glTexParameteri(texture_type[texture_unit], GL_TEXTURE_MAG_FILTER, magnify_filter);
        glTexParameteri(texture_type[texture_unit], GL_TEXTURE_MIN_FILTER, minify_filter);
        glGenerateMipmap(texture_type[texture_unit]);
        unbind(texture_unit);
    }
    // TODO: When total units are exceeded
}

void Texture::set_wrapping(const GLint wrap_s, const GLint wrap_t, const GLint wrap_r, const uint32_t texture_unit) {
    /* Default: GL_REPEAT
     * {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, or GL_MIRROR_CLAMP_TO_EDGE}
     */
    if (texture_unit < total_units) {
        bind(texture_unit);
        glTexParameteri(texture_type[texture_unit], GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(texture_type[texture_unit], GL_TEXTURE_WRAP_T, wrap_t);
        glTexParameteri(texture_type[texture_unit], GL_TEXTURE_WRAP_R, wrap_r);
        unbind(texture_unit);
    }
    // TODO: When total units are exceeded
}