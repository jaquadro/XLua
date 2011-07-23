#include "../Common.h"

#include <sstream>

extern "C" {
	#include "luajit.h"
}

extern "C" const char* pushnexttemplate (lua_State* L, const char* path);
extern "C" void loaderror (lua_State* L, const char* filename);
extern "C" const char* mkfuncname (lua_State* L, const char* modname);
extern "C" int ll_loadfunc (lua_State* L, const char* path, const char* sym);

extern "C" void x_updatepath (lua_State* L) {
	LPRDATA rdPtr = XLuaGlobal::Get().GetStateByState(L)->rdList.front()->rdPtr;

	char* drive = (char*) calloc(_MAX_PATH, sizeof(char));
	char* dir = (char*) calloc(_MAX_PATH, sizeof(char));
	callRunTimeFunction(rdPtr, RFUNCTION_GETFILEINFOS, FILEINFO_DRIVE, (long)drive);
	callRunTimeFunction(rdPtr, RFUNCTION_GETFILEINFOS, FILEINFO_DIR, (long)dir);
	//rdPtr->rRd->GetApplicationDrive(drive);
	//rdPtr->rRd->GetApplicationDirectory(dir);

	std::stringstream pstr; pstr << drive << dir << "?.lua;";
	std::stringstream cstr; cstr << drive << dir << "?.dll;";

	free(drive);
	free(dir);

	lua_getfield(L, -1, "path");
	pstr << lua_tostring(L, -1);

	lua_pop(L, 1);
	lua_pushstring(L, pstr.str().c_str());
	lua_setfield(L, -2, "path");

	lua_getfield(L, -1, "cpath");
	cstr << lua_tostring(L, -1);

	lua_pop(L, 1);
	lua_pushstring(L, cstr.str().c_str());
	lua_setfield(L, -2, "cpath");
	
	//lua_settop(L, top);
}

std::vector<ScriptRecord*> x_find_records (lua_State* L, const char* name) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);

	std::vector<ScriptRecord*> srset;

	for (XLuaState::IRdList ir = state->rdList.begin(); ir != state->rdList.end(); ir++) {
		XLuaObject* lm = *ir;

		for (XLuaObject::IScripts is = lm->scripts.begin(); is != lm->scripts.end(); is++) {
			ScriptRecord* sr = &(is->second);

			if (sr->name.compare(name) == 0) {
				srset.push_back(sr);
			}
		}
	}

	return srset;
}

extern "C" int loader_XLua_Embedded (lua_State *L) {
	//const char *filename;
	const char *name = luaL_checkstring(L, 1);

	//filename = x_findfile(L, name, "path");
	//if (filename == NULL) return 1;  /* library not found in this path */

	std::vector<ScriptRecord*> srset = x_find_records(L, name);
	if (srset.empty())
		return 1;

	//LuaInternalFile lif;
	/*DWORD dwsz;
	LPRDATA rdPtr = LGLOB->GetStateByState(L)->rdList.front()->rdPtr;
	HANDLE hFile = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvOpenHFile(filename, &dwsz, 0);

	if (hFile == INVALID_HANDLE_VALUE) {
		loaderror(L, filename);
		return 0;
	}*/

	//LuaInternalFile lif;
	//char* text = (char*)calloc(dwsz+1, sizeof(char));

	//DWORD dwread;
	//ReadFile(hFile, text, dwsz, &dwread, NULL);
	
	for (std::vector<ScriptRecord*>::iterator is = srset.begin(); is != srset.end(); is++) {
		ScriptRecord* sr = *is;

		if (luaL_loadbuffer(L, sr->script.c_str(), sr->script.size(), name) != 0) {
			//free(text);
			//rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvCloseHFile(hFile);
			loaderror(L, name);
			return 0;
		}
	}

	//free(text);
	//rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvCloseHFile(hFile);

	//luaJIT_setmode(L, -1, LUAJIT_MODE_FUNC|LUAJIT_MODE_OFF);
	return 1;
}

const char* x_findfile (lua_State* L, const char* name, const char* pname) {
	LPRDATA rdPtr = XLuaGlobal::Get().GetStateByState(L)->rdList.front()->rdPtr;

	const char* path;
	name = luaL_gsub(L, name, ".", LUA_DIRSEP);
	lua_getfield(L, LUA_ENVIRONINDEX, pname);
	path = lua_tostring(L, -1);
	if (path == NULL) {
		luaL_error(L, LUA_QL("package.%s") " must be a string", pname);
	}
	lua_pushliteral(L, "");  /* error accumulator */
	while ((path = pushnexttemplate(L, path)) != NULL) {
		const char* filename;
		filename = luaL_gsub(L, lua_tostring(L, -1), LUA_PATH_MARK, name);
		lua_remove(L, -2);  /* remove path template */

		DWORD dwsz;
		HANDLE hFile = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvOpenHFile(filename, &dwsz, 0);
		if (hFile != INVALID_HANDLE_VALUE) {
			rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvCloseHFile(hFile);
			return filename;
		}
		rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvCloseHFile(hFile);

		//if (readable(filename)) { /* does file exist and is readable? */
		//	return filename;  /* return that file name */
		//}
		lua_pushfstring(L, "\n\tno file " LUA_QS, filename);
		lua_remove(L, -2);  /* remove file name */
		lua_concat(L, 2);  /* add entry to possible error message */
	}
	return NULL;  /* not found */
}

extern "C" int loader_XLua (lua_State *L) {
	const char *filename;
	const char *name = luaL_checkstring(L, 1);
	filename = x_findfile(L, name, "path");
	if (filename == NULL) return 1;  /* library not found in this path */

	//LuaInternalFile lif;
	DWORD dwsz;
	LPRDATA rdPtr = XLuaGlobal::Get().GetStateByState(L)->rdList.front()->rdPtr;
	HANDLE hFile = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvOpenHFile(filename, &dwsz, 0);

	if (hFile == INVALID_HANDLE_VALUE) {
		loaderror(L, filename);
		return 0;
	}

	//LuaInternalFile lif;
	char* text = (char*)calloc(dwsz+1, sizeof(char));

	DWORD dwread;
	ReadFile(hFile, text, dwsz, &dwread, NULL);

	if (dwsz != dwread || luaL_loadbuffer(L, text, dwsz, name) != 0) {
		free(text);
		rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvCloseHFile(hFile);
		loaderror(L, filename);
		return 0;
	}

	free(text);
	rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvCloseHFile(hFile);

	luaJIT_setmode(L, -1, LUAJIT_MODE_FUNC|LUAJIT_MODE_OFF);
	return 1;
}

extern "C" int loader_XC (lua_State *L) {
	const char *funcname;
	const char *filename;
	const char *name = luaL_checkstring(L, 1);
	filename = x_findfile(L, name, "cpath");
	if (filename == NULL) return 1;  /* library not found in this path */

	char* tpath = (char*) calloc(_MAX_PATH, sizeof(char));

	LPRDATA rdPtr = XLuaGlobal::Get().GetStateByState(L)->rdList.front()->rdPtr;
	rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvGetFile(filename, tpath, 0);

	funcname = mkfuncname(L, name);
	if (ll_loadfunc(L, tpath, funcname) != 0) {
		free(tpath);
	    loaderror(L, filename);
		return 0;
	}

	free(tpath);
	return 1;
}

static const int sentinel_ = 0;
#define sentinel	((void *)&sentinel_)

extern "C" int loader_XCDirect (lua_State *L, const char* filename, const char* name) {
	const char *funcname;
	//const char *filename = luaL_checkstring(L, 1);
	//const char *name = luaL_checkstring(L, 2);
	DWORD dwsz;

	// Check existance of module
	LPRDATA rdPtr = XLuaGlobal::Get().GetStateByState(L)->rdList.front()->rdPtr;
	HANDLE hFile = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvOpenHFile(filename, &dwsz, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		return 1;
	}
	rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvCloseHFile(hFile);

	// Extract + Load module
	char* tpath = (char*) calloc(_MAX_PATH, sizeof(char));
	rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvGetFile(filename, tpath, 0);

	funcname = mkfuncname(L, name);
	if (ll_loadfunc(L, tpath, funcname) != 0) {
		free(tpath);
	    loaderror(L, filename);
		return 0;
	}
	free(tpath);

	return 1;
}

extern "C" int x_require_direct (lua_State *L) {
	const char *name = luaL_checkstring(L, 1);
	const char *filename = luaL_checkstring(L, 2);

	lua_settop(L, 2);  /* _LOADED table will be at index 3 */
	lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
	lua_getfield(L, 3, name);
	if (lua_toboolean(L, -1)) {  /* is it there? */
		if (lua_touserdata(L, -1) == sentinel)  /* check loops */
			luaL_error(L, "loop or previous error loading module " LUA_QS, name);
		return 1;  /* package is already loaded */
	}
  
	loader_XCDirect(L, filename, name);
    
    if (lua_isstring(L, -1))  /* loader returned error message? */
		luaL_error(L, "module " LUA_QS " not found:%s", name, lua_tostring(L, -2));
	else if (!lua_isfunction(L, -1))  /* did it find module? */
		luaL_error(L, "module " LUA_QS " not found:%s", name, lua_tostring(L, -2));

	lua_pushlightuserdata(L, sentinel);
	lua_setfield(L, 3, name);  /* _LOADED[name] = sentinel */
	lua_pushstring(L, name);  /* pass name as argument to module */
	lua_call(L, 1, 1);  /* run loaded module */
	if (!lua_isnil(L, -1))  /* non-nil return? */
		lua_setfield(L, 3, name);  /* _LOADED[name] = returned value */
	lua_getfield(L, 3, name);
	if (lua_touserdata(L, -1) == sentinel) {   /* module did not set a value? */
		lua_pushboolean(L, 1);  /* use true as result */
		lua_pushvalue(L, -1);  /* extra copy to be returned */
		lua_setfield(L, 3, name);  /* _LOADED[name] = true */
	}

	return 1;
}