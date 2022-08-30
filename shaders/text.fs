#version 330 core

out vec4 Final;

in vec2 fUV;

uniform sampler2D texture;

void main() {
    Final = vec4(1.0, 1.0, 1.0, 0.0);//texture(texture, fUV);
}
