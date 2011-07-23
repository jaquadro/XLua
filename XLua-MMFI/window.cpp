#include "window.h"
#include "collisions.h"

int Window::IndexMetamethod (lua_State* L) {
	if (lua_tointeger(L, lua_upvalueindex(UV_TYPE)) != TYPE_STATIC)
		return 0;

	const char* key = lua_tostring(L, 2);

	int ret = StandardIndex(L, WindowRead, WindowWrite);
	if (ret > -1) return ret;

	return 0;
}

int Window::NewIndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_STATIC)
		return 0;

	const char* key = lua_tostring(L, 2);

	int ret = StandardNewIndex(L, WindowRead, WindowWrite);
	if (ret > -1) return ret;

	return 0;
}

int Window::MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc) {
	return ::MemoClosure(L, key, lfunc, 2);
}

int Window::NewWindow (lua_State* L) {
	LPRH rh = xlua_get_run_header(L);

	int base = lua_gettop(L);								// +0
	if (base <= 0)
		return 0;

	lua_checkstack(L, base + 10);

	int (*index_method)(lua_State*L) = Window::IndexMetamethod;
	int (*newindex_method)(lua_State*L) = Window::NewIndexMetamethod;

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

int Window::ClientWidth (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	RECT rc;
	GetClientRect(rh->rhHMainWin, &rc);

	lua_pushnumber(L, rc.right - rc.left);
	return 1;
}

int Window::ClientHeight (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	RECT rc;
	GetClientRect(rh->rhHMainWin, &rc);

	lua_pushnumber(L, rc.bottom - rc.top);
	return 1;
}

int Window::FrameWidth (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	RECT rc;
	GetWindowRect(rh->rhHEditWin, &rc);

	lua_pushnumber(L, rc.right - rc.left);
	return 1;
}

int Window::FrameHeight (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	RECT rc;
	GetWindowRect(rh->rhHEditWin, &rc);

	lua_pushnumber(L, rc.bottom - rc.top);
	return 1;
}

int Window::Width (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	RECT rc;
	GetWindowRect(rh->rhHMainWin, &rc);

	lua_pushnumber(L, rc.right - rc.left);
	return 1;
}

int Window::Height (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	RECT rc;
	GetWindowRect(rh->rhHMainWin, &rc);

	lua_pushnumber(L, rc.bottom - rc.top);
	return 1;
}
