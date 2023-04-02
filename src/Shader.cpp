#include "Shader.h"
#include "spdlog/spdlog.h"
#include <fstream>
#include <vector>
#include <sstream>

Shader::Shader(std::initializer_list<std::pair<const std::string &, ShaderType>> paths) {
    std::vector<std::pair<std::string, ShaderType>> sources;
    for (const auto &path : paths) {
        sources.emplace_back(Read(path.first), path.second);
    }

    std::vector<int> shaders;
    for (const auto &source : sources) {
        shaders.push_back(CreateShader(source.first, source.second));
    }

    for (const auto &shader : shaders) {
        CompileShader(shader);
    }

    CreateProgram(shaders);

    CleanupShaders(shaders);
}

std::string Shader::Read(const std::string &path) {
    std::ifstream file(path);
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

int Shader::CreateShader(const std::string &source, ShaderType type) {
    int shader = glCreateShader(static_cast<GLenum>(type));
    const char *source_ptr = source.c_str();
    glShaderSource(shader, 1, &source_ptr, nullptr);
    return shader;
}

void Shader::CompileShader(int shader) {
    glCompileShader(shader);
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> logMessage(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, logMessage.data());
        std::string errorMessage(logMessage.data(), logMessage.size());
        spdlog::error("Failed to compile shader: {}", errorMessage);
    }
}

void Shader::CreateProgram(std::vector<int> &shaders) {
    Handle = glCreateProgram();

    for (const auto &shader: shaders) {
        glAttachShader(Handle, shader);
    }

    glLinkProgram(Handle);
}

void Shader::CleanupShaders(std::vector<int> &shaders) const {
    for (const auto &shader: shaders) {
        glDetachShader(Handle, shader);
        glDeleteShader(shader);
    }
}

void Shader::Use() const {
    glUseProgram(Handle);
}

Shader::~Shader() {
    glDeleteShader(Handle);
}

void Shader::LoadInteger(const std::string &name, int value) const {
    GLint location = glGetUniformLocation(Handle, name.c_str());
    glUniform1i(location, value);
}

void Shader::LoadFloat(const std::string &name, float value) const {
    GLint location = glGetUniformLocation(Handle, name.c_str());
    glUniform1f(location, value);
}

void Shader::LoadVector3(const std::string &name, const glm::vec3 &value) const {
    GLint location = glGetUniformLocation(Handle, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::LoadVector4(const std::string &name, const glm::vec4 &value) const {
    GLint location = glGetUniformLocation(Handle, name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::LoadMatrix4(const std::string &name, const glm::mat4 &value) const {
    GLint location = glGetUniformLocation(Handle, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::Dispatch(int x, int y, int z) {
    glDispatchCompute(x, y, z);
}

void Shader::Wait(GLbitfield flags) {
    glMemoryBarrier(flags);
}
