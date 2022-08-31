#version 330 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vUV;

out vec2 fUV;

void main() {
   fUV = vUV;
   gl_Position = vec4(vPosition.x / 20.0 - 1.0, vPosition.y / 20.0, 0.0, 1.0);
   //gl_Position = transform * vec4(aPos, 1.0);
}
