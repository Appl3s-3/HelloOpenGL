#include "Text2.hpp"

FontAtlas::FontAtlas(std::string filename) {
    // TODO: Error handling
    // Create handles to a FT_Library and FT_Face
    
    FT_Library library;
    FT_Face face;

    int32_t library_error = FT_Init_FreeType(&library);
    if (library_error != 0) {
        std::cout << LUNA_FONT_ERROR "Failed to initialize FreeType Library." << std::endl;
    }
    
    // Load a face
    int32_t face_error = FT_New_Face(library, filename.data(), 0, &face);
    if (face_error != 0) {
        std::cout << LUNA_FONT_ERROR "Failed to load font." << std::endl;
    }

    // https://freetype.org/freetype2/docs/tutorial/step1.html#section-3

    int32_t set_error;
    /* handle to face object           */
    /* char_width in 1/64th of points  */
    /* char_height in 1/64th of points */
    /* horizontal device resolution    */
    /* vertical device resolution      */

    // Set size with 1 of 2 functions
    //set_error = FT_Set_Char_Size(face, 0, 16*64, 300, 300);

    set_error = FT_Set_Pixel_Sizes(face, 0, 48);
    if (set_error != 0) {
        std::cout << "Set Error" << std::endl;
    }

    // Load a glyph from the face (via character code) into the face's glyph slot. FT_Load_Glyph is called in this.
    load_metrics(face);

    buffer = new uint8_t[width * height];
    std::fill_n(buffer, width * height, 0);

    FT_Bitmap* bitmap_slot = &(face->glyph->bitmap);

    int load_error;
    uint32_t left = 0; // Pixels from the left of the atlas
    for (uint8_t c = 32; c < 128; ++c) {
        load_error = FT_Load_Char(face, c, FT_LOAD_RENDER);
        if (load_error != 0) {
            continue;
        }
        for (int32_t i = 0; i < bitmap_slot->rows; ++i) {
            std::memcpy(buffer + (i * width) + left, bitmap_slot->buffer + (i * bitmap_slot->width), bitmap_slot->width);
        }
        texture_left[c - 32] = left;
        left += bitmap_slot->width;
    }
}

FontAtlas::~FontAtlas() {
    delete buffer;
}

FT_Glyph_Metrics* FontAtlas::get_metric(uint8_t character_code) {
    return &metrics[character_code - 32];
}

uint32_t FontAtlas::get_texture_left(uint8_t character_code) {
    return texture_left[character_code - 32];
}

uint32_t FontAtlas::get_width() {
    return width;
}

uint32_t FontAtlas::get_height() {
    return height;
}

void FontAtlas::set_texture(luna::Texture& atlas, uint32_t unit) {
    atlas.bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    atlas.set_filters();
    atlas.set_wrapping();
    atlas.set_unit_from_buffer(buffer, width, height, unit);
}

void FontAtlas::write_png(std::string filename) {
	char* png_data = (char*)calloc(width * height * 4, 1);
	for(int i = 0; i < (width * height); ++i) {
		png_data[i * 4 + 0] |= buffer[i];
		png_data[i * 4 + 1] |= buffer[i];
		png_data[i * 4 + 2] |= buffer[i];
		png_data[i * 4 + 3] = 0xff;
	}

	stbi_write_png((filename + ".png").data(), width, height, 4, png_data, width * 4);

	free(png_data);
}

void FontAtlas::load_metrics(FT_Face face) {
    width = 0;
    height = 0;
    FT_GlyphSlot glyph_slot = face->glyph;
    FT_Bitmap* bitmap_slot = &(face->glyph->bitmap);

    int load_error;

    for (int32_t c = 32; c < 128; ++c) {
        load_error = FT_Load_Char(face, c, FT_LOAD_RENDER);
        if (load_error != 0) {
            std::cout << LUNA_FONT_ERROR "Failed to load Glyph [" << c << "]. Skipping." << std::endl;
            continue;
        }

        metrics[c - 32] = glyph_slot->metrics;
        height = std::max(height, bitmap_slot->rows);
        width += bitmap_slot->width;
    }
}

Text2::Text2(std::string _contents, FontAtlas& _font)
    :   contents(_contents), font(&_font) {
    generate_data();
    generate_indices();
}

void Text2::scale_to(double font_size) {
    // Get the scalar ratio between font height and first character y bearing
    double size_character_ratio = (double)font->get_height() / (double)font->get_metric(contents[0])->horiBearingY;

    // Multiply the first character y bearing by the ratio to get the current font size
    double current_font_size = vertices[1] * size_character_ratio;
    // Get the scalar ratio between the new font size and current font size
    double scale_ratio = font_size / current_font_size;

    // Multiply everything by the new ratio
    for (auto& vert : vertices) {
        vert *= scale_ratio;
    }
}

void Text2::generate_data() {
    FT_Glyph_Metrics* character_metric;
    uint32_t texture_left;

    FT_Vector advance = {0, 0};

    for (int32_t i = 0; i < contents.size(); ++i) {
        character_metric = font->get_metric(contents[i]);
        texture_left = font->get_texture_left(contents[i]);

        // Clockwise from top left
        vertices.push_back(advance.x + character_metric->horiBearingX);
        vertices.push_back(advance.y + character_metric->horiBearingY);

        vertices.push_back(advance.x + character_metric->horiBearingX + character_metric->width);
        vertices.push_back(advance.y + character_metric->horiBearingY);

        vertices.push_back(advance.x + character_metric->horiBearingX + character_metric->width);
        vertices.push_back(advance.y + character_metric->horiBearingY - character_metric->height);
        
        vertices.push_back(advance.x + character_metric->horiBearingX);
        vertices.push_back(advance.y + character_metric->horiBearingY - character_metric->height);

        advance.x += character_metric->horiAdvance;
        //advance.y += character_metric->vertAdvance;

        texture_coordinates.push_back((double)texture_left / (double)font->get_width());
        texture_coordinates.push_back(0.0);
        
        texture_coordinates.push_back(((double)texture_left + (double)character_metric->width) / (double)font->get_width());
        texture_coordinates.push_back(0.0);
        
        texture_coordinates.push_back(((double)texture_left + (double)character_metric->width) / (double)font->get_width());
        texture_coordinates.push_back((double)character_metric->height / (double)font->get_height());
        
        texture_coordinates.push_back((double)texture_left / (double)font->get_width());
        texture_coordinates.push_back((double)character_metric->height / (double)font->get_height());
    }
}

void Text2::generate_indices() {
    uint32_t total_quads = vertices.size() / 4;
    uint32_t offset = 0;
    for (int32_t i = 0; i < total_quads; ++i) {
        indices.push_back(offset + 0);
        indices.push_back(offset + 1);
        indices.push_back(offset + 2);

        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
        indices.push_back(offset + 0);
        offset += 4;
    }
}

/*

FontFace::FontFace(const char *filename) {
    // Load FreeType
    FT_Library ft;
    if (FT_Init_FreeType(&ft) != 0) {
        std::cout << LUNA_FONT_ERROR "Failed to initialize FreeType Library." << std::endl;
    }
    else {
        // Load font face
        FT_Face face;
        if (FT_New_Face(ft, filename, 0, &face)) {
            std::cout << LUNA_FONT_ERROR "Failed to load font." << std::endl;
        }
        else {
            // Set width and height. If width is 0, it is automatically set based on height.
            FT_Set_Pixel_Sizes(face, 0, 48);

            // Default 4, accepts 1, 2, 4, 8. Affects glTexImage2D
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

            // Store current texture_offset in an ivec2, and a pointer to the glyph's bitmap for easy access
            glm::ivec2 total_offset = glm::ivec2(0, 0);
            FT_Bitmap *glyph_bitmap = NULL;

            // Loop through every ASCII character and find metric data
            for (uint8_t c = 0; c < 128; ++c) {
                // load character glyph
                if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0) {
                    std::cout << LUNA_FONT_ERROR "Failed to load Glyph [" << c << "]. Skipping." << std::endl;
                    continue;
                }
                glyph_bitmap = &face->glyph->bitmap;
                //glyph_index[c] = c;
                texture_rect[c] = glm::ivec4(total_offset.x, total_offset.y, total_offset.x + glyph_bitmap->width, total_offset.y + glyph_bitmap->rows);
                metrics[c] = face->glyph->metrics;
                total_offset.x += glyph_bitmap->width;
                total_offset.y += glyph_bitmap->rows;

                //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }

            // Create an atlas_buffer
            glm::ivec2 atlas_size = glm::ivec2(total_offset.x, total_offset.y);
            uint8_t *atlas_buffer = new uint8_t[atlas_size.x * atlas_size.y];

            // Copy data into the atlas_buffer
            uint32_t glyph_offset = 0;
            uint32_t glyph_size = 0;
            for (uint8_t c = 0; c < 128; ++c) {
                // load character glyph
                if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0) {
                    continue;
                }
                glyph_offset = texture_rect[c].x * texture_rect[c].y;
                glyph_size = face->glyph->bitmap.rows * face->glyph->bitmap.width;
                std::memcpy(atlas_buffer + glyph_offset, face->glyph->bitmap.buffer, glyph_size);
            }

            // Free resources from FreeType
            FT_Done_Face(face);
            FT_Done_FreeType(ft);

            // Create the atlas (to be changed to inheritance)
            atlas = new Texture(1);
            atlas->bind();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_size.x, atlas_size.y, 0, GL_RED, GL_UNSIGNED_BYTE, atlas_buffer);
            atlas->unbind();
        }
    }
}

FontFace::~FontFace() {

}

float FontFace::character_to_vertices(uint8_t c, float *position_vertices, float *texture_vertices, uint32_t position_start, uint32_t texture_start, glm::vec2 origin) {
    // For now assume that text is horizontal
    glm::vec4 tl_br = glm::vec4(origin.x + metrics[c].horiBearingX,
                                origin.y + metrics[c].horiBearingY,
                                origin.x + metrics[c].horiBearingX + metrics[c].width,
                                origin.y + metrics[c].horiBearingY - metrics[c].height);

    std::cout << "TL_BR: (" << tl_br.x << ", " << tl_br.y << ", " << tl_br.z << ", " << tl_br.w << ")" << '\n';
    std::cout << "Advance: " << metrics[c].horiAdvance << '\n';
    // Position Vertices
    // Top left
    *(position_vertices + position_start + 0) = tl_br.x;
    *(position_vertices + position_start + 1) = tl_br.y;
    // Top right
    *(position_vertices + position_start + 2) = tl_br.z;
    *(position_vertices + position_start + 3) = tl_br.y;
    // Bottom left
    *(position_vertices + position_start + 4) = tl_br.x;
    *(position_vertices + position_start + 5) = tl_br.w;
    // Bottom right
    *(position_vertices + position_start + 6) = tl_br.z;
    *(position_vertices + position_start + 7) = tl_br.w;

    // Texture Vertices
    // Top left
    *(texture_vertices + texture_start + 0) = texture_rect[c].x;
    *(texture_vertices + texture_start + 1) = texture_rect[c].y;
    // Top right
    *(texture_vertices + texture_start + 2) = texture_rect[c].z;
    *(texture_vertices + texture_start + 3) = texture_rect[c].y;
    // Bottom left
    *(texture_vertices + texture_start + 4) = texture_rect[c].x;
    *(texture_vertices + texture_start + 5) = texture_rect[c].w;
    // Bottom right
    *(texture_vertices + texture_start + 6) = texture_rect[c].z;
    *(texture_vertices + texture_start + 7) = texture_rect[c].w;

    return metrics[c].horiAdvance;
}


void Text2::set_text(std::string body) {
    // Set the new character length
    uint32_t new_body_size = body.size();
    uint32_t total_vertices = new_body_size * 4;
    uint32_t total_elements = new_body_size * 6;

    // Create dynamic arrays
    float *position_vertices = new float[total_vertices * 2];
    float *texture_vertices = new float[total_vertices * 2];

    // Loop through the string and generate vertices
    float origin_x = 0;
    float advance = 0;
    uint32_t index = 0;
    for (int32_t i = 0; i < new_body_size; ++i) {
        advance = font_->character_to_vertices(body[i], position_vertices, texture_vertices, index, index, glm::vec2(origin_x, 0));
        origin_x += advance;
        index += 8;
    }

    const uint32_t group_sizes[2] = {total_vertices * 2 * sizeof(float), total_vertices * 2 * sizeof(float)};
    const uint32_t group_counts[2] = {total_vertices * 2, total_vertices * 2};

    for (int i = 0; i < total_vertices * 2; ++i) {
        position_vertices[i] *= 1./18000.;
    }

    // Set the group data for the vertices in VertexArray
    allocateBuffer(2, group_sizes, group_counts, GL_ARRAY_BUFFER);
    // Set the data in the vertices in VertexArray
    updateBuffer(0, position_vertices);
    updateBuffer(1, texture_vertices);

    // Set the element array indices
    uint32_t *element_array = new uint32_t[total_elements];
    // Turn into function
    uint32_t element_offset = 0;
    for (int32_t i = 0; i < total_elements; i += 6) {
        element_array[i + 0] = 0 + element_offset;
        element_array[i + 1] = 1 + element_offset;
        element_array[i + 2] = 2 + element_offset;

        element_array[i + 3] = 1 + element_offset;
        element_array[i + 4] = 2 + element_offset;
        element_array[i + 5] = 3 + element_offset;
        element_offset += 4;
    }

    //TODO: see if the address of a single variable can be passed in
    const uint32_t element_sizes[1] = {total_elements * sizeof(uint32_t)};
    const uint32_t element_counts[1] = {total_elements};
    allocateBuffer(1, element_sizes, element_counts, GL_ELEMENT_ARRAY_BUFFER);
    updateBuffer(0, element_array, GL_ELEMENT_ARRAY_BUFFER);

    // Specify how the data is stored
    interpretAttributes(0, 0, 2);
    interpretAttributes(1, 1, 2);

    // Free resources
    delete[] position_vertices;
    delete[] texture_vertices;
    delete[] element_array;
}

void apply_scaling(double scaling) {

}
*/