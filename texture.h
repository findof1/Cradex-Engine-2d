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

  Texture(const char *imagePath, GLenum textureType, GLenum texSlot, GLenum format, GLenum pixelType)
  {
    type = textureType;

    glGenTextures(1, &ID);
    slot = texSlot;
    glActiveTexture(slot);
    glBindTexture(type, ID);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data)
    {
      glTexImage2D(textureType, 0, format, width, height, 0, format, pixelType, data);
      glGenerateMipmap(textureType);
    }
    else
    {
      std::cerr << "Failed to load texture: " << imagePath << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(type, 0);
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