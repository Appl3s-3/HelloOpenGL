#include "luna/ShaderProgram.hpp"

using namespace luna;

ShaderProgram::ShaderProgram(Shader& vertex_shader, Shader& fragment_shader) {
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader.shader_id);
    glAttachShader(program_id, fragment_shader.shader_id);
    glLinkProgram(program_id);

    int32_t success;
    char infoLog[512];
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id, 512, NULL, infoLog);
        std::cout << LUNA_CREATION_ERROR "Failed to link shader program.\n" << infoLog << std::endl;
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program_id);
}

void ShaderProgram::bind() {
    glUseProgram(program_id);
}