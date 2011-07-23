#include "system.h"
#include "object.h"

int Counter::IndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_OBJECT)
		return 0;

	// Check that object is valid
	if (!ObjectCheck(L))
		return 0;

	int ret;
	
	ret = Common::CommonIndex(L);
	if (ret > -1) return ret;

	ret = Sprites::SpritesIndex(L);
	if (ret > -1) return ret;

	ret = Movements::MovementsIndex(L);
	if (ret > -1) return ret;

	// Our fields
	ret = StandardIndex(L, CounterRead, CounterWrite);
	if (ret > -1) return ret;

	// Simulate some inheritance
	return Object::InheritCommon(L);
}

int Counter::NewIndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_OBJECT)
		return 0;

	// Check that object is valid
	if (!ObjectCheck(L)) {
		return 0;
	}

	int ret;
	
	ret = Common::CommonNewIndex(L);
	if (ret > -1) return ret;

	ret = Sprites::SpritesNewIndex(L);
	if (ret > -1) return ret;

	ret = Movements::MovementsNewIndex(L);
	if (ret > -1) return ret;

	// Our fields
	ret = StandardNewIndex(L, CounterRead, CounterWrite);
	if (ret > -1) return ret;

	// It's some untracked value, let them add it
	lua_rawset(L, 1);

	return 0;
}

// -----

int Text::IndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_OBJECT)
		return 0;

	// Check that object is valid
	if (!ObjectCheck(L))
		return 0;

	int ret;
	
	ret = Common::CommonIndex(L);
	if (ret > -1) return ret;

	ret = Sprites::SpritesIndex(L);
	if (ret > -1) return ret;

	ret = Movements::MovementsIndex(L);
	if (ret > -1) return ret;

	// Our fields
	ret = StandardIndex(L, TextRead, TextWrite);
	if (ret > -1) return ret;

	// Simulate some inheritance
	return Object::InheritCommon(L);
}

int Text::NewIndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_OBJECT)
		return 0;

	// Check that object is valid
	if (!ObjectCheck(L)) {
		return 0;
	}

	int ret;
	
	ret = Common::CommonNewIndex(L);
	if (ret > -1) return ret;

	ret = Sprites::SpritesNewIndex(L);
	if (ret > -1) return ret;

	ret = Movements::MovementsNewIndex(L);
	if (ret > -1) return ret;

	// Our fields
	ret = StandardNewIndex(L, TextRead, TextWrite);
	if (ret > -1) return ret;

	// It's some untracked value, let them add it
	lua_rawset(L, 1);

	return 0;
}

// -----

int System::MaxValue (lua_State* L) {
	LPRS o = (LPRS)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->rsMaxiDouble);
	return 1;
}

int System::MinValue (lua_State* L) {
	LPRS o = (LPRS)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushnumber(L, o->rsMiniDouble);
	return 1;
}

int System::Value (lua_State* L) {
	LPRS o = (LPRS)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	if (o->rsValue.m_type == TYPE_LONG)
		lua_pushnumber(L, ~o->rsValue.m_long);
	else if (o->rsValue.m_type == TYPE_FLOAT)
		lua_pushnumber(L, o->rsValue.m_double);
	else
		lua_pushnumber(L, 0);

	return 1;
}

int System::Text (lua_State* L) {
	LPRS o = (LPRS)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushstring(L, o->rsTextBuffer);
	return 1;
}

// -----

int System::SetMaxValue (lua_State* L) {
	LPRS o = (LPRS)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	
	o->rsMaxi = lua_tointeger(L, PARAM1);
	o->rsMaxiDouble = lua_tonumber(L, PARAM1);

	return 0;
}

int System::SetMinValue (lua_State* L) {
	LPRS o = (LPRS)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	
	o->rsMini = lua_tointeger(L, PARAM1);
	o->rsMiniDouble = lua_tonumber(L, PARAM1);

	return 0;
}

int System::SetValue (lua_State* L) {
	LPRS o = (LPRS)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	
	if ((double)lua_tointeger(L, PARAM1) == lua_tonumber(L, PARAM1)) {
		o->rsValue.m_type = TYPE_LONG;
		o->rsValue.m_long = ~lua_tointeger(L, PARAM1);
	}
	else {
		o->rsValue.m_type = TYPE_FLOAT;
		o->rsValue.m_double = lua_tonumber(L, PARAM1);
	}

	return 0;
}
