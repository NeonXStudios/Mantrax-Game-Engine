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

    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    stbi_write_jpg(filename, width, height, 4, pixels, 100);

    delete[] pixels;
    glBindTexture(GL_TEXTURE_2D, 0);
}
