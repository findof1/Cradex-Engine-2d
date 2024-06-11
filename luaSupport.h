#include "gameObject.h"
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

int createGameObject(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  Window *window = (Window *)lua_touserdata(L, lua_upvalueindex(2));

  int type = luaL_optinteger(L, 1, 1);
  float posX = luaL_optnumber(L, 2, 0.0f);
  float posY = luaL_optnumber(L, 3, 0.0f);
  float rotation = luaL_optnumber(L, 4, 0.0f);
  float scaleX = luaL_optnumber(L, 5, 1.0f);
  float scaleY = luaL_optnumber(L, 6, 1.0f);
  int textureIndex = luaL_optinteger(L, 7, -1);

  GameObject *gameObject = new GameObject(*renderer, *window, type, {posX, posY}, rotation, {scaleX, scaleY}, textureIndex);

  lua_pushlightuserdata(L, gameObject);

  return 1;
}

int setGameObjectPosition(lua_State *L)
{
  GameObject *gameObject = (GameObject *)lua_touserdata(L, 1);

  float posX = lua_tonumber(L, 2);
  float posY = lua_tonumber(L, 3);

  gameObject->position = {posX, posY};

  return 0;
}

int setGameObjectScale(lua_State *L)
{
  GameObject *gameObject = (GameObject *)lua_touserdata(L, 1);

  float scaleX = lua_tonumber(L, 2);
  float scaleY = lua_tonumber(L, 3);

  gameObject->scale = {scaleX, scaleY};

  return 0;
}

int setGameObjectRotation(lua_State *L)
{
  GameObject *gameObject = (GameObject *)lua_touserdata(L, 1);

  float rotation = lua_tonumber(L, 2);

  gameObject->rotation = rotation;

  return 0;
}

int getGameObjectRotation(lua_State *L)
{
  GameObject *gameObject = (GameObject *)lua_touserdata(L, 1);

  lua_pushnumber(L, gameObject->rotation);

  return 1;
}

int getGameObjectPosition(lua_State *L)
{
  GameObject *gameObject = (GameObject *)lua_touserdata(L, 1);

  lua_pushnumber(L, gameObject->position.x);
  lua_pushnumber(L, gameObject->position.y);

  return 2;
}

int getGameObjectScale(lua_State *L)
{
  GameObject *gameObject = (GameObject *)lua_touserdata(L, 1);

  lua_pushnumber(L, gameObject->scale.x);
  lua_pushnumber(L, gameObject->scale.y);

  return 2;
}

int drawGameObject(lua_State *L)
{
  GameObject *gameObject = (GameObject *)lua_touserdata(L, 1);

  Shader *shader = (Shader *)lua_touserdata(L, lua_upvalueindex(1));

  gameObject->draw(*shader);

  return 0;
}