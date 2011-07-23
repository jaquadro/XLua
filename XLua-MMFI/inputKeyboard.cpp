#include "input.h"
#include "collisions.h"

int Keyboard::IndexMetamethod (lua_State* L) {
	if (lua_tointeger(L, lua_upvalueindex(UV_TYPE)) != TYPE_STATIC)
		return 0;

	const char* key = lua_tostring(L, 2);

	int ret = StandardIndex(L, KeyboardRead, KeyboardWrite);
	if (ret > -1) return ret;

	return 0;
}

int Keyboard::NewIndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_STATIC)
		return 0;

	const char* key = lua_tostring(L, 2);

	int ret = StandardNewIndex(L, KeyboardRead, KeyboardWrite);
	if (ret > -1) return ret;

	return 0;
}

int Keyboard::MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc) {
	return ::MemoClosure(L, key, lfunc, 2);
}

int Keyboard::NewKeyboard (lua_State* L) {
	LPRH rh = xlua_get_run_header(L);

	int base = lua_gettop(L);								// +0
	if (base <= 0)
		return 0;

	lua_checkstack(L, base + 10);

	int (*index_method)(lua_State*L) = Keyboard::IndexMetamethod;
	int (*newindex_method)(lua_State*L) = Keyboard::NewIndexMetamethod;

	// Create object table
	lua_createtable(L, 0, 0);								// +1 = Object Table

	// Create object metatable
	lua_createtable(L, 0, 2);								// +2 = Object Metatable

	// Add __index
	lua_pushnumber(L, TYPE_STATIC);							// +3
	lua_pushlightuserdata(L, (void*)rh);					// +4
	lua_pushcclosure(L, index_method, 2);					// +3
	lua_setfield(L, -2, "__index");							// +2

	// Add __newindex
	lua_pushnumber(L, TYPE_OBJECT);							// +3
	lua_pushlightuserdata(L, (void*)rh);					// +4
	lua_pushcclosure(L, newindex_method, 2);				// +3
	lua_setfield(L, -2, "__newindex");						// +2

	// Add __metatable
	lua_pushstring(L, "Access violation");					// +3
	lua_setfield(L, -2, "__metatable");						// +2
	
	// Bind metatable
	lua_setmetatable(L, -2);								// +1

	return 1;
}

// -----

int Keyboard::KeyDown (lua_State* L) {
	return Keyboard::MemoClosure(L, lua_tostring(L, 2), Keyboard::KeyDownFunc);
}

int Keyboard::KeyDownFunc (lua_State* L) {
	if (!lua_isnumber(L, 1))
		return 0;

	int key = lua_tointeger(L, 1);
	short state = GetAsyncKeyState(key);

	if (state & 0x8000) {
		lua_pushboolean(L, true);
	} else {
		lua_pushboolean(L, false);
	}

	return 1;
}

int Keyboard::KeyUp (lua_State* L) {
	return Keyboard::MemoClosure(L, lua_tostring(L, 2), Keyboard::KeyUpFunc);
}

int Keyboard::KeyUpFunc (lua_State* L) {
	if (!lua_isnumber(L, 1))
		return 0;

	int key = lua_tointeger(L, 1);
	short state = GetAsyncKeyState(key);

	if (state & 0x8000) {
		lua_pushboolean(L, false);
	} else {
		lua_pushboolean(L, true);
	}

	return 1;
}

int Keyboard::SimulateKeyDown (lua_State* L) {
	return Keyboard::MemoClosure(L, lua_tostring(L, 2), Keyboard::SimulateKeyDownFunc);
}

int Keyboard::SimulateKeyDownFunc (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh)
		return 0;

	if (GetActiveWindow() != rh->rhHTopLevelWnd)
		return 0;

	if (!lua_isnumber(L, 1))
		return 0;

	int key = lua_tointeger(L, 1);
	short state = GetAsyncKeyState(key);

	// Don't force a key down if it's already down
	if (state & 0x8000) {
		return 0;
	}

	// Forbiddon keys
	switch (key) {
		case 0x12:
		case 0xA4:
		case 0xA5:
			return 0;
	}

	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
	input.ki.dwFlags = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	lua_pushboolean(L, SendInput(1, &input, sizeof(INPUT)));
	return 1;
}

int Keyboard::SimulateKeyUp (lua_State* L) {
	return Keyboard::MemoClosure(L, lua_tostring(L, 2), Keyboard::SimulateKeyUpFunc);
}

int Keyboard::SimulateKeyUpFunc (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh)
		return 0;

	if (GetActiveWindow() != rh->rhHTopLevelWnd)
		return 0;

	if (!lua_isnumber(L, 1))
		return 0;

	int key = lua_tointeger(L, 1);
	short state = GetAsyncKeyState(key);

	// Don't force a key up if it's already up
	if (!(state & 0x8000)) {
		return 0;
	}

	// Forbiddon keys
	switch (key) {
		case 0x12:
		case 0xA4:
		case 0xA5:
			return 0;
	}

	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	lua_pushboolean(L, SendInput(1, &input, sizeof(INPUT)));
	return 1;
}
