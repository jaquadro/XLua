#ifndef EXT_OPENGL_H_
#define EXT_OPENGL_H_

#include "common.h"

static LuaManager2* get_manager (LuaState* state) {
	for (LuaState::IRdList i_rd = state->rdList.begin(); i_rd != state->rdList.end(); i_rd++) {
		if ((*i_rd)->extOpenGL == NULL) {
			continue;
		}
		return *i_rd;
	}

	return NULL;
}

static int ext_opengl_get_texture (lua_State* L) {
	LuaState* state = LGLOB->GetStateByState(L);

	// Get param
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int texid = lua_tointeger(L, 1);
	if (texid < 0 || texid >= 256) {
		return 0;
	}

	LuaManager2* lman = get_manager(state);
	if (!lman) {
		return 0;
	}

	lua_pushinteger(L, lman->extOpenGL->textureID[texid]);

	return 1;
}

static int ext_opengl_get_glsl (lua_State* L) {
	LuaState* state = LGLOB->GetStateByState(L);

	// Get param
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int gid = lua_tointeger(L, 1);
	if (gid < 0 || gid >= 256) {
		return 0;
	}

	LuaManager2* lman = get_manager(state);
	if (!lman) {
		return 0;
	}

	lua_pushinteger(L, lman->extOpenGL->glsl[gid]);

	return 1;
}

static const luaL_reg ext_opengllib[] = {
	{"GetTexture", ext_opengl_get_texture},
	{"GetGLSL", ext_opengl_get_glsl},
	{NULL, NULL}
};

LUALIB_API __declspec(dllexport) int luaopen_ext_opengl (lua_State *L) {
  luaL_register(L, "opengl", ext_opengllib);
  return 1;
}

#endif