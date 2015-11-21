#pragma once
#include <map>
#include <GL\glew.h>
#include <stb\stb_truetype.h>
#include "TextInfo.h"
#include "Vertex.h"
#include "vmath.hpp"

// Manages the in-game font. *Note that this only supports a single font.*
class FontManager
{
    // Maps characters to the TextInfo representing each character,
    std::map<int, TextInfo> fontData;

    // Holds the font texture that is regenerated as necessary. This is bound to GL_TEXTURE0 for now, but probably should have it's own binding point.
    GLuint fontTexture;
    int width;
    int height;

    int usedWidth;
    int usedHeight;
    int lastMaxHeight;

    // Holds STB font info for loading in new font data as necessary
    stbtt_fontinfo fontInfo;
    unsigned char *loadedFontFile;

    const int verticesPerChar = 4;
    void AddToFontTexture(CharInfo& charInfo);
    CharInfo& GetCharacterInfo(int fontPixelHeight, int character);
public:
    FontManager();
    bool LoadFont(const char *fontName);

    int GetSentenceVertexCount(std::string& sentence);
    colorTextureVertex* AllocateSentenceVertices(std::string& sentence, int pixelHeight, vmath::vec3 textColor);
    void RenderSentenceVertices(GLuint vao, GLuint vbo, GLint projLocation, GLint mvLocation, vmath::mat4& perspectiveMatrix, vmath::mat4& mvMatrix, int vertexCount);

    ~FontManager();
};

