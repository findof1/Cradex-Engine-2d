#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
class Window
{
public:
  GLFWwindow *window;

  Window()
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
    window = glfwCreateWindow(vidmode->width, vidmode->height, "Cradex Engine", NULL, NULL);
    if (window == NULL)
    {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    // wireframe setting
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // fill setting
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
  }

  void clear() const
  {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void render() const
  {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  void close() const
  {
    glfwTerminate();
  }

private:
  static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
  {
    glViewport(0, 0, width, height);
  }
};
#endif
