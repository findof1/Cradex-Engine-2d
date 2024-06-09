#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void processInput(GLFWwindow *window, Shader shader);
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

  Shader shader("./Shaders/vertexShader.vs", "./Shaders/fragmentShader.fs");

  // clang-format off

float vertices[] = {
0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

  // clang-format on

  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3};

  unsigned int VBOs[1];
  unsigned int VAOs[1];
  unsigned int EBO;

  glGenVertexArrays(1, VAOs);
  glGenBuffers(1, VBOs);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAOs[0]);

  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  unsigned int texture1, texture2;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width, height, nrChannels;
  unsigned char *data = stbi_load("wall.jpg", &width, &height,
                                  &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load("awesomeface.png", &width, &height,
                   &nrChannels, 0);
  stbi_set_flip_vertically_on_load(false);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);

  shader.use();

  shader.setInt("texture1", 0);

  shader.setInt("texture2", 1);

  shader.setFloat("mixVal", 0.5);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window, shader);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader.use();
    glBindVertexArray(VAOs[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
void processInput(GLFWwindow *window, Shader shader)
{
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    int mixValLocation = glGetUniformLocation(shader.ID, "mixVal");
    GLfloat mixVal;
    glGetUniformfv(shader.ID, mixValLocation, &mixVal);
    shader.setFloat("mixVal", mixVal + 0.005);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    int mixValLocation = glGetUniformLocation(shader.ID, "mixVal");
    GLfloat mixVal;
    glGetUniformfv(shader.ID, mixValLocation, &mixVal);
    shader.setFloat("mixVal", mixVal - 0.005);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}