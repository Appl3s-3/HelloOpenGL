#include <iostream>

#include <vector>
#include <cstdint>
#include <chrono>

#include "mini/miniGL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "luna/Luna.hpp"
#include "mira/Input.hpp"
#include "Text2.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


void text() {
    luna::Initiate();
    luna::Window window(1280, 720, std::string("Danganronpa Playground"));
    
    luna::Shader text_vs = luna::Shader(GL_VERTEX_SHADER, "../shaders/text.vs");
    luna::Shader text_fs = luna::Shader(GL_FRAGMENT_SHADER, "../shaders/text.fs");
    luna::ShaderProgram program = luna::ShaderProgram(text_vs, text_fs);

    FontAtlas arial_font("../res/arial.ttf");
    //luna::Texture arial_tex = luna::Texture();
    //arial_font.set_texture(arial_tex);
    //arial_tex.bind();

    std::cout << "Created Font" << std::endl;
    std::string t1 = "Once upon a time there lived a princess.";
    std::string t2 = "She was lonely beacuse she lived in a castle all by herself.";
    std::string t3 = "The only person that was around was her grandmother.";

    Text2 words_data = Text2(t1, arial_font);
    Text2 words2_data = Text2(t2, arial_font);
    Text2 words3_data = Text2(t3, arial_font);
    words_data.scale_to(16.0 / 1080.0);
    words2_data.scale_to(16.0 / 1080.0);
    words3_data.scale_to(16.0 / 1080.0);

    const uint32_t vertex_group_sizes1[] = {words_data.vertices.size() * 4, words_data.vertices.size() * 4};
    const uint32_t vertex_group_counts1[] = {words_data.vertices.size(), words_data.vertices.size()};
    const uint32_t index_group_sizes1[] = {words_data.indices.size() * 4};
    const uint32_t index_group_counts1[] = {words_data.indices.size()};

    const uint32_t vertex_group_sizes2[] = {words2_data.vertices.size() * 4, words2_data.vertices.size() * 4};
    const uint32_t vertex_group_counts2[] = {words2_data.vertices.size(), words2_data.vertices.size()};
    const uint32_t index_group_sizes2[] = {words2_data.indices.size() * 4};
    const uint32_t index_group_counts2[] = {words2_data.indices.size()};

    const uint32_t vertex_group_sizes3[] = {words3_data.vertices.size() * 4, words3_data.vertices.size() * 4};
    const uint32_t vertex_group_counts3[] = {words3_data.vertices.size(), words3_data.vertices.size()};
    const uint32_t index_group_sizes3[] = {words3_data.indices.size() * 4};
    const uint32_t index_group_counts3[] = {words3_data.indices.size()};

    std::cout << "Begin Vertex Array" << std::endl;
    luna::VertexArray words = luna::VertexArray();
    words.allocateBuffer(2, vertex_group_sizes1, vertex_group_counts1, GL_ARRAY_BUFFER);
    words.allocateBuffer(1, index_group_sizes1, index_group_counts1, GL_ELEMENT_ARRAY_BUFFER);
    words.updateBuffer(0, words_data.vertices.data());
    words.updateBuffer(1, words_data.indices.data());
    words.interpretAttributes(0, 0, 2);
    words.interpretAttributes(1, 1, 2);
    words.updateBuffer(0, words_data.indices.data(), GL_ELEMENT_ARRAY_BUFFER);

    luna::VertexArray words2 = luna::VertexArray();
    words2.allocateBuffer(2, vertex_group_sizes2, vertex_group_counts2, GL_ARRAY_BUFFER);
    words2.allocateBuffer(1, index_group_sizes2, index_group_counts2, GL_ELEMENT_ARRAY_BUFFER);
    words2.updateBuffer(0, words2_data.vertices.data());
    words2.updateBuffer(1, words2_data.indices.data());
    words2.interpretAttributes(0, 0, 2);
    words2.interpretAttributes(1, 1, 2);
    words2.updateBuffer(0, words2_data.indices.data(), GL_ELEMENT_ARRAY_BUFFER);

    luna::VertexArray words3 = luna::VertexArray();
    words3.allocateBuffer(2, vertex_group_sizes3, vertex_group_counts3, GL_ARRAY_BUFFER);
    words3.allocateBuffer(1, index_group_sizes3, index_group_counts3, GL_ELEMENT_ARRAY_BUFFER);
    words3.updateBuffer(0, words3_data.vertices.data());
    words3.updateBuffer(1, words3_data.indices.data());
    words3.interpretAttributes(0, 0, 2);
    words3.interpretAttributes(1, 1, 2);
    words3.updateBuffer(0, words3_data.indices.data(), GL_ELEMENT_ARRAY_BUFFER);

    program.bind();

    mira::Input in = mira::Input(window.get());

    double delta = 0.;
    double frame_start = glfwGetTime();
    std::cout << frame_start << std::endl;
    //double frame_end = 0.;
    //double idle_time = 0.;

    uint64_t frame_ticks = 0;

    int32_t g[] = {0, 0, 0};
    uint32_t current_text = 0;

    std::cout << "Begin Loop" << std::endl;
    while (window.not_closed()) {
        window.poll();
        in.process();

        if (frame_ticks % 2 == 0) {
            g[current_text] += 2;
        }

        if (in.keyboard.key_A_down == true) {
            g[current_text] = 0;
            ++current_text;
            current_text %= 3;
        }

        window.clear();

        if (current_text == 0) {
            words.draw(0, g[0]);
        }
        else if (current_text == 1) {
            words2.draw(0, g[1]);
        }
        else {
            words3.draw(0, g[2]);
        }

        
        window.swap_buffers();

        ++frame_ticks;
        //frame_end = glfwGetTime();
        do {
            delta = glfwGetTime() - frame_start;
        } while (delta <= 1.0 / 60.0);
        //idle_time = glfwGetTime() - frame_end;
        
        frame_start = glfwGetTime();
    }

    luna::Terminate();
}

int main(int argc, char const *argv[]) {
    text();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
