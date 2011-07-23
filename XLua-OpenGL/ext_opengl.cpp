#include "ext_opengl.h"

#include "LuaGL.h"

static int ext_opengl_get_current_shader (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTOPENGL* ogl = (EXTOPENGL*) xlua_get_object(L, objid);

	if (!ogl || ogl->rHo.hoIdentifier != OBJID) {
		return 0;
	}

	lua_pushinteger(L, ogl->glslVar);

	return 1;
}

static int ext_opengl_get_display_list (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	int listid = lua_tointeger(L, 2);

	EXTOPENGL* ogl = (EXTOPENGL*) xlua_get_object(L, objid);

	if (listid < 0 || listid >= ogl->maxDsp) {
		return 0;
	}

	if (!ogl || ogl->rHo.hoIdentifier != OBJID) {
		return 0;
	}

	lua_pushinteger(L, ogl->dspListStart + listid);

	return 1;
}

static int ext_opengl_get_shader (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	int glid = lua_tointeger(L, 2);

	EXTOPENGL* ogl = (EXTOPENGL*) xlua_get_object(L, objid);

	if (glid < 0 || glid >= ogl->maxSha) {
		return 0;
	}

	if (!ogl || ogl->rHo.hoIdentifier != OBJID) {
		return 0;
	}

	lua_pushinteger(L, ogl->glsl[glid]);

	return 1;
}

static int ext_opengl_get_texture (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	int texid = lua_tointeger(L, 2);

	EXTOPENGL* ogl = (EXTOPENGL*) xlua_get_object(L, objid);

	if (texid < 0 || texid >= ogl->maxTex) {
		return 0;
	}

	if (!ogl || ogl->rHo.hoIdentifier != OBJID) {
		return 0;
	}

	lua_pushinteger(L, ogl->textureID[texid]);

	return 1;
}

static int ext_opengl_make_current (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTOPENGL* ogl = (EXTOPENGL*) xlua_get_object(L, objid);
	if (!ogl || ogl->rHo.hoIdentifier != OBJID) {
		return 0;
	}

	wglMakeCurrent(ogl->hDC, ogl->hRC);

	return 0;
}

static int ext_opengl_max_display_lists (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTOPENGL* ogl = (EXTOPENGL*) xlua_get_object(L, objid);

	if (!ogl || ogl->rHo.hoIdentifier != OBJID) {
		return 0;
	}

	lua_pushinteger(L, ogl->maxDsp);

	return 1;
}

static int ext_opengl_max_shaders (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTOPENGL* ogl = (EXTOPENGL*) xlua_get_object(L, objid);

	if (!ogl || ogl->rHo.hoIdentifier != OBJID) {
		return 0;
	}

	lua_pushinteger(L, ogl->maxSha);

	return 1;
}

static int ext_opengl_max_textures (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTOPENGL* ogl = (EXTOPENGL*) xlua_get_object(L, objid);

	if (!ogl || ogl->rHo.hoIdentifier != OBJID) {
		return 0;
	}

	lua_pushinteger(L, ogl->maxTex);

	return 1;
}

static const luaL_reg ext_opengllib[] = {
	{"GetCurrentShader", ext_opengl_get_current_shader},
	{"GetDisplayList", ext_opengl_get_display_list},
	{"GetGLSL", ext_opengl_get_shader},					// Deprecated, use GetShader
	{"GetShader", ext_opengl_get_shader},
	{"GetTexture", ext_opengl_get_texture},
	{"MakeCurrent", ext_opengl_make_current},
	{"MaxDisplayLists", ext_opengl_max_display_lists},
	{"MaxShaders", ext_opengl_max_shaders},
	{"MaxTextures", ext_opengl_max_textures},
	{NULL, NULL}
};

extern "C" const luaL_reg gllib[];
extern "C" const luaL_reg glulib[];

extern "C" __declspec(dllexport) int luaopen_ext_opengl (lua_State *L) {
	// Register LuaGL library as a by-product of registering ext.opengl
	luaL_register(L, "gl", gllib);

	// Load GL enumerations
	lua_getglobal(L, "gl");
	int i = 0;

	while (gl_str[i].str != 0) {
		lua_pushinteger(L, gl_str[i].value);
		lua_setfield(L, -2, gl_str[i].str);

		i++;
	}

	// Register GLU library as well
	luaL_register(L, "glu", glulib);

	lua_getglobal(L, "glu");
	i = 0;

	while (glu_str[i].str != 0) {
		lua_pushinteger(L, gl_str[i].value);
		lua_setfield(L, -2, gl_str[i].str);

		i++;
	}

	// Create the ext table if it is absent
	lua_getglobal(L, "ext");
	if (!lua_istable(L, -1)) {
		lua_newtable(L);
		lua_setglobal(L, "ext");
		lua_getglobal(L, "ext");
	}

	// Load the opengl library into ext.opengl
	lua_newtable(L);
	lua_setfield(L, -2, "opengl");
	lua_getfield(L, -1, "opengl");
	luaL_register(L, NULL, ext_opengllib);

	return 1;
}
