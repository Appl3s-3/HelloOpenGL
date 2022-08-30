#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <iostream>
#include <vector>
#include "mini/miniGL.h"
#include "LunaDefines.hpp"

/* Example Usage
float vertices[] = {0.0, 0.0,
                    1.0, 0.0,
                    0.0, 1.0}
                    
float tex_coords[] = {0.0, 0.0,
                      1.0, 0.0,
                      0.0, 1.0};

float indices[] = {1, 2, 3};

const uint32_t vertex_group_sizes[] = {24, 24};
const uint32_t vertex_group_counts[] = {6, 6};
const uint32_t index_group_sizes[] = {12};
const uint32_t index_group_counts[] = {3};

VertexArray objects();
objects.allocateBuffer(2, vertex_group_sizes, vertex_group_counts, GL_ARRAY_BUFFER);
objects.allocateBuffer(1, index_group_sizes, index_group_counts, GL_ELEMENT_ARRAY_BUFFER);
objects.updateBuffer(0, vertices);
objects.updateBuffer(1, tex_coords);
objects.interpretAttributes(0, 0, 2);
objects.interpretAttributes(1, 1, 2);
objects.updateBuffer(0, indices, GL_ELEMENT_ARRAY_BUFFER);

objects.draw();
// objects.draw(0, 1);
*/
namespace luna {
    /* VertexArray is a container to manage a huge array of data stored on the gpu
     * The class manages different 'sub groups' for both vertices and indices and is highly versatile
     * For example, there can be one sub group for position vertices and another for texture coordinates
     * Or instead, 2 for position vertices. Likewise, 2 for each also works.
     * To specify the layout of the VertexArray use the allocateBuffer method. (See above)
     * The purpose of each vertex sub group is specified with the interpretAttributes method. (See above)
     * Since element array buffers can be divided into sub groups, they can be drawn individually. (TODO: drawGroup method)
     * Simpler draw methods are provided if the VertexArray will only be used for a single object.
     * Additionally, sub groups can be individually updated with updateBuffer.
     */
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        //uint32_t get_max_size();
        //uint32_t get_size();
        inline void *get_buffer_access(const GLenum buffer_type = GL_ARRAY_BUFFER);
        void stop_buffer_access(const GLenum buffer_type = GL_ARRAY_BUFFER);

        void draw(const GLenum mode = GL_TRIANGLES);
        void draw(GLuint const start, GLuint const end, GLenum const mode = GL_TRIANGLES);

        void updateBuffer(const uint32_t group_id, const GLvoid *data, const GLenum buffer_type = GL_ARRAY_BUFFER);

        /* group_sizes: array containing size of each group in bytes
        * group_counts: array containing the number of elements (not indices) in each group
        */
        void allocateBuffer(const uint32_t subgroup_count, const uint32_t *group_sizes, const uint32_t *group_counts, const GLenum buffer_type, const GLenum usage = GL_STATIC_DRAW);
        /* attribute_index: the index at which the data is passed to the shader
        * attribute_size: the dimensions for the attribute
        */
        void interpretAttributes(const uint32_t group_id, const GLuint attribute_index, const GLint attribute_size, const GLenum data_type = GL_FLOAT, const GLboolean normalized = GL_FALSE);

    private:
        inline void bindVertexArray();
        inline void bindVertexBuffer();
        inline void bindElementBuffer();
        inline void unbindAll();

        GLuint array_id;
        GLuint vertex_id;
        GLuint element_id;

        // Offsets and sizes are in bytes
        // Representing the buffers within a vertex attribute object
        std::vector<uint32_t> group_vertex_offset;
        std::vector<uint32_t> group_vertex_count;
        std::vector<uint32_t> group_vertex_size;

        std::vector<uint32_t> group_element_offset;
        std::vector<uint32_t> group_element_count;
        std::vector<uint32_t> group_element_size;

        // Representing the whole vertex attribute object
        uint32_t vertex_buffer_count;
        uint32_t vertex_buffer_size;
        uint32_t vertex_buffer_max_size;

        uint32_t element_buffer_count;
        uint32_t element_buffer_size;
        uint32_t element_buffer_max_size;
    };
} // luna

#endif // VERTEX_ARRAY_HPP
