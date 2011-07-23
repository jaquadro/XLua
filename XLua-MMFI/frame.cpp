#include "frame.h"
#include "collisions.h"

int Frame::IndexMetamethod (lua_State* L) {
	if (lua_tointeger(L, lua_upvalueindex(UV_TYPE)) != TYPE_STATIC)
		return 0;

	const char* key = lua_tostring(L, 2);

	int ret = StandardIndex(L, FrameRead, FrameWrite);
	if (ret > -1) return ret;

	return 0;
}

int Frame::NewIndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_STATIC)
		return 0;

	const char* key = lua_tostring(L, 2);

	int ret = StandardNewIndex(L, FrameRead, FrameWrite);
	if (ret > -1) return ret;

	return 0;
}

int Frame::MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc) {
	return ::MemoClosure(L, key, lfunc, 2);
}

int Frame::NewFrame (lua_State* L) {
	LPRH rh = xlua_get_run_header(L);

	int base = lua_gettop(L);								// +0
	if (base <= 0)
		return 0;

	lua_checkstack(L, base + 10);

	int (*index_method)(lua_State*L) = Frame::IndexMetamethod;
	int (*newindex_method)(lua_State*L) = Frame::NewIndexMetamethod;

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

int Frame::XLeft (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rh3.rh3DisplayX);
	return 1;
}

int Frame::XRight (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rh3.rh3DisplayX + rh->rh3.rh3WindowSx);
	return 1;
}

int Frame::YTop (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rh3.rh3DisplayX);
	return 1;
}

int Frame::YBottom (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rh3.rh3DisplayY + rh->rh3.rh3WindowSy);
	return 1;
}

int Frame::Width (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rhFrame->m_hdr.leWidth);
	return 1;
}

int Frame::Height (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rhFrame->m_hdr.leHeight);
	return 1;
}

int Frame::VWidth (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rhLevelSx);
	return 1;
}

int Frame::VHeight (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh) return 0;

	lua_pushnumber(L, rh->rhLevelSy);
	return 1;
}

int Frame::TestPoint (lua_State* L) {
	return Frame::MemoClosure(L, lua_tostring(L, 2), Frame::TestPointFunc);
}

int Frame::TestPointFunc (lua_State* L) {
	if (!MinParamCount(L, "testPoint", 2))
		return 0;

	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh)
		return 0;

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);

	int layer = Collisions::LayerParam(L, 3);

	int test = Collisions::BackdropTestPoint(rh, x, y, layer);
	lua_pushboolean(L, test);

	return 1;
}

int Frame::TestRect (lua_State* L) {
	return Frame::MemoClosure(L, lua_tostring(L, 2), Frame::TestRectFunc);
}

int Frame::TestRectFunc (lua_State* L) {
	if (!MinParamCount(L, "testRect", 4))
		return 0;

	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	if (!rh)
		return 0;

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);

	int layer = Collisions::LayerParam(L, 5);

	int test = Collisions::BackdropTestRect(rh, x, y, w, h, layer);
	lua_pushboolean(L, test);

	return 1;
}
