#include "input.h"
#include "collisions.h"

int Mouse::IndexMetamethod (lua_State* L) {
	if (lua_tointeger(L, lua_upvalueindex(UV_TYPE)) != TYPE_STATIC)
		return 0;

	const char* key = lua_tostring(L, 2);

	int ret = StandardIndex(L, MouseRead, MouseWrite);
	if (ret > -1) return ret;

	return 0;
}

int Mouse::NewIndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_STATIC)
		return 0;

	const char* key = lua_tostring(L, 2);

	int ret = StandardNewIndex(L, MouseRead, MouseWrite);
	if (ret > -1) return ret;

	return 0;
}

int Mouse::MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc) {
	return ::MemoClosure(L, key, lfunc, 2);
}

int Mouse::NewMouse (lua_State* L) {
	LPRH rh = xlua_get_run_header(L);

	int base = lua_gettop(L);								// +0
	if (base <= 0)
		return 0;

	lua_checkstack(L, base + 10);

	int (*index_method)(lua_State*L) = Mouse::IndexMetamethod;
	int (*newindex_method)(lua_State*L) = Mouse::NewIndexMetamethod;

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

int Mouse::ButtonDown (lua_State* L) {
	return Mouse::MemoClosure(L, lua_tostring(L, 2), Mouse::ButtonDownFunc);
}

int Mouse::ButtonDownFunc (lua_State* L) {
	if (!lua_isnumber(L, 1))
		return 0;

	short state = 0;
	switch (lua_tointeger(L, 1)) {
		case 1: state = GetAsyncKeyState(VK_LBUTTON); break;
		case 2: state = GetAsyncKeyState(VK_MBUTTON); break;
		case 3: state = GetAsyncKeyState(VK_RBUTTON); break;
		case 4: state = GetAsyncKeyState(VK_XBUTTON1); break;
		case 5: state = GetAsyncKeyState(VK_XBUTTON2); break;
	}

	if (state & 0x8000) {
		lua_pushboolean(L, true);
	} else {
		lua_pushboolean(L, false);
	}

	return 1;
}

int Mouse::ButtonUp (lua_State* L) {
	return Mouse::MemoClosure(L, lua_tostring(L, 2), Mouse::ButtonUpFunc);
}

int Mouse::ButtonUpFunc (lua_State* L) {
	if (!lua_isnumber(L, 1))
		return 0;

	short state = 0;
	switch (lua_tointeger(L, 1)) {
		case 1: state = GetAsyncKeyState(VK_LBUTTON); break;
		case 2: state = GetAsyncKeyState(VK_MBUTTON); break;
		case 3: state = GetAsyncKeyState(VK_RBUTTON); break;
		case 4: state = GetAsyncKeyState(VK_XBUTTON1); break;
		case 5: state = GetAsyncKeyState(VK_XBUTTON2); break;
	}

	if (state & 0x8000) {
		lua_pushboolean(L, false);
	} else {
		lua_pushboolean(L, true);
	}

	return 1;
}

int Mouse::ClientX (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rh2.rh2MouseClient.x);
	return 1;
}

int Mouse::ClientY (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rh2.rh2MouseClient.y);
	return 1;
}

int Mouse::X (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rh2.rh2Mouse.x);
	return 1;
}

int Mouse::Y (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rh2.rh2Mouse.y);
	return 1;
}

int Mouse::WheelDelta (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rh4.rh4MouseWheelDelta);
	return 1;
}
