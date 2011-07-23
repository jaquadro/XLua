#include "runtime.h"

// All lookup functions for values require subtables.  Create and
// memoize corresponding lookup tables so we only pay the metatable
// price once

int Globals::GlobalsIndex (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));

	const char* key = lua_tostring(L, 2);

	if (!(strcmp(key, "values") == 0 || strcmp(key, "strings") == 0)) {
		return -1;
	}

	int (*index_method)(lua_State*L) = 0;
	int (*newindex_method)(lua_State*L) = 0;

	if (strcmp(key, "values") == 0) {
		index_method = Globals::Value;
		newindex_method = Globals::SetValue;
	}
	else if (strcmp(key, "strings") == 0) {
		index_method = Globals::String;
		newindex_method = Globals::SetString;
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
	lua_pushvalue(L, TYPE_STATIC_VALS);
	lua_pushvalue(L, lua_upvalueindex(UV_STATIC_RH));
	lua_pushcclosure(L, index_method, 2);
	lua_setfield(L, -2, "__index");

	// Add __newindex
	lua_pushvalue(L, TYPE_STATIC_VALS);
	lua_pushvalue(L, lua_upvalueindex(UV_STATIC_RH));
	lua_pushcclosure(L, newindex_method, 2);
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

int Globals::GlobalsNewIndex (lua_State* L) {
	return -1;
}

int Globals::Value (lua_State* L) {
	int key = ValueKeyLookup(L);
	if (key < 0)
		return 0;

	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	CRunApp* app = rh->rhApp;

	while (app->m_pParentApp && app->m_bShareGlobalValues) {
		app = app->m_pParentApp;
	}

	if (key >= app->m_nGlobalValues) {
		return 0;
	}

	switch (app->m_pGlobalValues[key].m_type) {
		case TYPE_LONG: lua_pushinteger(L, app->m_pGlobalValues[key].m_long); break;
		case TYPE_FLOAT: lua_pushnumber(L, app->m_pGlobalValues[key].m_double); break;
		case TYPE_STRING: lua_pushstring(L, app->m_pGlobalValues[key].m_pString); break;
	}

	return 1;
}

int Globals::String (lua_State* L) {
	int key = StringKeyLookup(L);
	if (key < 0)
		return 0;

	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	CRunApp* app = rh->rhApp;

	while (app->m_pParentApp && app->m_bShareGlobalValues) {
		app = app->m_pParentApp;
	}

	if (key >= app->m_nGlobalStrings) {
		return 0;
	}

	if (app->m_pGlobalString[key] == NULL) {
		lua_pushstring(L, "");
	}
	else {
		lua_pushstring(L, app->m_pGlobalString[key]);
	}

	return 1;
}

int Globals::SetValue (lua_State* L) {
	int key = ValueKeyLookup(L);
	if (key < 0)
		return 0;

	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	CRunApp* app = rh->rhApp;

	while (app->m_pParentApp && app->m_bShareGlobalValues) {
		app = app->m_pParentApp;
	}

	if (key >= app->m_nGlobalValues) {
		return 0;
	}

	int intval = lua_tointeger(L, 3);
	double dblval = lua_tonumber(L, 3);

	// Try to figure out if it's an int or a double.
	if ((double)intval == dblval) {
		app->m_pGlobalValues[key].m_type = TYPE_LONG;
		app->m_pGlobalValues[key].m_long = intval;
	}
	else {
		app->m_pGlobalValues[key].m_type = TYPE_DOUBLE;
		app->m_pGlobalValues[key].m_double = dblval;
	}

	return 0;
}

int Globals::SetString (lua_State* L) {
	int key = StringKeyLookup(L);
	if (key < 0)
		return 0;

	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));
	CRunApp* app = rh->rhApp;

	while (app->m_pParentApp && app->m_bShareGlobalValues) {
		app = app->m_pParentApp;
	}

	if (key >= app->m_nGlobalStrings) {
		return 0;
	}

	unsigned slen = lua_objlen(L, 3);
	app->m_pGlobalString[key] = (LPSTR) mvReAlloc(rh->rh4.rh4Mv, app->m_pGlobalString[key], slen + 1);
	strncpy_s(app->m_pGlobalString[key], slen + 1, lua_tostring(L, 3), slen + 1);

	return 0;
}

// -----

int Globals::ValueKeyLookup (lua_State* L) {
	if (lua_isnumber(L, LPARAM_KEY))
		return lua_tointeger(L, LPARAM_KEY) - 1;

	if (!lua_isstring(L, LPARAM_KEY))
		return -1;

	const char* key = lua_tostring(L, LPARAM_KEY);

	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));

	int* vnameCount = (int*)rh->rhApp->m_pGlobalValueNames;
	char* vname = (char*)vnameCount + 4;

	for (int i = 0; i < *vnameCount; i++) {
		if (strcmp(key, vname) == 0)
			return i;

		vname += strlen(vname) + 1;
	}

	return -1;
}

int Globals::StringKeyLookup (lua_State* L) {
	if (lua_isnumber(L, LPARAM_KEY))
		return lua_tointeger(L, LPARAM_KEY) - 1;

	if (!lua_isstring(L, LPARAM_KEY))
		return -1;

	const char* key = lua_tostring(L, LPARAM_KEY);

	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_STATIC_RH));

	int* vnameCount = (int*)rh->rhApp->m_pGlobalStringNames;
	char* vname = (char*)vnameCount + 4;

	for (int i = 0; i < *vnameCount; i++) {
		if (strcmp(key, vname) == 0)
			return i;

		vname += strlen(vname) + 1;
	}

	return -1;
}
