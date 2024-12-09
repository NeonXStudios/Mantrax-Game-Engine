#include "../includes/GarinIO.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

std::string GarinIO::GetFileNameWithoutExtension(const std::string &filepath)
{
    fs::path pathObj(filepath);
    return pathObj.stem().string();
}

void GarinIO::replaceAll(std::string &str, char oldChar, char newChar)
{
    std::replace(str.begin(), str.end(), oldChar, newChar);
}

std::string GarinIO::GetWithAfterAssetDir(const std::string &fullPath)
{
    std::string keyword = "assets";
    size_t pos = fullPath.find(keyword);

    if (pos != std::string::npos)
    {
        std::string relativePath = fullPath.substr(pos);
        replaceAll(relativePath, '\\', '/');
        replaceAll(relativePath, '/', '/');
        return relativePath;
    }
    return "";
}

void GarinIO::screenshot(unsigned int textureID, int width, int height, const char *filename)
{
    glBindTexture(GL_TEXTURE_2D, textureID);

    unsigned char *pixels = new unsigned char[width * height * 4];
    unsigned char *flipped = new unsigned char[width * height * 4];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int srcPos = (y * width + x) * 4;
            int dstPos = ((height - 1 - y) * width + x) * 4;

            flipped[dstPos + 0] = pixels[srcPos + 0]; // R
            flipped[dstPos + 1] = pixels[srcPos + 1]; // G
            flipped[dstPos + 2] = pixels[srcPos + 2]; // B
            flipped[dstPos + 3] = pixels[srcPos + 3]; // A
        }
    }

    stbi_write_jpg(filename, width, height, 4, flipped, 100);

    delete[] pixels;
    delete[] flipped;
    glBindTexture(GL_TEXTURE_2D, 0);
}