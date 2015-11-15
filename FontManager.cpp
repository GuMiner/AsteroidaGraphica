#include <fstream>
#include <vector>
#include <stb/stb_truetype.h>
#include "FontManager.h"

FontManager::FontManager()
{
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

    // TODO replicate the same image structure to make a map of data that we free elsewhere.
    //stbtt_fontinfo fontInfo;
    //stbtt_InitFont(&fontInfo, &buffer[0], 0);
    
    return true;
}


FontManager::~FontManager()
{
}
