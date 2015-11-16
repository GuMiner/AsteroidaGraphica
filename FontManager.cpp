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
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fontTexture);

    int maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    width = maxTextureSize;
    height = maxTextureSize;

    glTextureStorage2D(GL_TEXTURE_2D, 1, GL_RED, width, height);
   
    // Wrap around if we have excessive UVs
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return true;
}

// Adds the specified font to the font texture, loading the character position information into the provided structure.
void FontManager::AddToFontTexture(CharInfo& charInfo)
{
    glActiveTexture(GL_TEXTURE1);
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

// Given a sentence, allocates the vertexes corresponding to the sentence.
// The vertexes start at (0, 0, 0) and go in the X-direction, with 1 unit == pixelHeight.
colorTextureVertex* FontManager::AllocateSentenceVertices(std::string sentence, int pixelHeight, vmath::vec3 textColor)
{
    // Note that we 'render' space, tab, etc.
    int verticesPerChar = 6;
    colorTextureVertex *vertices = new colorTextureVertex[sentence.length() * verticesPerChar];

    vmath::vec3 lastPos(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < (int)sentence.length(); i++)
    {
        CharInfo& charInfo = GetCharacterInfo(pixelHeight, sentence[i]);

        // TODO fill in the 0's and 1's with the correct data and fill in the 0's and 1's with the correct texture data.

        // Lower-left, upper-left, lower-right (CW triangle)
        vertices[i*verticesPerChar + 0].Set(0, 0, lastPos[2], textColor[0], textColor[1], textColor[2], 0, 1);
        vertices[i*verticesPerChar + 1].Set(0, 1, lastPos[2], textColor[0], textColor[1], textColor[2], 0, 0);
        vertices[i*verticesPerChar + 2].Set(1, 0, lastPos[2], textColor[0], textColor[1], textColor[2], 1, 1);

        // Lower-left, upper-left, upper-right (CW triangle)
        vertices[i*verticesPerChar + 3].Set(1, 0, lastPos[2], textColor[0], textColor[1], textColor[2], 1, 1);
        vertices[i*verticesPerChar + 4].Set(0, 1, lastPos[2], textColor[0], textColor[1], textColor[2], 0, 0);
        vertices[i*verticesPerChar + 5].Set(1, 1, lastPos[2], textColor[0], textColor[1], textColor[2], 1, 0);
    }
    return vertices;
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
