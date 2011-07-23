#include "object.h"

int Animations::AnimationsIndex (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	if ((o->hoOEFlags & OEFLAG_ANIMATIONS) == 0)
		return -1;

	return StandardIndex(L, AnimationsRead, AnimationsWrite);
}

int Animations::AnimationsNewIndex (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	if ((o->hoOEFlags & OEFLAG_ANIMATIONS) == 0)
		return -1;

	return StandardNewIndex(L, AnimationsRead, AnimationsWrite);
}

// Getters

int Animations::AnimationFrame (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRA* ra = GetRA(ho);

	lua_pushnumber(L, ra->raAnimFrame - 1);

	return 1;
}

int Animations::AnimationSet (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	lua_pushnumber(L, com->rcAnim);

	return 1;
}

// Mutators

int Animations::SetAnimationFrame (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	tagRA* ra = GetRA(ho);

	ra->raAnimFrame = lua_tointeger(L, PARAM1) + 1;
	ra->raAnimFrameForced = ra->raAnimFrame;

	com->rcChanged = 1;

	return 1;
}

int Animations::SetAnimationSet (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	com->rcAnim = lua_tointeger(L, PARAM1);
	com->rcChanged = 1;

	return 1;
}
