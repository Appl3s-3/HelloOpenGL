#ifndef MIRA_KEYBOARD_HPP
#define MIRA_KEYBOARD_HPP

#include "GLFW/glfw3.h"

//uint32_t id ## _held; // Keys can be held down for ~200 days given that the program is running at 240 fps
//uint8_t id ## _down; // Be wary of padding

#define TESTKEY8(window, id1, id2, id3, id4, id5, id6, id7, id8)\
    key_ ## id1 ## _held = (++key_ ## id1 ## _held * (glfwGetKey(window, GLFW_KEY_ ## id1 ) == GLFW_PRESS));\
    key_ ## id2 ## _held = (++key_ ## id2 ## _held * (glfwGetKey(window, GLFW_KEY_ ## id2 ) == GLFW_PRESS));\
    key_ ## id3 ## _held = (++key_ ## id3 ## _held * (glfwGetKey(window, GLFW_KEY_ ## id3 ) == GLFW_PRESS));\
    key_ ## id4 ## _held = (++key_ ## id4 ## _held * (glfwGetKey(window, GLFW_KEY_ ## id4 ) == GLFW_PRESS));\
    key_ ## id5 ## _held = (++key_ ## id5 ## _held * (glfwGetKey(window, GLFW_KEY_ ## id5 ) == GLFW_PRESS));\
    key_ ## id6 ## _held = (++key_ ## id6 ## _held * (glfwGetKey(window, GLFW_KEY_ ## id6 ) == GLFW_PRESS));\
    key_ ## id7 ## _held = (++key_ ## id7 ## _held * (glfwGetKey(window, GLFW_KEY_ ## id7 ) == GLFW_PRESS));\
    key_ ## id8 ## _held = (++key_ ## id8 ## _held * (glfwGetKey(window, GLFW_KEY_ ## id8 ) == GLFW_PRESS));\
    key_ ## id1 ## _down = (key_ ## id1 ## _held == 1 );\
    key_ ## id2 ## _down = (key_ ## id2 ## _held == 1 );\
    key_ ## id3 ## _down = (key_ ## id3 ## _held == 1 );\
    key_ ## id4 ## _down = (key_ ## id4 ## _held == 1 );\
    key_ ## id5 ## _down = (key_ ## id5 ## _held == 1 );\
    key_ ## id6 ## _down = (key_ ## id6 ## _held == 1 );\
    key_ ## id7 ## _down = (key_ ## id7 ## _held == 1 );\
    key_ ## id8 ## _down = (key_ ## id8 ## _held == 1 );

#define IDENTIFYKEY8(id1, id2, id3, id4, id5, id6, id7, id8)\
    uint32_t key_ ## id1 ## _held;\
    uint32_t key_ ## id2 ## _held;\
    uint32_t key_ ## id3 ## _held;\
    uint32_t key_ ## id4 ## _held;\
    uint32_t key_ ## id5 ## _held;\
    uint32_t key_ ## id6 ## _held;\
    uint32_t key_ ## id7 ## _held;\
    uint32_t key_ ## id8 ## _held;\
    uint8_t key_ ## id1 ## _down;\
    uint8_t key_ ## id2 ## _down;\
    uint8_t key_ ## id3 ## _down;\
    uint8_t key_ ## id4 ## _down;\
    uint8_t key_ ## id5 ## _down;\
    uint8_t key_ ## id6 ## _down;\
    uint8_t key_ ## id7 ## _down;\
    uint8_t key_ ## id8 ## _down;

namespace mira {
    class Keyboard {
    public:
        Keyboard(GLFWwindow *window);
        ~Keyboard();

        // Commonly used methods
        void process();

        // Public members
        IDENTIFYKEY8(A, B, C, D, E, F, G, H)
        IDENTIFYKEY8(I, J, K, L, M, N, O, P)
        IDENTIFYKEY8(Q, R, S, T, U, V, W, X)
        IDENTIFYKEY8(Y, Z, 0, 1, 2, 3, 4, 5)
        IDENTIFYKEY8(6, 7, 8, 9, RIGHT, LEFT, DOWN, UP)
        IDENTIFYKEY8(ESCAPE, ENTER, TAB, BACKSPACE, COMMA, PERIOD, SPACE, SLASH)
    private:
        GLFWwindow *window_ref;
    };
}

#endif // MIRA_KEYBOARD_HPP
