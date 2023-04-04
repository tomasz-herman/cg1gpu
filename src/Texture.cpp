#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "spdlog/spdlog.h"

Texture::Options::Options(std::initializer_list<std::pair<int, int>> parameters) {
    for (const auto &parameter: parameters) {
        SetParameter(parameter.first, parameter.second);
    }
}

Texture::Options Texture::Options::SetParameter(int name, int value) {
    Parameters[name] = value;
    return *this;
}

Texture::Texture() {
    glGenTextures(1, &Handle);
}

Texture::Texture(const std::string & path, const Texture::Options & options, bool generateMipmaps) {
    glGenTextures(1, &Handle);
    Bind();
    LoadDataFromFile(path);
    ApplyOptions(options);
    if(generateMipmaps) GenerateMipmaps();
    Unbind();
}

void Texture::ApplyOptions(const Texture::Options &options) {
    for (const auto &parameter : options.Parameters) {
        glTexParameteri(GL_TEXTURE_2D, parameter.first, parameter.second);
    }
}

void Texture::GenerateMipmaps() {
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Bind(int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, Handle);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::LoadDataFromFile(const std::string &path) {
    int width, height, channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (data) {
        LoadData(data, width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);

        stbi_image_free(data);
    } else {
        spdlog::error("Failed to read image {}", path);
    }
}

void Texture::LoadDataFromMemory(unsigned char const *loc, int len) {
    int width, height, channels;
    unsigned char *data = stbi_load_from_memory(loc, len, &width, &height, &channels, 4);
    if (data) {
        LoadData(data, width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);

        stbi_image_free(data);
    } else {
        spdlog::error("Failed to read image from memory");
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &Handle);
}

Texture::Options Texture::Options::Default = Texture::Options(
        {
                std::pair<int, int>(GL_TEXTURE_MIN_FILTER, GL_LINEAR),
                std::pair<int, int>(GL_TEXTURE_MAG_FILTER, GL_LINEAR),
                std::pair<int, int>(GL_TEXTURE_WRAP_S, GL_REPEAT),
                std::pair<int, int>(GL_TEXTURE_WRAP_T, GL_REPEAT),
        });

Texture::Options Texture::Options::Empty = Texture::Options({});