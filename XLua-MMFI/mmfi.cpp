#include "common.h"
#include "type.h"
#include "object.h"
#include "class.h"
#include "frame.h"
#include "runtime.h"
#include "window.h"
#include "input.h"

#include "xsurface.h"

void PushStandardClosure (lua_State* L, lua_CFunction func) {
	lua_pushstring(L, KEY_POOL_OBJECT);
	lua_rawget(L, LUA_REGISTRYINDEX);
	lua_pushstring(L, KEY_POOL_CLASS);
	lua_rawget(L, LUA_REGISTRYINDEX);

	lua_pushcclosure(L, func, 2);
}

extern "C" __declspec(dllexport) int luaopen_mmfi (lua_State *L) {

	// Registry
	lua_pushstring(L, KEY_POOL_OBJECT);
	lua_createtable(L, 0, 0);
	lua_rawset(L, LUA_REGISTRYINDEX);

	lua_pushstring(L, KEY_POOL_CLASS);
	lua_createtable(L, 0, 0);
	lua_rawset(L, LUA_REGISTRYINDEX);

	lua_pushstring(L, KEY_TYPE_REGISTRY);
	lua_createtable(L, 0, 0);
	lua_rawset(L, LUA_REGISTRYINDEX);

	// Register initial types
	lua_pushstring(L, KEY_TYPE_REGISTRY);
	lua_rawget(L, LUA_REGISTRYINDEX);

	for (int i = 0; ; i++) {
		if (typeList[i].id == 0)
			break;

		lua_pushinteger(L, typeList[i].id);
		lua_createtable(L, 2, 0);
		lua_rawset(L, -3);

		lua_pushinteger(L, typeList[i].id);
		lua_rawget(L, -2);

		lua_pushlightuserdata(L, typeList[i].index);
		lua_rawseti(L, -2, 1);
		lua_pushlightuserdata(L, typeList[i].newIndex);
		lua_rawseti(L, -2, 2);

		lua_pop(L, 1);
	}

	lua_pop(L, 1);

	// New* Functions
	//PushStandardClosure(L, Object::NewObject);
	//lua_setglobal(L, "NewObject");

	//PushStandardClosure(L, ObjectClass::NewObjectClass);
	//lua_setglobal(L, "NewObjectClass");

	// MMFI
	lua_newtable(L);
	lua_setglobal(L, "mmf");
	lua_getglobal(L, "mmf");

	Frame::NewFrame(L);
	lua_setfield(L, -2, "frame");

	Runtime::NewRuntime(L);
	lua_setfield(L, -2, "runtime");

	Window::NewWindow(L);
	lua_setfield(L, -2, "window");

	Mouse::NewMouse(L);
	lua_setfield(L, -2, "mouse");

	Keyboard::NewKeyboard(L);
	lua_setfield(L, -2, "keyboard");

	PushStandardClosure(L, Object::NewObject);
	lua_setfield(L, -2, "newObject");

	PushStandardClosure(L, ObjectClass::NewObjectClass);
	lua_setfield(L, -2, "newObjectClass");

	// Enums
	for (int i = 0; ; i++) {
		if (mmfiEnum[i].name == 0)
			break;

		lua_pushinteger(L, mmfiEnum[i].iVal);
		lua_setfield(L, -2, mmfiEnum[i].name);
	}

	lua_pop(L, 1);

	XSurface::Register(L);

	return 0;
}
