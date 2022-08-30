#if !defined(TEXT2_HPP)
#define TEXT2_HPP

#include <cstdint>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>

#include "ft2build.h"
#include FT_FREETYPE_H
#include "glm/glm.hpp"

#include "stb_image_write.h"

#include "luna/Luna.hpp"

class FontAtlas {
public:
    FontAtlas(std::string filename);
    ~FontAtlas();

    inline FT_Glyph_Metrics* get_metric(uint8_t character_code);
    inline uint32_t get_texture_left(uint8_t character_code);
    inline uint32_t get_width();
    inline uint32_t get_height();
    void set_texture(luna::Texture& atlas, uint32_t unit = 0);
    void write_png(std::string filename);
private:
    void load_metrics(FT_Face face);

    uint32_t width = 0;
    uint32_t height = 0;

    uint32_t texture_left[128 - 32];
    FT_Glyph_Metrics metrics[128 - 32];
    uint8_t* buffer;
};

class Text2 {
public:
    Text2(std::string _contents, FontAtlas& _font);

    void scale_to(double font_size);
    
    std::vector<float> vertices;
    std::vector<float> texture_coordinates;
    std::vector<uint32_t> indices;

private:
    void generate_data();
    void generate_indices();

    //std::vector<glm::vec2> character_positions; // Transform
    std::string contents;

    FontAtlas *font = NULL;
};

/*
class FontFace: public luna::Texture {
public:
    FontFace(const char *filename);
    virtual ~FontFace();

    float character_to_vertices(uint8_t c, float *position_vertices, float *texture_vertices, uint32_t position_start, uint32_t texture_start, glm::vec2 origin);

private:
    std::map<uint8_t, uint32_t> glyph_index;
    glm::ivec4 texture_rect[128];
    FT_Glyph_Metrics metrics[128];
    Texture *atlas;
};

class Text2: public luna::VertexArray {
public:
    Text2(std::string body, FontFace *font);
    virtual ~Text2();

    void set_text(std::string body);
    void apply_scaling(double scaling);

private:
    uint32_t current_body_size;
    FontFace *font_; // might become a vector
};
*/
#endif // TEXT2_HPP
