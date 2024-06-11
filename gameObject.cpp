#include "GameObject.h"

GameObject::GameObject(Renderer &renderer, Window window, int type, glm::vec2 pos, float rot, glm::vec2 scale, int texture)
    : renderer(renderer),
      position(pos), rotation(rot), scale(scale), texture(texture)
{
  int screenWidth;
  int screenHeight;
  glfwGetFramebufferSize(window.window, &screenWidth, &screenHeight);
  if (type == 1)
  {
    startVertex = renderer.indices.size();

    vertexCount = 6;
    renderer.addRectangle({pos.x, pos.y}, {screenWidth / 9600.0f, screenHeight / 3085.714f});
  }
  modelMatrix = glm::mat4(1.0f);
}

void GameObject::draw(Shader &shader)
{

  if (position != prevPosition || rotation != prevRotation || scale != prevScale)
  {
    isModelMatrixDirty = true;
  }

  if (isModelMatrixDirty)
  {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    prevPosition = position;
    prevRotation = rotation;
    prevScale = scale;

    isModelMatrixDirty = false;
  }

  shader.setMatrix4("transform", modelMatrix);
  if (texture >= 0)
  {
    shader.setInt("textureActive", texture);
  }
  else
  {
    shader.setInt("textureActive", 0);
  }
  renderer.draw(startVertex, vertexCount);
}