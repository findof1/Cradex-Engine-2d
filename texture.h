#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>

class Texture
{
public:
  unsigned int ID;
  GLenum type;
  int slot;

  Texture(const char *imagePath, GLenum texSlot)
  {
    type = GL_TEXTURE_2D;

    std::string extension = getFileExtension(imagePath);
    GLenum format = GL_RGB;
    if (extension == "png")
    {
      format = GL_RGBA;
    }

    glGenTextures(1, &ID);
    slot = texSlot;
    glActiveTexture(slot);
    glBindTexture(type, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
      std::cerr << "Failed to load texture: " << imagePath << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(type, 0);
  }

  static std::string getFileExtension(const std::string &filename)
  {
    size_t pos = filename.find_last_of(".");
    if (pos != std::string::npos)
      return filename.substr(pos + 1);
    else
      return "";
  }

  void Bind() const
  {
    glActiveTexture(slot);
    glBindTexture(type, ID);
  }

  void Unbind() const
  {
    glBindTexture(type, 0);
  }

  void Delete() const
  {
    glDeleteTextures(1, &ID);
  }
};

#endif