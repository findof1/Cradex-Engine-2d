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

void processInput(GLFWwindow *window, Shader shader, float *movX, float *movY, float deltaTime);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
  GLFWwindow *window = glfwCreateWindow(vidmode->width, vidmode->height, "Cradex Engine", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  // wireframe setting
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // fill setting
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  Shader shader("./Shaders/vertexShader.vs", "./Shaders/fragmentShader.fs");

  // clang-format off

  // clang-format on

  Texture texture1("wall.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
  // Texture texture2("awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);

  Renderer renderer({}, {});
  renderer.addRectangle({-0.5f, -0.5f, -0.5f}, {2.0f, 2.0f}, {0.5f, 0.0f, 0.0f});
  renderer.addRectangle({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f});
  shader.use();

  shader.setInt("textureActive", 1);

  shader.setInt("texture1", 0);

  shader.setInt("mixVal", 1.0f);

  glm::mat4 trans = glm::mat4(1.0f);
  unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");

  float movX = 0;
  float movY = 0;

  double lastFrame = glfwGetTime();
  while (!glfwWindowShouldClose(window))
  {
    double currentFrame = glfwGetTime();
    float deltaTime = static_cast<float>(currentFrame - lastFrame);
    lastFrame = currentFrame;

    processInput(window, shader, &movX, &movY, deltaTime);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    texture1.Bind();

    trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(0.2f, 0.35f, 0.0f));
    transformLoc = glGetUniformLocation(shader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    shader.setInt("textureActive", 1);
    renderer.draw(0, 6);
    shader.setInt("textureActive", 0);
    trans = glm::translate(trans, glm::vec3(movX, movY, 0.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    renderer.draw(6, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}