#include "object.h"

int Common::CommonIndex (lua_State* L) {
	return StandardIndex(L, CommonRead, CommonWrite);
}

int Common::CommonNewIndex (lua_State* L) {
	return StandardNewIndex(L, CommonRead, CommonWrite);
}

// State Getters

int Common::Type (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoIdentifier);
	return 1;
}

int Common::ClassID (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoOi);
	return 1;
}

int Common::ClassName (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	LPRS r = (LPRS)o;
	lua_pushstring(L, (LPSTR) &(o->hoOiList->oilName));
	return 1;
}

int Common::Class (lua_State* L) {
	lua_pushvalue(L, lua_upvalueindex(UV_OBJECT_CLASS));
	return 1;
}

int Common::Fixed (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, (o->hoCreationId<<16)+o->hoNumber);
	return 1;
}

int Common::X (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoX);
	return 1;
}

int Common::Y (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoY);
	return 1;
}

int Common::XLeft (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoX - o->hoImgXSpot);
	return 1;
}

int Common::XRight (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoX + (o->hoImgWidth - o->hoImgXSpot));
	return 1;
}

int Common::YTop (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoY - o->hoImgYSpot);
	return 1;
}

int Common::YBottom (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoY + (o->hoImgHeight - o->hoImgYSpot));
	return 1;
}

int Common::Width (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoImgWidth);
	return 1;
}

int Common::Height (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoImgHeight);
	return 1;
}

int Common::HotspotX (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoImgXSpot);
	return 1;
}

int Common::HotspotY (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoImgYSpot);
	return 1;
}

int Common::Layer (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->hoLayer + 1);
	return 1;
}

// State Mutators

int Common::SetX (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	o->hoAdRunHeader->rh4.rh4KpxFunctions[RFUNCTION_SETPOSITION].routine(o, lua_tointeger(L, 3), o->hoY);
	o->hoX = lua_tointeger(L, 3);
	return 0;
}

int Common::SetY (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	o->hoAdRunHeader->rh4.rh4KpxFunctions[RFUNCTION_SETPOSITION].routine(o, o->hoX, lua_tointeger(L, 3));
	o->hoY = lua_tointeger(L, 3);
	return 0;
}

int Common::SetLayer (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(o);
	tagRSPR* spr = GetRSPR(o);

	o->hoLayer = lua_tointeger(L, 3) - 1;

	if (com && com->rcSprite) {
		npWin win = o->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		SetSpriteLayer(win, com->rcSprite, o->hoLayer);

		if (spr) {
			spr->rsLayer = o->hoLayer;
			spr->rsZOrder = com->rcSprite->sprZOrder;
		}
	}

	return 0;
}

// Actions

int Common::Destroy (lua_State* L) {
	int r = Object::MemoClosure(L, lua_tostring(L, 2), Common::DestroyFunc);

	lua_pushnil(L);
	lua_setmetatable(L, 1);

	return r;
}

int Common::DestroyFunc (lua_State* L) {
	int fixed = lua_tointeger(L, lua_upvalueindex(UV_OBJECT_FIXED));

	// Clear object from cache
	lua_pushstring(L, KEY_POOL_OBJECT);
	lua_rawget(L, LUA_REGISTRYINDEX);
	lua_pushinteger(L, fixed);
	lua_rawget(L, -2);

	if (lua_istable(L, -1)) {
		lua_pushinteger(L, fixed);
		lua_pushnil(L);
		lua_rawset(L, -4);
	}

	// Delete object
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	o->hoAdRunHeader->rh4.rh4KpxFunctions[RFUNCTION_DESTROY].routine(o, 0, 0);
	return 0;
}
