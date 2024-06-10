#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <vector>

class Renderer
{
public:
  unsigned int VAO, VBO, EBO;
  std::vector<float> vertices;
  std::vector<int> indices;

  Renderer(const std::vector<float> &inputVertices, const std::vector<int> &inputIndices)
  {
    vertices = inputVertices;
    indices = inputIndices;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
  }

  void draw(int start, int count) const
  {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void *)(start * sizeof(int)));
  }

  void addVertices(const std::vector<float> &newVertices)
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, newVertices.size() * sizeof(float), newVertices.data());
  }

  void addRectangle(const std::vector<float> &position, const std::vector<float> &scale, const std::vector<float> &color = {1, 1, 1, 1}, const std::vector<float> &texPos = {0, 1})
  {
    float x = position[0];
    float y = position[1];
    float z;
    if (2 < position.size())
    {
      z = position[2];
    }
    else
    {
      z = 0.0f;
    }

    float width = scale[0];
    float height = scale[1];

    std::vector<float> rectVertices = {
        x, y, z, color[0], color[1], color[2], color[3], texPos[0], texPos[1],
        x + width, y, z, color[0], color[1], color[2], color[3], texPos[1], texPos[1],
        x + width, y + height, z, color[0], color[1], color[2], color[3], texPos[1], texPos[0],
        x, y + height, z, color[0], color[1], color[2], color[3], texPos[0], texPos[0]};

    int startIndex = vertices.size() / 9;
    int numVertices = rectVertices.size() / 9;
    for (int i = 0; i < numVertices; ++i)
    {
      vertices.insert(vertices.end(), rectVertices.begin() + i * 9, rectVertices.begin() + (i + 1) * 9);
    }

    std::vector<int> rectIndices = {
        startIndex, startIndex + 1, startIndex + 3,
        startIndex + 1, startIndex + 2, startIndex + 3};

    int startIdx = indices.size();
    for (int idx : rectIndices)
    {
      indices.push_back(idx);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_DYNAMIC_DRAW);
  }

  ~Renderer()
  {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }
};

#endif