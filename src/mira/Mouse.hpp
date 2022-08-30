#ifndef MIRA_MOUSE_HPP
#define MIRA_MOUSE_HPP

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace mira {
    class Mouse {
    public:
        Mouse(GLFWwindow *window);
        ~Mouse();

        void process();

        glm::vec2 position_past;
        glm::vec2 position_delta;
        glm::vec2 position;

        bool button1_down;
        bool button2_down;
        bool button3_down;
        bool button4_down;
        bool button5_down;
        bool button6_down;
        bool button7_down;
        bool button8_down;

        bool button1_up;
        bool button2_up;
        bool button3_up;
        bool button4_up;
        bool button5_up;
        bool button6_up;
        bool button7_up;
        bool button8_up;

        uint32_t button1_held;
        uint32_t button2_held;
        uint32_t button3_held;
        uint32_t button4_held;
        uint32_t button5_held;
        uint32_t button6_held;
        uint32_t button7_held;
        uint32_t button8_held;

    private:
        GLFWwindow *window_ref;
    };
}

#endif // MIRA_MOUSE_HPP
