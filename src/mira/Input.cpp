#include "mira/Input.hpp"

using namespace mira;

Input::Input(GLFWwindow *window)
    : window_ref(window),
      keyboard(window),
      mouse(window) {
}

Input::~Input() {

}

void Input::process() {
    keyboard.process();
    mouse.process();
}
