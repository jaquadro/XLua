#include "common.h"

// All lookup functions for values require subtables.  Create and
// memoize corresponding lookup tables so we only pay the metatable
// price once

int Values::ValuesIndex (lua_State* L, LPRVAL rv) {
	const char* key = lua_tostring(L, 2);

	if (!(strcmp(key, "flags") == 0 || strcmp(key, "values") == 0 || strcmp(key, "strings") == 0)) {
		return -1;
	}

	int (*index_method)(lua_State*L) = 0;
	int (*newindex_method)(lua_State*L) = 0;

	if (strcmp(key, "flags") == 0) {
		index_method = Values::Flag;
		newindex_method = Values::SetFlag;
	}
	else if (strcmp(key, "values") == 0) {
		index_method = Values::Value;
		newindex_method = Values::SetValue;
	}
	else if (strcmp(key, "strings") == 0) {
		index_method = Values::String;
		newindex_method = Values::SetString;
	}

	// If we memoized the table in the metatable, return that
	lua_getmetatable(L, 1);
	lua_pushstring(L, key);
	lua_rawget(L, -2);

	if (lua_istable(L, -1)) {
		return 1;
	}

	// Return metatable to top of stack
	lua_pop(L, 1);

	// Calling table and key are allready on the stack in the order we need them
	lua_pushstring(L, key);

	// Create object table
	lua_createtable(L, 0, 0);

	// Create object metatable
	lua_createtable(L, 0, 2);

	// Add __index
	lua_pushvalue(L, lua_upvalueindex(1));
	lua_pushvalue(L, lua_upvalueindex(2));
	lua_pushvalue(L, lua_upvalueindex(3));
	lua_pushlightuserdata(L, (void*)rv);
	lua_pushcclosure(L, index_method, 4);
	lua_setfield(L, -2, "__index");

	// Add __newindex
	lua_pushvalue(L, lua_upvalueindex(1));
	lua_pushvalue(L, lua_upvalueindex(2));
	lua_pushvalue(L, lua_upvalueindex(3));
	lua_pushlightuserdata(L, (void*)rv);
	lua_pushcclosure(L, newindex_method, 4);
	lua_setfield(L, -2, "__newindex");

	// Add __metatable
	lua_pushstring(L, "Access violation");
	lua_setfield(L, -2, "__metatable");
	
	// Bind metatable
	lua_setmetatable(L, -2);

	// Set calling table
	lua_rawset(L, -3);

	// Return new table
	lua_pushstring(L, key);
	lua_rawget(L, -2);

	return 1;
}

int Values::ValuesNewIndex (lua_State* L, LPRVAL rv) {
	return -1;
}

int Values::Flag (lua_State* L) {
	unsigned int key = lua_tointeger(L, 2);

	if (!CommonObject::ObjectCheck(L)) {
		return 0;
	}

	if (key == 0 || key > 32) {
		return 0;
	}
	key %= 32;

	LPRVAL rv = (LPRVAL)lua_touserdata(L, lua_upvalueindex(4));

	lua_pushboolean(L, (rv->rvValueFlags & (1 << key)) != 0);
	return 1;
}

int Values::Value (lua_State* L) {
	unsigned int key = lua_tointeger(L, 2);

	if (!CommonObject::ObjectCheck(L)) {
		return 0;
	}

	key -= 1;
	if (key >= VALUES_NUMBEROF_ALTERABLE) {
		return 0;
	}

	LPRVAL rv = (LPRVAL)lua_touserdata(L, lua_upvalueindex(4));

	switch (rv->rvpValues[key].m_type) {
		case TYPE_LONG: lua_pushinteger(L, rv->rvpValues[key].m_long); break;
		case TYPE_FLOAT: lua_pushnumber(L, rv->rvpValues[key].m_double); break;
		case TYPE_STRING: lua_pushstring(L, rv->rvpValues[key].m_pString); break;
	}

	return 1;
}

int Values::String (lua_State* L) {
	unsigned int key = lua_tointeger(L, 2);

	if (!CommonObject::ObjectCheck(L)) {
		return 0;
	}

	key -= 1;
	if (key >= STRINGS_NUMBEROF_ALTERABLE) {
		return 0;
	}

	LPRVAL rv = (LPRVAL)lua_touserdata(L, lua_upvalueindex(4));

	if (rv->rvStrings[key] == NULL) {
		lua_pushstring(L, "");
	}
	else {
		lua_pushstring(L, rv->rvStrings[key]);
	}

	return 1;
}

int Values::SetFlag (lua_State* L) {
	unsigned int key = lua_tointeger(L, 2);
	unsigned int bit = lua_toboolean(L, 3);

	if (!CommonObject::ObjectCheck(L)) {
		return 0;
	}

	if (key == 0 || key > 32) {
		return 0;
	}
	key %= 32;

	LPRVAL rv = (LPRVAL)lua_touserdata(L, lua_upvalueindex(4));

	rv->rvValueFlags = (rv->rvValueFlags & ~(1 << key)) | (bit << key);
	return 0;
}

int Values::SetValue (lua_State* L) {
	unsigned int key = lua_tointeger(L, 2);

	if (!CommonObject::ObjectCheck(L)) {
		return 0;
	}

	key -= 1;
	if (key >= VALUES_NUMBEROF_ALTERABLE) {
		return 0;
	}

	LPRVAL rv = (LPRVAL)lua_touserdata(L, lua_upvalueindex(4));

	int intval = lua_tointeger(L, 3);
	double dblval = lua_tonumber(L, 3);

	// Try to figure out if it's an int or a double.
	if ((double)intval == dblval) {
		rv->rvpValues[key].m_type = TYPE_LONG;
		rv->rvpValues[key].m_long = intval;
	}
	else {
		rv->rvpValues[key].m_type = TYPE_DOUBLE;
		rv->rvpValues[key].m_double = dblval;
	}

	return 0;
}

int Values::SetString (lua_State* L) {
	unsigned int key = lua_tointeger(L, 2);

	if (!CommonObject::ObjectCheck(L)) {
		return 0;
	}

	key -= 1;
	if (key >= STRINGS_NUMBEROF_ALTERABLE) {
		return 0;
	}

	LPHO oh = (LPHO) lua_touserdata(L, lua_upvalueindex(1));
	LPRVAL rv = (LPRVAL)lua_touserdata(L, lua_upvalueindex(4));

	unsigned slen = lua_objlen(L, 3);
	rv->rvStrings[key] = (LPSTR) mvReAlloc(oh->hoAdRunHeader->rh4.rh4Mv, rv->rvStrings[key], slen + 1);
	strncpy_s(rv->rvStrings[key], slen + 1, lua_tostring(L, 3), slen + 1);

	return 0;
}
