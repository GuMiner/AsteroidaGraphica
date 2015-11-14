// Holds a pointer to image texture data in GL_RGBA32F fomat.
struct ImageTexture
{
    unsigned int textureId;

    int width;
    int height;
    unsigned char* imageData;

    ImageTexture()
    {
    }

    ImageTexture(unsigned int textureId, unsigned char* imageData, int width, int height)
        : textureId(textureId), imageData(imageData), width(width), height(height)
    {
    }
};