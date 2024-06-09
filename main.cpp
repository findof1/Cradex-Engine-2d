#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

void processInput(GLFWwindow *window, float triangle1[], unsigned int *VBOs, size_t triangle1Size);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "uniform vec4 ourColor;"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = ourColor;\n"
                                   "}\n\0";

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(800, 600, "Cradex Engine", NULL, NULL);
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

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float triangle1[] = {
      0.75f,
      0.0f,
      0.0f,
      0.25f,
      0.0f,
      0.0f,
      0.5f,
      0.5f,
      0.0f,

  };

  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3};

  unsigned int EBO;

  unsigned int VBOs[1];
  unsigned int VAOs[1];

  glGenVertexArrays(2, VAOs);
  glGenBuffers(2, VBOs);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAOs[0]);

  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window, triangle1, VBOs, sizeof(triangle1));

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, 0.5f, 0.0f, 1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_DYNAMIC_DRAW);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
void processInput(GLFWwindow *window, float triangle1[], unsigned int *VBOs, size_t triangle1Size)
{
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    for (int i = 0; i < 9; i += 3)
    {
      triangle1[i] += 0.0001f;
      glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
      glBufferData(GL_ARRAY_BUFFER, triangle1Size, triangle1, GL_DYNAMIC_DRAW);
    }
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    for (int i = 0; i < 9; i += 3)
    {
      triangle1[i] -= 0.0001f;
      glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
      glBufferData(GL_ARRAY_BUFFER, triangle1Size, triangle1, GL_DYNAMIC_DRAW);
    }
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}