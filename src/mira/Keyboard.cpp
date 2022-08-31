#include "mira/Keyboard.hpp"

using namespace mira;

Keyboard::Keyboard(GLFWwindow *window)
    : window_ref(window) {
}

Keyboard::~Keyboard() {
}

void Keyboard::process() {
    TESTKEY8(window_ref, A, B, C, D, E, F, G, H)
    TESTKEY8(window_ref, I, J, K, L, M, N, O, P)
    TESTKEY8(window_ref, Q, R, S, T, U, V, W, X)
    TESTKEY8(window_ref, Y, Z, 0, 1, 2, 3, 4, 5)
    TESTKEY8(window_ref, 6, 7, 8, 9, RIGHT, LEFT, DOWN, UP)
    TESTKEY8(window_ref, ESCAPE, ENTER, TAB, BACKSPACE, COMMA, PERIOD, SPACE, SLASH)
}
