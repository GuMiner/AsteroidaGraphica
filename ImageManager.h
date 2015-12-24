#pragma once

#include <map>
#include <stb/stb_image.h>
#include <glbinding/gl/gl.h>
#include "ImageTexture.h"

// Holds *all* of the texture images and communicates using 'stb_image'. 
// Note that this only supports PNG images, which can be changed by modifying stb_implementations.cpp
class ImageManager
{
    std::map<gl::GLuint, ImageTexture> imageTextures;

public:
    ImageManager();
    gl::GLuint AddImage(const char* filename);
    ImageTexture GetImage(gl::GLuint textureId);

    ~ImageManager();
};

