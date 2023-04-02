#pragma once

#include <map>
#include <string>
#include "glad/glad.h"

class Texture {
public:
    class Options {
    public:
        static Options Default, Empty;
        std::map<int, int> Parameters;
        Options(std::initializer_list<std::pair<int, int>> parameters);
        Options SetParameter(int name, int value);
    };

    GLuint Handle = 0;
    Texture();
    explicit Texture(const std::string &path, const Options &options = Options({}), bool generateMipmaps = true);

    void ApplyOptions(const Options &options);
    static void GenerateMipmaps();
    void Bind(int unit = 0) const;
    static void Unbind();
    void LoadDataFromFile(const std::string &path);
    void LoadDataFromMemory(unsigned char const *loc, int len);

    template<class T>
    void LoadData(T *data, int width, int height, int internalFormat, int format, int type, int level = 0) {
        glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, format, type, data);
    }

    template<class T>
    void UpdateData(T *data, int xOffset, int yOffset, int width, int height, int format, int type, int level = 0) {
        glTexSubImage2D(GL_TEXTURE_2D, level, xOffset, yOffset, width, height, format, type, data);
    }

    template<class T>
    void ReadData(T *data, int format, int type, int level = 0) {
        glGetTexImage(GL_TEXTURE_2D, level, format, type, data);
    }

    template<class T>
    void ClearData(T *data, int format, int type, int level = 0) {
        glClearTexImage(GL_TEXTURE_2D, level, format, type, data);
    }

    ~Texture();
};