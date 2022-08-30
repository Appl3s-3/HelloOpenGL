#include "luna/Shader.hpp"

using namespace luna;

Shader::Shader(const GLenum type, const char* source, const SourceText source_type) {
    switch (source_type) {
    case SourceText::string:
        //std::cout << "Creating shader from string." << '\n';
        shader_id = create_shader(type, source);
        break;
    case SourceText::file:
        //std::cout << "Creating shader from file." << '\n';
        const char *src = read_file(source);
        shader_id = create_shader(type, src);
        delete src;
        break;
    }
}

Shader::~Shader() {
    glDeleteShader(shader_id);
}

char *Shader::read_file(const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        const long int length = ftell(f) + 1; // Add 1 to ftell to make space for the NULL terminating character
        fseek(f, 0, SEEK_SET);
        char *buffer = (char*)calloc(length, sizeof(char));
        //char *buffer = new char[length];
        if (buffer != NULL) {
            fread(buffer, sizeof(char), length, f);
        }
        else {
            printf(LUNA_MEMORY_ERROR "Failed to allocate memory.\n");
        }
        fclose(f);
        //buffer[length - 1] = '\0';
        //std::cout << buffer << "<<<EOF>>>" << std::endl;
        return buffer;
    }
    printf(LUNA_FILE_ERROR "Failed to read file.\n");
    return (char*)0;
}

GLuint Shader::create_shader(const GLenum type, const char *source) {
    if (source == NULL) {
        printf(LUNA_FILE_ERROR "Shader source == NULL.\n");
        return 0;
    }
    const uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Error Handling
    int32_t success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        char *shaderType;
        switch (type) {
            case GL_VERTEX_SHADER:
                shaderType = (char*)"vertex";
                break;
            case GL_FRAGMENT_SHADER:
                shaderType = (char*)"fragment";
                break;
            default:
                shaderType = (char*)"unknown";
                break;
        }
        printf(LUNA_CREATION_ERROR "Failed to compile %s shader.\n%s\n", shaderType, infoLog);
        return 0;
    }

    //free((char*)src);
    return shader;
}
