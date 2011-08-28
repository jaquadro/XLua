#include "dll_static.h"

static int dll_arraylib_new (lua_State* L) {
	int argc = lua_gettop(L);
	if (argc < 1) {
		return 0;
	}

	int type = DATA_DOUBLE;
	const char* param = 0;

	// Figure out how many dimensions we're requesting
	int dims = argc;
	if (lua_isstring(L, argc)) {
		dims--;
		param = lua_tostring(L, argc);
	}

	if (dims <= 0)
		return 0;

	// Verify dimensions are numeric and calulate count
	int count = 1;
	int* dimary = (int*)malloc(dims * sizeof(int));

	for (int i = 1; i <= dims; i++) {
		if (!lua_isnumber(L, i))
			return 0;
		dimary[i - 1] = lua_tointeger(L, i);
		count *= dimary[i - 1];
	}

	// Verify count is valid
	if (count <= 0)
		return 0;

	// Be generous about what we accept for the type parameter
	if (param != 0) {
		if (strcmp(param, "byte") == 0 || strcmp(param, "char") == 0) {
			type = DATA_BYTE;
		}
		else if (strcmp(param, "short") == 0) {
			type = DATA_SHORT;
		}
		else if (strcmp(param, "long") == 0 || strcmp(param, "int") == 0) {
			type = DATA_LONG;
		}
		else if (strcmp(param, "float") == 0) {
			type = DATA_FLOAT;
		}
	}

	ArrayUD* aud = (ArrayUD*)lua_newuserdata(L, sizeof(ArrayUD));
	aud->arr = new StaticArray(type, dims, dimary);

	// If a ridiculous request was made, we might fail
	if (aud->arr->error()) {
		delete aud->arr;

		lua_pushstring(L, "Could not allocate memory for requested size");
		lua_error(L);
		return 0;
	}

	luaL_getmetatable(L, "lib.array");
	lua_setmetatable(L, -2);

	return 1;
}

static int dll_arraylib_clone (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 1, "lib.array");

	if (!aud->arr) {
		return 0;
	}

	ArrayUD* newaud = (ArrayUD*)lua_newuserdata(L, sizeof(ArrayUD));
	newaud->arr = new StaticArray(*aud->arr);

	// If a ridiculous request was made, we might fail
	if (aud->arr->error()) {
		delete newaud->arr;

		lua_pushstring(L, "Could not allocate memory for copy");
		lua_error(L);
		return 0;
	}

	luaL_getmetatable(L, "lib.array");
	lua_setmetatable(L, -2);

	return 1;
}

static int dll_arraylib_tostring (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 1, "lib.array");

	if (!aud->arr) {
		return 0;
	}

	lua_pushfstring(L, "array(%d)", aud->arr->count());
	return 1;
}

static int dll_arraylib_size (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 1, "lib.array");

	if (!aud->arr) {
		return 0;
	}

	lua_pushinteger(L, aud->arr->count());
	return 1;
}

static int dll_arraylib_dimensions (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 1, "lib.array");

	if (!aud->arr) {
		return 0;
	}

	lua_pushinteger(L, aud->arr->dimCount());
	return 1;
}

static int dll_arraylib_dimsize (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 1, "lib.array");
	int idx = luaL_checkint(L, 2);

	if (!aud->arr) {
		return 0;
	}

	lua_pushinteger(L, aud->arr->dimSize(idx - 1));
	return 1;
}

/**
 * xx.at(self, index)
 * ? = xx[y]
 */
static int dll_arraylib_at (lua_State* L, ArrayUD* aud) {
	int index = luaL_checkint(L, 2) - 1;
	if (!aud->arr) {
		return 0;
	}

	luaL_argcheck(L, index >= 0 && index < aud->arr->count(), 2, "index out of range");

	int type = aud->arr->type();

	switch (type) {
		case DATA_DOUBLE: lua_pushnumber(L, aud->arr->getDouble(index)); break;
		case DATA_LONG: lua_pushinteger(L, aud->arr->getLong(index)); break;
		case DATA_SHORT: lua_pushinteger(L, aud->arr->getShort(index)); break;
		case DATA_BYTE: lua_pushinteger(L, aud->arr->getByte(index)); break;
		case DATA_FLOAT: lua_pushnumber(L, aud->arr->getFloat(index)); break;
		default: return 0;
	}

	return 1;
}

/**
 * xx.at(self, index1, index2, ... indexn)
 */
static int dll_arraylib_multiat (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 1, "lib.array");
	int dimCnt = lua_gettop(L) - 1;

	if (!aud->arr) {
		return 0;
	}

	if (dimCnt != aud->arr->dimCount()) {
		lua_pushstring(L, "Invalid argument count");
		lua_error(L);
	}

	int type = aud->arr->type();

	int* args = (int*)malloc(dimCnt * sizeof(int));
	for (int i = 0; i < dimCnt; i++)
		args[i] = lua_tointeger(L, i + 2) - 1;

	switch (type) {
		case DATA_DOUBLE: lua_pushnumber(L, aud->arr->getDouble(args)); break;
		case DATA_LONG: lua_pushinteger(L, aud->arr->getLong(args)); break;
		case DATA_SHORT: lua_pushinteger(L, aud->arr->getShort(args)); break;
		case DATA_BYTE: lua_pushinteger(L, aud->arr->getByte(args)); break;
		case DATA_FLOAT: lua_pushnumber(L, aud->arr->getFloat(args)); break;
		default: 
			free(args);
			return 0;
	}

	free(args);
	return 1;
}

static int dll_arraylib_staticat (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)lua_touserdata(L, 1);
	return dll_arraylib_at(L, aud);
}

/**
 * xx.set(self, index, value)
 * xx[y] = ?
 */
static int dll_arraylib_set (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)lua_touserdata(L, 1); //luaL_checkudata(L, 1, "lib.array");
	if (!aud->arr) {
		return 0;
	}

	int index = luaL_checkint(L, 2) - 1;
	luaL_argcheck(L, index >= 0 && index < aud->arr->count(), 2, "index out of range");

	luaL_checkany(L, 3);

	int type = aud->arr->type();

	switch (type) {
		case DATA_DOUBLE: aud->arr->setDouble(index, lua_tonumber(L, 3)); break;
		case DATA_LONG: aud->arr->setLong(index, lua_tointeger(L, 3)); break;
		case DATA_SHORT: aud->arr->setShort(index, (short)lua_tointeger(L, 3)); break;
		case DATA_BYTE: aud->arr->setChar(index, (char)lua_tointeger(L, 3)); break;
		case DATA_FLOAT: aud->arr->setFloat(index, (float)lua_tonumber(L, 3)); break;
	}

	return 0;
}

/**
 * xx.set(self, index1, index2, ... indexn, value)
 */
static int dll_arraylib_multiset (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 1, "lib.array");

	int valIndex = lua_gettop(L);
	int dimCnt = valIndex - 2;

	if (!aud->arr) {
		return 0;
	}

	luaL_checkany(L, valIndex);

	if (dimCnt != aud->arr->dimCount()) {
		lua_pushstring(L, "Invalid argument count");
		lua_error(L);
	}

	int type = aud->arr->type();

	int* args = (int*)malloc(dimCnt * sizeof(int));
	for (int i = 0; i < dimCnt; i++)
		args[i] = lua_tointeger(L, i + 2) - 1;

	switch (type) {
		case DATA_DOUBLE: aud->arr->setDouble(args, lua_tonumber(L, valIndex)); break;
		case DATA_LONG: aud->arr->setLong(args, lua_tointeger(L, valIndex)); break;
		case DATA_SHORT: aud->arr->setShort(args, (short)lua_tointeger(L, valIndex)); break;
		case DATA_BYTE: aud->arr->setChar(args, (char)lua_tointeger(L, valIndex)); break;
		case DATA_FLOAT: aud->arr->setFloat(args, (float)lua_tonumber(L, valIndex)); break;
	}

	free(args);
	return 0;
}

static int dll_arraylib_staticset (lua_State* L) {
	return dll_arraylib_set(L);
}

/**
 * xx.reset(self)
 */
static int dll_arraylib_reset (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 1, "lib.array");

	if (!aud->arr) {
		return 0;
	}

	aud->arr->reset();

	return 0;
}

/**
 * xx.resize(self)
 */
static int dll_arraylib_resize (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 1, "lib.array");
	//int size = luaL_checkint(L, 2);
	int dimCnt = lua_gettop(L) - 1;

	if (!aud->arr) {
		return 0;
	}

	int* args = (int*)malloc(dimCnt * sizeof(int));
	for (int i = 0; i < dimCnt; i++) {
		args[i] = lua_tointeger(L, i + 2);
		if (args[i] <= 0)
			return 0;
	}

	aud->arr->resize(dimCnt, args);

	// If a ridiculous request was made, we might fail
	if (aud->arr->error()) {
		aud->arr->clearError();

		lua_pushstring(L, "Could not allocate memory for requested size");
		lua_error(L);
	}

	return 0;
}

/**
 * xx.destroy(self)
 */
static int dll_arraylib_destroy (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 1, "lib.array");

	if (!aud->arr) {
		return 0;
	}

	delete aud->arr;
	aud->arr = 0;

	return 0;
}

static int dll_arraylib_index (lua_State* L) {
	ArrayUD* aud = (ArrayUD*)lua_touserdata(L, 1); //luaL_checkudata(L, 1, "lib.array");

	if (lua_isnumber(L, 2)) {
		return dll_arraylib_at(L, aud);
	}

	const char* key = lua_tostring(L, 2); //luaL_checkstring(L, 2);

	if (strcmp(key, "at") == 0) {
		lua_pushcfunction(L, dll_arraylib_multiat);
		return 1;
	}
	else if (strcmp(key, "set") == 0) {
		lua_pushcfunction(L, dll_arraylib_multiset);
		return 1;
	}
	else if (strcmp(key, "reset") == 0) {
		lua_pushcfunction(L, dll_arraylib_reset);
		return 1;
	}
	else if (strcmp(key, "resize") == 0) {
		lua_pushcfunction(L, dll_arraylib_resize);
		return 1;
	}
	else if (strcmp(key, "size") == 0) {
		lua_pushcfunction(L, dll_arraylib_size);
		return 1;
	}
	else if (strcmp(key, "dimSize") == 0) {
		lua_pushcfunction(L, dll_arraylib_dimsize);
		return 1;
	}
	else if (strcmp(key, "dimensions") == 0) {
		lua_pushcfunction(L, dll_arraylib_dimensions);
		return 1;
	}
	else if (strcmp(key, "destroy") == 0) {
		lua_pushcfunction(L, dll_arraylib_destroy);
		return 1;
	}
	else if (strcmp(key, "clone") == 0) {
		lua_pushcfunction(L, dll_arraylib_clone);
		return 1;
	}

	return 0;
}

static const struct luaL_Reg arraylib_f[] = {
	{"new", dll_arraylib_new},
	{"at", dll_arraylib_staticat},
	{"set", dll_arraylib_staticset},
	{NULL, NULL}
};

static const struct luaL_Reg arraylib_m[] = {
	{"__newindex", dll_arraylib_set},
	{"__index", dll_arraylib_index},
	{"__len", dll_arraylib_size},
	{"__tostring", dll_arraylib_tostring},
	{"__gc", dll_arraylib_destroy},
	{NULL, NULL}
};

// The name of this routine is critical to the lua package system.
// require "super.sub.lib" would translate to a function named luaopen_super_sub_lib.
// The package would be opened in the table super.sub.lib.

// package array
extern "C" __declspec(dllexport) int luaopen_array (lua_State *L) {
	luaL_newmetatable(L, "lib.array");
	luaL_register(L, NULL, arraylib_m);
	luaL_register(L, "array", arraylib_f);
  return 0;
}
