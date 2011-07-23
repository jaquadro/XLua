#include "Common.h"

void LuaXLua::SetEnumInt (const char* name, int val) {
	lua_pushinteger(_state->state, val);
	lua_setfield(_state->state, -2, name);
}

int LuaXLua::pass (lua_State* L) {
	lua_pushboolean(L, 1);
	return 1;
}

int LuaXLua::fail (lua_State* L) {
	lua_pushboolean(L, 0);
	return 1;
}

void LuaXLua::Register () {

	// Create xlua package
	lua_newtable(_state->state);
	lua_setglobal(_state->state, "xlua");
	lua_getglobal(_state->state, "xlua");

	// ENUM Values
	SetEnumInt("REMOTE_LOAD", REMOTE_LOAD);
	SetEnumInt("REMOTE_DESTROY", REMOTE_DESTROY);

	// RPC functions
	lua_pushcfunction(_state->state, LuaXLua::exportFunction);
	lua_setfield(_state->state, -2, "Export");
	lua_pushcfunction(_state->state, LuaXLua::clearExport);
	lua_setfield(_state->state, -2, "ClearExport");
	lua_pushcfunction(_state->state, LuaXLua::rpc);
	lua_setfield(_state->state, -2, "Call");
	lua_pushcfunction(_state->state, LuaXLua::rload);
	lua_setfield(_state->state, -2, "Load");
	lua_pushcfunction(_state->state, LuaXLua::rloadFile);
	lua_setfield(_state->state, -2, "LoadFile");
	lua_pushcfunction(_state->state, LuaXLua::grant);
	lua_setfield(_state->state, -2, "Grant");
	lua_pushcfunction(_state->state, LuaXLua::revoke);
	lua_setfield(_state->state, -2, "Revoke");
	lua_pushcfunction(_state->state, LuaXLua::newState);
	lua_setfield(_state->state, -2, "NewState");
	lua_pushcfunction(_state->state, LuaXLua::deleteState);
	lua_setfield(_state->state, -2, "DeleteState");
}

// xlua.export(func, func_name, allowed_states ...)
int LuaXLua::exportFunction (lua_State* L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	if (!lua_isfunction(L, 1)) {
		lua_pushstring(L, "xlua.export: Argument 1 requires a function\n");
		lua_error(L);
	}

	if (!lua_isstring(L, 2)) {
		lua_pushstring(L, "xlua.export: Argument 2 requires a string\n");
		lua_error(L);
	}

	ExportEntry ee;
	
	lua_pushvalue(L, 1);
	ee.func_id = luaL_ref(L, LUA_REGISTRYINDEX);

	for (int i = 3; lua_isnumber(L, i); i++) {
		ee.allowed_states.insert(lua_tointeger(L, i));
	}

	// Remove old entry if present
	std::string fname = lua_tostring(L, 2);
	IExports iter = state->xlua.exports.find(fname);

	if (iter != state->xlua.exports.end()) {
		luaL_unref(L, LUA_REGISTRYINDEX, iter->second.func_id);
	}

	// Set export
	state->xlua.exports[lua_tostring(L, 2)] = ee;

	return 0;
}

// xlua.clearExport(func_name)
int LuaXLua::clearExport (lua_State* L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	if (!lua_isstring(L, 1)) {
		lua_pushstring(L, "xlua.clearExport: Argument 1 requires a string\n");
		lua_error(L);
	}

	std::string fname = lua_tostring(L, 1);
	IExports iter = state->xlua.exports.find(fname);

	if (iter == state->xlua.exports.end())
		return 0;

	luaL_unref(L, LUA_REGISTRYINDEX, iter->second.func_id);
	state->xlua.exports.erase(lua_tostring(L, 1));

	return 0;
}

// xlua.grant(feature, to_states ...)
int LuaXLua::grant (lua_State* L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	if (!lua_isnumber(L, 1)) return 0;
	int feature = lua_tointeger(L, 1);

	if (feature <= 0 || feature > 1) return 0;

	int top = lua_gettop(L);
	for (int i = 2; i <= top; i++) {
		if (!lua_isnumber(L, i)) continue;
		int sid = lua_tointeger(L, i);

		switch (feature) {
			case REMOTE_LOAD: state->xlua.rload_states.insert(sid); break;
			case REMOTE_DESTROY: state->xlua.destroy_states.insert(sid); break;
		}
	}

	return 0;
}

// xlua.revoke(feature, from_states ...)
int LuaXLua::revoke (lua_State* L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	if (!lua_isnumber(L, 1)) return 0;
	int feature = lua_tointeger(L, 1);

	if (feature <= 0 || feature > 1) return 0;

	int top = lua_gettop(L);
	for (int i = 2; i <= top; i++) {
		if (!lua_isnumber(L, i)) continue;
		int sid = lua_tointeger(L, i);

		switch (feature) {
			case REMOTE_LOAD: state->xlua.rload_states.erase(sid); break;
			case REMOTE_DESTROY: state->xlua.destroy_states.erase(sid); break;
		}
	}

	return 0;
}

// xlua.rpc(state_id, func_name, args...)
int LuaXLua::rpc (lua_State* L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	int top = lua_gettop(L);

	int sid = lua_tointeger(L, 1);
	std::string fname = lua_tostring(L, 2);

	XLuaState* altstate = XLuaGlobal::Get().GetState(sid);
	if (!altstate)
		return 0;

	IExports iter = altstate->xlua.exports.find(fname);
	if (iter == altstate->xlua.exports.end())
		return 0;

	// Permission check
	if (iter->second.allowed_states.size() > 0) {
		ISid auth = iter->second.allowed_states.find(state->stateId);
		if (auth == iter->second.allowed_states.end())
			return 0;
	}

	// Setup function call
	lua_State* K = altstate->state;
	int kpre = lua_gettop(K);

	lua_checkstack(K, top - 2);
	
	lua_rawgeti(K, LUA_REGISTRYINDEX, iter->second.func_id);
	if (!lua_isfunction(K, -1))
		return 0;

	// We have to be careful what parameters we can push
	LuaXLua::stackCopy(L, K, 3, top);

	lua_call(K, top - 2, LUA_MULTRET);

	// Now we have the task of fetching return values!
	int kpost = lua_gettop(K);
	lua_checkstack(L, kpost - kpre);
	LuaXLua::stackCopy(K, L, kpre + 1, kpost);

	return kpost - kpre;
}

std::map<const void*, int> LuaXLua::tableRef;

void LuaXLua::stackCopy (lua_State* src, lua_State* dst, int si, int ei) {
	for (int i = si; i <= ei; i++) {
		switch (lua_type(src, i)) {
			case LUA_TNUMBER: lua_pushnumber(dst, lua_tonumber(src, i)); break;
			case LUA_TSTRING: lua_pushstring(dst, lua_tostring(src, i)); break;
			case LUA_TBOOLEAN: lua_pushboolean(dst, lua_toboolean(src, i)); break;
			case LUA_TTABLE: LuaXLua::tableCopy(src, dst, i); break;
			default: lua_pushnil(dst);
		}
	}

	// Clean up any saved table references
	if (tableRef.size() > 0) {
		for (ITableRef r = LuaXLua::tableRef.begin(); r != LuaXLua::tableRef.end(); r++) {
			luaL_unref(dst, LUA_REGISTRYINDEX, r->second);
		}
		tableRef.clear();
	}
}

void LuaXLua::tableCopy (lua_State* src, lua_State* dst, int si) {

	// Check if we've already processed this table
	const void* p = lua_topointer(src, si);
	ITableRef ref = LuaXLua::tableRef.find(p);
	if (ref != LuaXLua::tableRef.end()) {
		lua_rawgeti(dst, LUA_REGISTRYINDEX, ref->second);
		return;
	}

	// We haven't, so create the new table and cache its reference
	lua_checkstack(dst, 3);
	lua_newtable(dst);
	lua_pushvalue(dst, -1);
	tableRef[p] = luaL_ref(dst, LUA_REGISTRYINDEX);

	lua_pushnil(src); /* first key */
	while (lua_next(src, si)) {
		// Push key
		switch (lua_type(src, -2)) {
			case LUA_TNUMBER: lua_pushnumber(dst, lua_tonumber(src, -2)); break;
			case LUA_TSTRING: lua_pushstring(dst, lua_tostring(src, -2)); break;
			default:
				lua_pop(src, 1);
				continue;
				// If the key is weird, just skip this pair
		}
		switch (lua_type(src, -1)) {
			case LUA_TNUMBER: lua_pushnumber(dst, lua_tonumber(src, -1)); break;
			case LUA_TSTRING: lua_pushstring(dst, lua_tostring(src, -1)); break;
			case LUA_TBOOLEAN: lua_pushboolean(dst, lua_toboolean(src, -1)); break;
			case LUA_TTABLE: LuaXLua::tableCopy(src, dst, lua_gettop(src)); break; // Can we do this recursively?
			default: lua_pushnil(dst);
		}
		
		lua_rawset(dst, -3);
		lua_pop(src, 1);
	}
}

// xlua.rload(sid, src, name = nil)
int LuaXLua::rload (lua_State* L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL)
		return fail(L);

	int sid = lua_tointeger(L, 1);
	const char* src = lua_tostring(L, 2);
	const char* name = 0;

	if (lua_isstring(L, 3))
		name = lua_tostring(L, 3);

	XLuaState* altstate = XLuaGlobal::Get().GetState(sid);
	if (!altstate)
		return fail(L);

	ISid iter = altstate->xlua.rload_states.find(state->stateId);
	if (iter == altstate->xlua.rload_states.end())
		return fail(L);

	altstate->LoadString(src, name);

	return pass(L);
}

// xlua.rloadFile(sid, file)
int LuaXLua::rloadFile (lua_State* L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL)
		return fail(L);

	int sid = lua_tointeger(L, 1);
	const char* file = lua_tostring(L, 2);
	//const char* name = 0;

	//if (lua_isstring(L, 3))
	//	name = lua_tostring(L, 3);

	XLuaState* altstate = XLuaGlobal::Get().GetState(sid);
	if (!altstate)
		return fail(L);

	ISid iter = altstate->xlua.rload_states.find(state->stateId);
	if (iter == altstate->xlua.rload_states.end())
		return fail(L);

	altstate->LoadFile(file);

	return pass(L);
}

// xlua.newState(id)
int LuaXLua::newState (lua_State* L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL)
		return fail(L);

	if (!lua_isnumber(L, 1))
		return fail(L);

	int sid = lua_tointeger(L, 1);

	XLuaState* chk = XLuaGlobal::Get().GetState(sid);
	if (chk != NULL)
		return fail(L);

	if (!XLuaGlobal::Get().CreateState(sid))
		return fail(L);

	// Need to grant "parent" state permissions to mutate state
	chk = XLuaGlobal::Get().GetState(sid);
	if (chk == NULL)
		return fail(L);

	chk->xlua.rload_states.insert(state->stateId);
	chk->xlua.destroy_states.insert(state->stateId);

	return pass(L);
}

// xlua.deleteState(id)
int LuaXLua::deleteState (lua_State* L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL)
		return fail(L);

	if (!lua_isnumber(L, 1))
		return fail(L);

	int sid = lua_tointeger(L, 1);

	XLuaState* chk = XLuaGlobal::Get().GetState(sid);
	if (chk == NULL)
		return fail(L);

	if (chk->rdList.size() > 0)
		return fail(L);

	ISid iter = chk->xlua.destroy_states.find(state->stateId);
	if (iter == chk->xlua.destroy_states.end())
		return fail(L);

	if (!XLuaGlobal::Get().DeleteState(sid))
		return fail(L);

	return pass(L);
}
