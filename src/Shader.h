#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "glad/glad.h"

enum class ShaderType {
    VERTEX_SHADER = GL_VERTEX_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
    TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
    TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
    COMPUTE_SHADER = GL_COMPUTE_SHADER
};

class Shader {
public:
    GLuint Handle = 0;
    Shader(std::initializer_list<std::pair<const std::string, ShaderType>> paths);
    void Use() const;
    void LoadInteger(const std::string &name, int value) const;
    void LoadFloat(const std::string &name, float value) const;
    void LoadVector3(const std::string &name, const glm::vec3 &value) const;
    void LoadVector4(const std::string &name, const glm::vec4 &value) const;
    void LoadMatrix4(const std::string &name, const glm::mat4 &value) const;
    static void Dispatch(int x, int y, int z);
    static void Wait(GLbitfield flags = GL_ALL_BARRIER_BITS);
    ~Shader();

private:
    static std::string Read(const std::string &path);
    static int CreateShader(const std::string &source, ShaderType type);
    static void CompileShader(int shader);
    void CreateProgram(std::vector<int> &shaders);
    void CleanupShaders(std::vector<int> &shaders) const;
};
