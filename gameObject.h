#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "renderer.h"
#include "shader.h"
#include "window.h"

class GameObject
{
public:
  glm::vec2 position;
  float rotation;
  glm::vec2 scale;
  Renderer &renderer;
  int startVertex;
  int vertexCount;
  glm::mat4 modelMatrix;
  bool isModelMatrixDirty;
  glm::vec2 prevPosition;
  float prevRotation;
  glm::vec2 prevScale;
  int texture;

  GameObject(Renderer &renderer, Window window, int type = 1, glm::vec2 pos = glm::vec2(0.0f), float rot = 0.0f, glm::vec2 scale = glm::vec2(1.0f), int texture = -1);

  void draw(Shader &shader);
};

#endif