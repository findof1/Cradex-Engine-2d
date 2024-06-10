#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "shader.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "renderer.h"
#include "window.h"

void processInput(GLFWwindow *window, Shader shader, float *movX, float *movY, float deltaTime);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main()
{
  Window window;
  Shader shader("./Shaders/vertexShader.vs", "./Shaders/fragmentShader.fs");

  Texture texture1("awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  // Texture texture2("awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);

  Renderer renderer({}, {});
  renderer.addRectangle({-1.0f, -1.0f, -0.5f}, {2.0f, 2.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 2.0f});
  renderer.addRectangle({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f});
  shader.use();

  shader.setInt("textureActive", 1);

  shader.setInt("texture1", 0);

  shader.setInt("mixVal", 1.0f);

  glm::vec4 overrideColor(-1.0f, -1.0f, -1.0f, -1.0f);
  shader.setFloatVec4("overrideColor", overrideColor);
  glm::mat4 trans = glm::mat4(1.0f);

  float movX = 0;
  float movY = 0;

  double lastFrame = glfwGetTime();
  while (!glfwWindowShouldClose(window.window))
  {
    double currentFrame = glfwGetTime();
    float deltaTime = static_cast<float>(currentFrame - lastFrame);
    lastFrame = currentFrame;

    processInput(window.window, shader, &movX, &movY, deltaTime);

    window.clear();

    shader.use();
    texture1.Bind();

    trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(1.2f, 2.1f, 0.0f));
    shader.setMaxrix4("transform", trans);

    shader.setInt("textureActive", 1);
    renderer.draw(0, 6);
    shader.setInt("textureActive", 0);

    trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(0.2f, 0.35f, 0.0f));
    trans = glm::translate(trans, glm::vec3(movX, movY, 0.0f));
    shader.setMaxrix4("transform", trans);

    glm::vec4 overrideColor((sin(glfwGetTime()) + 1.0f) / 2.0f, (sin(glfwGetTime() * 2) + 1.0f) / 2.0f, (sin(glfwGetTime() * 3) + 1.0f) / 2.0f, -1.0f);
    glUniform4fv(glGetUniformLocation(shader.ID, "overrideColor"), 1, glm::value_ptr(overrideColor));
    renderer.draw(6, 6);

    window.render();
  }

  window.close();
  return 0;
}

void processInput(GLFWwindow *window, Shader shader, float *movX, float *movY, float deltaTime)
{
  float speed = 5.0f;
  speed *= deltaTime;
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    *movY += speed;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    *movY -= speed;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    *movX -= speed;
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    *movX += speed;
  }
}
