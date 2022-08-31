#include "VertexArray.hpp"

using namespace luna;

VertexArray::VertexArray()
    : vertex_buffer_count(0),
      vertex_buffer_size(0),
      vertex_buffer_max_size(0),
      element_buffer_count(0),
      element_buffer_size(0),
      element_buffer_max_size(0)
      {
    glGenVertexArrays(1, &array_id);
    glGenBuffers(1, &vertex_id);
    glGenBuffers(1, &element_id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &array_id);
    glDeleteBuffers(1, &vertex_id);
    glDeleteBuffers(1, &element_id);
}

void *VertexArray::get_buffer_access(const GLenum buffer_type) {
    return glMapBuffer(buffer_type, GL_READ_WRITE);
}

void VertexArray::stop_buffer_access(const GLenum buffer_type) {
    GLboolean result = glUnmapBuffer(buffer_type);
    if (result == GL_FALSE) {
        // Error handling
    }
}

void VertexArray::draw(const GLenum mode) {
    bindVertexArray();
    glDrawElements(mode, element_buffer_count, GL_UNSIGNED_INT, (const GLvoid*)0);
    unbindAll();
}

void VertexArray::draw(GLuint const start, GLuint const end, GLenum const mode) {
    bindVertexArray();
    glDrawRangeElements(mode, start, end, (end - start) * 3, GL_UNSIGNED_INT, (const GLvoid*)0);
    unbindAll();
}

void VertexArray::updateBuffer(const uint32_t group_id, const GLvoid *data, const GLenum buffer_type) {
    bindVertexArray();
    switch (buffer_type) {
        case GL_ARRAY_BUFFER:
            bindVertexBuffer();
            glBufferSubData(GL_ARRAY_BUFFER, group_vertex_offset[group_id], group_vertex_size[group_id], data);
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            bindElementBuffer();
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, group_element_offset[group_id], group_element_size[group_id], data);
            break;
        default:
            std::cout << LUNA_BUFFER_ERROR "Attempted to update an unknown buffer type in a Vertex Array." << std::endl;
            break;
    }
    unbindAll();
}

void VertexArray::allocateBuffer(const uint32_t subgroup_count, const uint32_t *group_sizes, const uint32_t *group_counts, const GLenum buffer_type, const GLenum usage) {
    // Create pointers to members of the class
    std::vector<uint32_t> *group_offset = NULL;
    std::vector<uint32_t> *group_size = NULL;
    std::vector<uint32_t> *group_count = NULL;
    uint32_t *buffer_size = NULL;
    uint32_t *buffer_max_size = NULL;
    uint32_t *buffer_count = NULL;

    // Set pointers to members of the class that correspond to buffer_type
    switch (buffer_type) {
    case GL_ARRAY_BUFFER:
        group_offset = &group_vertex_offset;
        group_count = &group_vertex_count;
        group_size = &group_vertex_size;

        buffer_size = &vertex_buffer_size;
        buffer_max_size = &vertex_buffer_max_size;
        buffer_count = &vertex_buffer_count;
        break;
    case GL_ELEMENT_ARRAY_BUFFER:
        group_offset = &group_element_offset;
        group_count = &group_element_count;
        group_size = &group_element_size;

        buffer_size = &element_buffer_size;
        buffer_max_size = &element_buffer_max_size;
        buffer_count = &element_buffer_count;
        break;
    default:
        std::cout << LUNA_BUFFER_ERROR "Attempted to allocate an unknown buffer type to a Vertex Array." << std::endl;
        return;
    }

    // Clear the old group data
    group_offset->clear();
    group_count->clear();
    group_size->clear();

    // Sigma counters
    int total_size = 0;
    int total_count = 0;

    // Calculate and set new group data
    for (int i = 0; i < subgroup_count; ++i) {
        group_offset->push_back(total_size);
        group_count->push_back(total_count);
        group_size->push_back(*(group_sizes + i));

        total_size += *(group_sizes + i);
        total_count += *(group_counts + i);
    }

    // If the total_size exceeds the current buffer_max_size
    // Reallocate the buffer
    if (total_size > *buffer_max_size) {
        *buffer_max_size = total_size;
        *buffer_count = total_count;

        bindVertexArray();

        switch (buffer_type) {
            case GL_ARRAY_BUFFER:
                bindVertexBuffer();
                glBufferData(GL_ARRAY_BUFFER, *buffer_max_size, NULL, usage);
                break;
            case GL_ELEMENT_ARRAY_BUFFER:
                bindElementBuffer();
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, *buffer_max_size, NULL, usage);
                break;
            default:
                std::cout << LUNA_BUFFER_ERROR "Attempted to allocate an unknown buffer type to a Vertex Array." << std::endl;
                return;
        }

        unbindAll();
    } // Reallocate the buffer

    // Set buffer_size to the amount of used space
    *buffer_size = total_size;
}

void VertexArray::interpretAttributes(const uint32_t group_id, const GLuint attribute_index, const GLint attribute_size, const GLenum data_type, const GLboolean normalized) {
    bindVertexArray();
    // Stride is the vertex size
    // Normalized works on chars (0-255), etc
    bindVertexBuffer();
    // Accessing a std::vector will return a reference, thus the type must be cast to a pointer before void to avoid a warning.
    glVertexAttribPointer(attribute_index, attribute_size, data_type, normalized, 0, (const void*)(uintptr_t)(group_vertex_offset[group_id]));
    glEnableVertexAttribArray(attribute_index);
    unbindAll();
}

inline void VertexArray::bindVertexArray() {
    glBindVertexArray(array_id);
}

inline void VertexArray::bindVertexBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
}

inline void VertexArray::bindElementBuffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_id);
}

inline void VertexArray::unbindAll() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
