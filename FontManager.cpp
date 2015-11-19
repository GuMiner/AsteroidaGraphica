#include <cstring>
#include <fstream>
#include <vector>
#include "FontManager.h"

FontManager::FontManager()
{
    loadedFontFile = nullptr;
    usedWidth = 0;
    usedHeight = 0;
    lastMaxHeight = 0;
}

bool FontManager::LoadFont(const char *fontName)
{
    std::ifstream file(fontName, std::ios::binary | std::ios::ate);
    if (!file)
    {
        return false;
    }

    // We started at the *end* of the file, so we know how long it is! Use that to allocate data and read in the entire file.
    std::ifstream::pos_type fileLength = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer((unsigned int)fileLength);
    file.read(&buffer[0], fileLength);
    file.close();

    // Transfer over the file to our permament buffer.
    loadedFontFile = new unsigned char[(unsigned int)fileLength];
    memcpy_s(loadedFontFile, (rsize_t)fileLength, &buffer[0], (rsize_t)fileLength);

    // Initialize the font and generic OpenGL info.
    stbtt_InitFont(&fontInfo, loadedFontFile, 0);
        
    // Create a new texture for the image.
    glGenTextures(1, &fontTexture);

    // Bind the texture and prepare to send in image data.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTexture);

    int maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    width = maxTextureSize;
    height = maxTextureSize;

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RED, width, height);
   
    // Wrap around if we have excessive UVs
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return true;
}

// Adds the specified font to the font texture, loading the character position information into the provided structure.
void FontManager::AddToFontTexture(CharInfo& charInfo)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTexture);

    // Move down a row if we need to.
    if (usedWidth + charInfo.width > width)
    {
        usedWidth = 0;
        usedHeight += lastMaxHeight;
        lastMaxHeight = 0;
    }

    // Store the image and move around our current position.
    glTexSubImage2D(GL_TEXTURE_2D, 0, usedWidth, usedHeight, charInfo.width, charInfo.height, GL_RED, GL_UNSIGNED_BYTE, charInfo.characterBitmap);
    charInfo.textureX = usedWidth;
    charInfo.textureY = usedHeight;

    usedWidth += charInfo.width;
    lastMaxHeight = vmath::max(lastMaxHeight, charInfo.height);
}

// Returns the character info pertaining to the specified font pixel height and character
CharInfo& FontManager::GetCharacterInfo(int fontPixelHeight, int character)
{
    if (fontData.count(character) == 0)
    {
        // There is no characters at all loaded of the specific type, perform a load of generic text information.
        TextInfo textInfo;
        stbtt_GetFontVMetrics(&fontInfo, &textInfo.ascent, 0, 0);
        
        fontData[character] = textInfo;
    }

    if (fontData[character].characterSizes.count(fontPixelHeight) == 0)
    {
        // We need to add to the mapping of character sizes this new character size.
        CharInfo charInfo;
        charInfo.scale = stbtt_ScaleForPixelHeight(&fontInfo, (float)fontPixelHeight);
        stbtt_GetCodepointHMetrics(&fontInfo, character, &charInfo.advanceWidth, &charInfo.leftSideBearing);
        charInfo.characterBitmap = stbtt_GetCodepointBitmap(&fontInfo, 0, charInfo.scale, character, &charInfo.width, &charInfo.height, &charInfo.xOffset, &charInfo.yOffset);
   
        AddToFontTexture(charInfo);

        fontData[character].characterSizes[fontPixelHeight] = charInfo;
    }

    return fontData[character].characterSizes[fontPixelHeight];
}

int FontManager::GetSentenceVertexCount(std::string& sentence)
{
    // Note that we 'render' space, tab, etc.
    return sentence.length() * verticesPerChar;
}

// Given a sentence, allocates the vertexes corresponding to the sentence.
// The vertexes start at (0, 0, 0) and go in the X-direction, with 1 unit == pixelHeight.
colorTextureVertex* FontManager::AllocateSentenceVertices(std::string& sentence, int pixelHeight, vmath::vec3 textColor)
{
    colorTextureVertex *vertices = new colorTextureVertex[GetSentenceVertexCount(sentence)];

    float lastZPos = 0.0f;
    float lastYPos = 0.0f;
    float lastXPos = 0.0f;
    float vertScale;
    for (int i = 0; i < (int)sentence.length(); i++)
    {
        CharInfo& charInfo = GetCharacterInfo(pixelHeight, sentence[i]);
        if (i == 0)
        {
            vertScale = 1.0f / (float)charInfo.height;
        }

        // Character vertex positions.
        float effectiveWidth = vertScale * (float)charInfo.width;;
        float xDepth = effectiveWidth + lastXPos;
        float yDepth = vertScale * (float)charInfo.height;
        
        // Character texture vertex positions.
        float textureX = (float)charInfo.textureX / (float)width;
        float textureY = (float)charInfo.textureY / (float)height;
        float textureXEnd = (float)(charInfo.textureX + charInfo.width) / (float)width;
        float textureYEnd = (float)(charInfo.textureY + charInfo.height) / (float)height;

        // Lower-left, upper-left, lower-right (CW triangle)
        vertices[i*verticesPerChar + 0].Set(lastXPos, lastYPos, lastZPos, textColor[0], textColor[1], textColor[2], textureX, textureYEnd);
        vertices[i*verticesPerChar + 1].Set(lastXPos, yDepth, lastZPos, textColor[0], textColor[1], textColor[2], textureX, textureY);
        vertices[i*verticesPerChar + 2].Set(xDepth, lastYPos, lastZPos, textColor[0], textColor[1], textColor[2], textureXEnd, textureYEnd);

        // Lower-left, upper-left, upper-right (CW triangle)
        vertices[i*verticesPerChar + 3].Set(xDepth, lastYPos, lastZPos, textColor[0], textColor[1], textColor[2], textureXEnd, textureYEnd);
        vertices[i*verticesPerChar + 4].Set(lastYPos, yDepth, lastZPos, textColor[0], textColor[1], textColor[2], textureX, textureY);
        vertices[i*verticesPerChar + 5].Set(xDepth, yDepth, lastZPos, textColor[0], textColor[1], textColor[2], textureXEnd, textureY);

        lastXPos += effectiveWidth;
    }
    return vertices;
}

// TODO some of this code should remain, some should leave (currently too vague)
void FontManager::RenderSentenceVertices(GLuint vao, GLuint vbo, GLint projLocation, GLint mvLocation, vmath::mat4& perspectiveMatrix, vmath::mat4& mvMatrix, int vertexCount)
{
    // Bind in the texture and vertices we're using.
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTexture);

    glUniformMatrix4fv(projLocation, 1, GL_FALSE, perspectiveMatrix);
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mvMatrix);

    // Draw the text
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

FontManager::~FontManager()
{
    // Free all of the loaded font bitmaps at program end.
    for (std::map<int, TextInfo>::iterator iterator = fontData.begin(); iterator != fontData.end(); iterator++)
    {
        std::map<int, CharInfo>& charSizes = iterator->second.characterSizes;
        for (std::map<int, CharInfo>::iterator charIterator = charSizes.begin(); charIterator != charSizes.end(); charIterator++)
        {
            stbtt_FreeBitmap(charIterator->second.characterBitmap, nullptr);
        }
    }

    // Deletes the loaded font (that stb references)
    if (loadedFontFile != nullptr)
    {
        delete[] loadedFontFile;
    }

    glDeleteTextures(1, &fontTexture);
}
