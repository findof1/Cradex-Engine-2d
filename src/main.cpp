#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Classes/shader.h"
#include "Classes/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "External/stb_image.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "Classes/renderer.h"
#include "Classes/window.h"
#include "Classes/gameObject.h"
#include "Scripting/luaSupport.h"
#include <filesystem>
#include <unordered_set>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
std::unordered_set<int> pressedKeys;
void processInput(GLFWwindow *window, float deltaTime, lua_State *L);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
int main()
{
  Window window;
  Shader shader("./Shaders/vertexShader.vs", "./Shaders/fragmentShader.fs");
  Renderer renderer;

  Texture texture1("./Textures/wall.jpg", GL_TEXTURE0);
  Texture texture2("./Textures/awesomeface.png", GL_TEXTURE1);

  float movX = 0;
  float movY = 0;
  float velocityX = 0;
  float velocityY = 0;

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  lua_pushlightuserdata(L, &renderer);
  lua_pushlightuserdata(L, &window);
  lua_pushcclosure(L, createGameObject, 2);
  lua_setglobal(L, "createGameObject");

  lua_pushcclosure(L, setGameObjectPosition, 0);
  lua_setglobal(L, "setGameObjectPosition");

  lua_pushcclosure(L, setGameObjectScale, 0);
  lua_setglobal(L, "setGameObjectScale");

  lua_pushcclosure(L, setGameObjectRotation, 0);
  lua_setglobal(L, "setGameObjectRotation");

  lua_pushcclosure(L, getGameObjectRotation, 0);
  lua_setglobal(L, "getGameObjectRotation");

  lua_pushcclosure(L, getGameObjectPosition, 0);
  lua_setglobal(L, "getGameObjectPosition");

  lua_pushcclosure(L, getGameObjectScale, 0);
  lua_setglobal(L, "getGameObjectScale");

  lua_pushlightuserdata(L, &shader);
  lua_pushcclosure(L, drawGameObject, 1);
  lua_setglobal(L, "drawGameObject");

  int result = luaL_dofile(L, "script.lua");

  if (result != LUA_OK)
  {
    const char *errorMsg = lua_tostring(L, -1);
    std::cerr << "Error loading Lua script: " << errorMsg << std::endl;
    lua_pop(L, 1);
  }

  glfwSetKeyCallback(window.window, key_callback);

  double lastFrame = glfwGetTime();

  while (!glfwWindowShouldClose(window.window))
  {
    double currentFrame = glfwGetTime();
    float deltaTime = static_cast<float>(currentFrame - lastFrame);
    lastFrame = currentFrame;

    processInput(window.window, deltaTime, L);

    movX += velocityX;
    movY += velocityY;
    velocityX *= 0.9;
    velocityY *= 0.9;

    window.clear();
    shader.use();
    texture1.Bind();
    texture2.Bind();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    // glm::vec4 overrideColor((sin(glfwGetTime()) + 1.0f) / 2.0f, (sin(glfwGetTime() * 2) + 1.0f) / 2.0f, (sin(glfwGetTime() * 3) + 1.0f) / 2.0f, 1.0f);
    // shader.setFloatVec4("overrideColor", overrideColor);

    glm::vec4 overrideColor(1.0f, 1.0f, 1.0f, 1.0f);
    shader.setFloatVec4("overrideColor", overrideColor);
    lua_getglobal(L, "update");
    lua_pushnumber(L, deltaTime);
    lua_call(L, 1, 0);

    window.render();
  }
  lua_close(L);
  window.close();
  return 0;
}

void processInput(GLFWwindow *window, float deltaTime, lua_State *L)
{
  lua_newtable(L);

  int i = 1;
  for (int key : pressedKeys)
  {
    lua_pushinteger(L, i++);
    lua_pushinteger(L, key);
    lua_settable(L, -3);
  }

  lua_setglobal(L, "pressedKeys");
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS)
  {
    pressedKeys.insert(key);
  }
  else if (action == GLFW_RELEASE)
  {
    pressedKeys.erase(key);
  }
}
