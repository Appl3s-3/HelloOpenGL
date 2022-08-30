#ifndef MIRA_INPUT_HPP
#define MIRA_INPUT_HPP

#include "mira/Keyboard.hpp"
#include "mira/Mouse.hpp"

namespace mira {
    class Input {
    public:
        Input(GLFWwindow *window);
        ~Input();

        void process();

        Keyboard keyboard;
        Mouse mouse;
        
    private:
        GLFWwindow *window_ref;
    };
}

#endif // MIRA_INPUT_HPP
