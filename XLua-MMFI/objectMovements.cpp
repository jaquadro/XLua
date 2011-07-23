#include "object.h"

int Movements::MovementsIndex (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	if ((o->hoOEFlags & OEFLAG_MOVEMENTS) == 0)
		return -1;

	return StandardIndex(L, MovementsRead, MovementsWrite);
}

int Movements::MovementsNewIndex (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	if ((o->hoOEFlags & OEFLAG_MOVEMENTS) == 0)
		return -1;

	return StandardNewIndex(L, MovementsRead, MovementsWrite);
}

// Getters

int Movements::Direction (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	lua_pushnumber(L, com->rcDir);

	return 1;
}

// Mutators

int Movements::SetDirection (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	com->rcDir = lua_tointeger(L, PARAM1);
	com->rcChanged = 1;

	return 1;
}
