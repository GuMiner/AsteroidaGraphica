#pragma once
#include <map>

class FontManager
{
    std::map<bool, unsigned char*> fontData;
public:
    FontManager();
    bool LoadFont(const char *fontName);
    ~FontManager();
};

