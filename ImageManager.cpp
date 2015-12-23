#include <string>
#include <sstream>
#include "ImageManager.h"
#include <iostream>
#include "Logger.h"

ImageManager::ImageManager()
{
}

// Gets additional image data given the texture ID.
ImageTexture ImageManager::GetImage(GLuint textureId)
{
    return imageTextures[textureId];
}

// Adds an image to the list of tracked images, returning the texture ID of that image.
GLuint ImageManager::AddImage(const char* filename)
{
    // Load in the image
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* imageData = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    if (imageData && width && height)
    {      
        // Create a new texture for the image.
        GLuint newTextureId;
        glGenTextures(1, &newTextureId);
        
        // Bind the texture and send in image data

        //glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum error = glGetError();
        std::cout << error << " " << glewGetErrorString(error) << std::endl;
        glBindTexture(GL_TEXTURE_2D, newTextureId);
        error = glGetError();
        std::cout << error << " " << glewGetErrorString(error) << std::endl;
        // Wrap around if we have excessive UVs
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        error = glGetError();
        std::cout << error << " " << glewGetErrorString(error) << std::endl;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        error = glGetError();
        std::cout << error << " " << glewGetErrorString(error) << std::endl;
        //glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, width, height);
        for (int i = 0; i < width*height * 4; i++)
        {
            imageData[i] = 200;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &imageData[0]);
        //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        error = glGetError();
        std::cout << error << " " << glewGetErrorString(error) << std::endl;
        
        

        imageTextures[newTextureId] = ImageTexture(newTextureId, imageData, width, height);
        return newTextureId;
    }
    else
    {
        std::stringstream errStream;
        errStream << "Failed to load image:" << stbi_failure_reason();
        Logger::LogError(errStream.str().c_str());
    }

    return 0;
}

ImageManager::~ImageManager()
{
    // Free all of the loaded textures and in-memory image data at program end.
    for (std::map<GLuint, ImageTexture>::iterator iterator = imageTextures.begin(); iterator != imageTextures.end(); iterator++)
    {
        glDeleteTextures(1, &iterator->first);
        stbi_image_free(iterator->second.imageData);
    }
}
