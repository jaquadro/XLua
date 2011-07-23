#include "object.h"
#include "class.h"

int Object::IndexMetamethod (lua_State* L) {
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

	ret = Animations::AnimationsIndex(L);
	if (ret > -1) return ret;

	ret = Values::ValuesIndex(L);
	if (ret > -1) return ret;

	// Simulate some inheritance
	lua_pushstring(L, "common");
	lua_rawget(L, lua_upvalueindex(UV_OBJECT_CLASS));
	if (!lua_istable(L, -1))
		return 0;

	lua_pushvalue(L, 2);
	lua_rawget(L, -2);

	if (!lua_isnil(L, -1))
		return 1;

	return 0;
}

int Object::NewIndexMetamethod (lua_State* L) {
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

	ret = Animations::AnimationsNewIndex(L);
	if (ret > -1) return ret;

	ret = Values::ValuesIndex(L);
	if (ret > -1) return ret;

	// It's some untracked value, let them add it
	lua_rawset(L, 1);

	return 0;
}

int Object::InheritCommon (lua_State* L) {
	lua_pushstring(L, "common");
	lua_rawget(L, lua_upvalueindex(UV_OBJECT_CLASS));
	if (!lua_istable(L, -1))
		return 0;

	lua_pushvalue(L, 2);
	lua_rawget(L, -2);

	if (!lua_isnil(L, -1))
		return 1;

	return 0;
}

int Object::MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc) {
	return ::MemoClosure(L, key, lfunc, 5);
}

LPHO Object::GetObject (lua_State* L, int index) {
	if (!lua_istable(L, index))
		return 0;

	int base = lua_gettop(L);								// +0
	lua_checkstack(L, base + 6);

	lua_getmetatable(L, index);								// +1 = Object metatable
	lua_pushstring(L, "__index");							// +2
	lua_rawget(L, -2);										// +2 = __index method

	lua_getupvalue(L, base + 2, UV_TYPE);					// +3 = "TYPE" upvalue
	if (lua_tointeger(L, base + 3) != TYPE_OBJECT)
		return 0;

	lua_getupvalue(L, base + 2, UV_OBJECT_LPHO);			// +4 = "HO" upvalue
	lua_getupvalue(L, base + 2, UV_OBJECT_LPRH);			// +5 = "RH" upvalue
	lua_getupvalue(L, base + 2, UV_OBJECT_FIXED);			// +6 = "FIXED" upvalue

	LPHO ho = (LPHO)lua_touserdata(L, base + 4);
	LPRH rh = (LPRH)lua_touserdata(L, base + 5);
	int fixed = lua_tointeger(L, base + 6);

	// Check that object is valid
	if (rh->rhObjectList[fixed & 0xFFFF].oblOffset != ho)
		return 0;

	lua_pop(L, 6);
	return ho;
}

int Object::NewObject (lua_State* L) {
	LPRH rh = xlua_get_run_header(L);

	int base = lua_gettop(L);								// +0
	if (base <= 0)
		return 0;

	lua_checkstack(L, base + 10);

	int hoNum = lua_tointeger(L, 1) & 0xFFFF;
	LPHO obj = rh->rhObjectList[hoNum].oblOffset;
	int fixed = obj->hoCreationId << 16 | obj->hoNumber;

	// Check if we cached the object
	lua_pushstring(L, KEY_POOL_OBJECT);						// +1
	lua_rawget(L, LUA_REGISTRYINDEX);						// +1 = Object Pool
	lua_pushinteger(L, fixed);								// +2
	lua_rawget(L, -2);										// +2 = Object Cached Table

	if (lua_istable(L, -1))
		return 1;
	else
		lua_pop(L, 1);										// +1

	// Check if we cached the class
	lua_pushstring(L, KEY_POOL_CLASS);						// +2
	lua_rawget(L, LUA_REGISTRYINDEX);						// +2 = Class Pool
	lua_pushinteger(L, obj->hoOi);							// +3
	lua_rawget(L, -2);										// +3 = Class Cached Table

	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);										// +2
		lua_pushcfunction(L, ObjectClass::NewObjectClass);	// +3
		lua_pushstring(L, (char*)obj->hoOiList->oilName);	// +4
		lua_call(L, 1, 1);									// +3 = Class Cached Table
	}

	// Determine the correct metamethods based on object type
	lua_pushstring(L, KEY_TYPE_REGISTRY);					// +4
	lua_rawget(L, LUA_REGISTRYINDEX);						// +4 = Type Registry
	lua_pushinteger(L, obj->hoIdentifier);					// +5
	lua_rawget(L, -2);										// +5 = Type Record

	lua_CFunction index_method = Object::IndexMetamethod;
	lua_CFunction newindex_method = Object::NewIndexMetamethod;

	if (lua_istable(L, -1)) {
		lua_rawgeti(L, -1, 1);								// +6
		lua_rawgeti(L, -2, 2);								// +7
		index_method = (lua_CFunction) lua_touserdata(L, base + 6);
		newindex_method = (lua_CFunction) lua_touserdata(L, base + 7);

		lua_pop(L, 2);
	}

	lua_pop(L, 2);											// +3

	// Create object table
	lua_createtable(L, 0, 0);								// +4 = Object Table

	// Create object metatable
	lua_createtable(L, 0, 3);								// +5 = Object Metatable

	// Add __index
	lua_pushnumber(L, TYPE_OBJECT);							// +6
	lua_pushlightuserdata(L, (void*)obj);					// +7
	lua_pushnumber(L, fixed);								// +8
	lua_pushlightuserdata(L, (void*)obj->hoAdRunHeader);	// +9
	lua_pushvalue(L, base + 3);								// +10
	lua_pushcclosure(L, index_method, 5);					// +6
	lua_setfield(L, -2, "__index");							// +5

	// Add __newindex
	lua_pushnumber(L, TYPE_OBJECT);							// +6
	lua_pushlightuserdata(L, (void*)obj);					// +7
	lua_pushnumber(L, fixed);								// +8
	lua_pushlightuserdata(L, (void*)obj->hoAdRunHeader);	// +9
	lua_pushvalue(L, base + 3);								// +10
	lua_pushcclosure(L, newindex_method, 5);				// +6
	lua_setfield(L, -2, "__newindex");						// +5

	// Add __metatable
	lua_pushstring(L, "Access violation");					// +6
	lua_setfield(L, -2, "__metatable");						// +5
	
	// Bind metatable
	lua_setmetatable(L, -2);								// +4

	// Cache table
	lua_pushinteger(L, fixed);								// +5
	lua_pushvalue(L, -2);									// +6
	lua_rawset(L, base + 1);								// +4

	return 1;
}
