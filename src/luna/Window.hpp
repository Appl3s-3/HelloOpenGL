#if !defined(LUNA_WINDOW_HPP)
#define LUNA_WINDOW_HPP

#include <string>
#include <cstdint>
#include <iostream>

#include "mini/miniGL.h"
#include "glm/glm.hpp"
#include "LunaDefines.hpp"

namespace luna {
    inline void Initiate() {
        glfwInit();
    }
    
    inline void Terminate() {
        glfwTerminate();
    }
    
    class Window {
    public:
        Window(uint32_t width, uint32_t height, std::string title, uint32_t major_version = 3, uint32_t minor_version = 3);
        ~Window();

        inline void make_context_current() { glfwMakeContextCurrent(window); }
        inline void poll() { glfwPollEvents(); }
        inline bool not_closed() { return !glfwWindowShouldClose(window); }
        inline void swap_buffers() { glfwSwapBuffers(window); }
        inline GLFWwindow* get() { return window; }

        void clear(double r = 0.0, double g = 0.0, double b = 0.0, double a = 0.0, GLenum o = GL_COLOR_BUFFER_BIT);

        glm::ivec2 dimensions;

    private:
        std::string title;
        GLFWwindow* window;
    };
}

#endif // LUNA_WINDOW_HPP
