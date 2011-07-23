#include <sstream>

#include "Common.h"
#include "EditSetup.h"

extern "C" int luaJIT_setmode(lua_State *L, int idx, int mode);

XLuaState::XLuaState ()
	: state(NULL), stack(), rdList(), dsCount(0), ready(false), queueRd(0), queueDelete(false)
#ifdef XLUA_LEGACY
	  , mmfi(), xlua(), objects(), localValues(), localStrings(), winExports(), mmfiReady(false), winiReady(false)
#endif
	{
	unsigned fpcontrol = fp_precision_double();

	state = luaL_newstate();
	if (state == NULL) {
		return;
	}

	stack.SetState(this);

#ifdef XLUA_LEGACY
	mmfi.SetState(this);
	xlua.SetState(this);
#endif

	luaopen_base(state);
	luaJIT_setmode(state, 0, 0);

	// Register DoCall function
	lua_pushcfunction(state, XLuaState::LuaC_DoCall);
	lua_setglobal(state, "DoCall");

	lua_pushcfunction(state, XLuaState::LuaC_DoCallA);
	lua_setglobal(state, "DoCallA");
	lua_pushcfunction(state, XLuaState::LuaC_DoCallB);
	lua_setglobal(state, "DoCallB");
	lua_pushcfunction(state, XLuaState::LuaC_DoCallC);
	lua_setglobal(state, "DoCallC");
	lua_pushcfunction(state, XLuaState::LuaC_DoCallD);
	lua_setglobal(state, "DoCallD");
	lua_pushcfunction(state, XLuaState::LuaC_DoCallE);
	lua_setglobal(state, "DoCallE");
	lua_pushcfunction(state, XLuaState::LuaC_DoCallF);
	lua_setglobal(state, "DoCallF");
	lua_pushcfunction(state, XLuaState::LuaC_DoCallG);
	lua_setglobal(state, "DoCallG");

	// REgister print function
	lua_pushcfunction(state, XLuaState::LuaC_Print);
	lua_setglobal(state, "print");

#ifdef XLUA_LEGACY
	xlua.Register();
#endif

	fp_precision_restore(fpcontrol);
	ready = true;
}

XLuaState::~XLuaState () {
	ready = false;

#ifdef XLUA_LEGACY
	winiReady = false;
	mmfiReady = false;
#endif

	lua_close(state);

#ifdef XLUA_LEGACY
	XLuaGlobal::Get()._winiStates.remove(this);
#endif

	for (IRdList i_rd = rdList.begin(); i_rd != rdList.end(); i_rd++) {
		if (*i_rd != NULL) {
			(*i_rd)->state = NULL;
		}
	}
	rdList.clear();
}

//-----------------------------------------------------------------------------

void XLuaState::BindRd (XLuaObject* rd) {
	if (rd == NULL) {
		return;
	}

	for (IRdList i_rd = rdList.begin(); i_rd != rdList.end(); i_rd++) {
		if (*i_rd == rd) {
			return;
		}
	}

	rdList.push_back(rd);
}

void XLuaState::UnbindRd (XLuaObject* rd) {
	if (stack.Level() > 0) {
		queueRd = rd;
		return;
	}

	for (IRdList i_rd = rdList.begin(); i_rd != rdList.end(); i_rd++) {
		if (*i_rd == rd) {
			rdList.erase(i_rd);
			return;
		}
	}
}

void XLuaState::ReleaseRd () {
	if (!queueRd && !queueDelete) return;

	if (queueDelete) {
		for (IRdList i_rd = rdList.begin(); i_rd != rdList.end(); i_rd++) {
			(*i_rd)->state = 0;
		}
		rdList.clear();
		return;
	}

	for (IRdList i_rd = rdList.begin(); i_rd != rdList.end(); i_rd++) {
		if (*i_rd == queueRd) {
			rdList.erase(i_rd);
			queueRd = 0;
			return;
		}
	}

	queueRd = 0;
}

//-----------------------------------------------------------------------------

void XLuaState::RaiseError (const std::string& str) {
	for (IRdList i_rd = rdList.begin(); i_rd != rdList.end(); i_rd++) {
		if (*i_rd != NULL) {
			(*i_rd)->RaiseError(str);
		}
	}
}

void XLuaState::RaisePrint (const std::string& str) {
	for (IRdList i_rd = rdList.begin(); i_rd != rdList.end(); i_rd++) {
		if (*i_rd != NULL) {
			(*i_rd)->RaisePrint(str);
		}
	}
}

//-----------------------------------------------------------------------------

void XLuaState::LoadFile (const char* str) {
	unsigned fpcontrol = fp_precision_double();
	int stacktop = lua_gettop(state);

	// Running code is like a function call
	stack.PushLuaFrame();

	int errCode = luaL_loadfile(state, str);

	stack.PopFrame();

	if (errCode) {
		switch (errCode) {
			case LUA_ERRFILE:	RaiseError(lua_tostring(state, -1)); break;
			case LUA_ERRMEM:	RaiseError(lua_tostring(state, -1)); break;
			case LUA_ERRSYNTAX:	RaiseError(lua_tostring(state, -1)); break;
		}

		lua_settop(state, stacktop);
		fp_precision_restore(fpcontrol);
		return;
	}

	// Setup erroor handler
	int base = lua_gettop(state);
	lua_pushcfunction(state, XLuaState::LuaC_Error);
	lua_insert(state, base);

	lua_pcall(state, 0, LUA_MULTRET, base);
	lua_remove(state, base);

	lua_settop(state, stacktop);
	fp_precision_restore(fpcontrol);

	// If we're waiting to die and we finished unwinding
	if (stack.Level() == 0 && queueDelete) {
		XLuaGlobal::Get().DeleteState(this);
	}
}

void XLuaState::LoadString (const char* str, const char* sname) {
	unsigned fpcontrol = fp_precision_double();
	int stacktop = lua_gettop(state);

	int sz = strlen(str);

	// Running code is like a function call
	stack.PushLuaFrame();

	std::string chunkname;
	if (sname) {
		chunkname = sname;
	}
	else {
		std::stringstream name; name << "Run String " << dsCount++;
		chunkname = name.str();
	}
	int errCode = luaL_loadbuffer(state, str, sz, chunkname.c_str());

	stack.PopFrame();

	if (errCode) {
		switch (errCode) {
			case LUA_ERRMEM:	RaiseError(lua_tostring(state, -1)); break;
			case LUA_ERRSYNTAX:	RaiseError(lua_tostring(state, -1)); break;
		}

		lua_settop(state, stacktop);
		fp_precision_restore(fpcontrol);
		return;
	}

	// Setup erroor handler
	int base = lua_gettop(state);
	lua_pushcfunction(state, XLuaState::LuaC_Error);
	lua_insert(state, base);

	lua_pcall(state, 0, LUA_MULTRET, base);
	lua_remove(state, base);

	lua_settop(state, stacktop);
	fp_precision_restore(fpcontrol);

	// If we're waiting to die and we finished unwinding
	if (stack.Level() == 0 && queueDelete) {
		XLuaGlobal::Get().DeleteState(this);
	}
}

void XLuaState::LoadEmbedded (const char* name, LPRDATA rdPtr) {
	XLuaObject::IScripts si = rdPtr->luaMan->scripts.find(name);
	if (si == rdPtr->luaMan->scripts.end())
		return;

	LoadString(si->second.script.c_str(), si->second.name.c_str());
}

void XLuaState::LoadDefaultLib(XLuaState::LuaPackage pkg) {
	unsigned fpcontrol = fp_precision_double();
	int stat = 0;

	// Running code is like a function call
	stack.PushLuaFrame();

	switch (pkg) {
		case PACKAGE_ALL: luaL_openlibs(state); break;
		case PACKAGE_BASE: stat = lua_cpcall(state, luaopen_base, 0); break;
		case PACKAGE_PACKAGE: stat = lua_cpcall(state, luaopen_package, 0); break;
		case PACKAGE_TABLE: stat = lua_cpcall(state, luaopen_table, 0); break;
		case PACKAGE_OS: stat = lua_cpcall(state, luaopen_os, 0); break;
		case PACKAGE_MATH: stat = lua_cpcall(state, luaopen_math, 0); break;
		case PACKAGE_STRING: stat = lua_cpcall(state, luaopen_string, 0); break;
		case PACKAGE_DEBUG: stat = lua_cpcall(state, luaopen_debug, 0); break;
		case PACKAGE_JIT: stat = lua_cpcall(state, luaopen_jit, 0); break;
		case PACKAGE_BIT: stat = lua_cpcall(state, luaopen_bit, 0); break;
#ifdef XLUA_LEGACY
		case PACKAGE_XLUA: stat = 0; xlua.Register(); break;
#endif
		default: break;
	}

	if (stat) {
		RaiseError(lua_tostring(state, -1));
	}

	stack.PopFrame();

	fp_precision_restore(fpcontrol);
}

extern "C" int load_XCDirect (lua_State *L, const char* path, const char* name);

void XLuaState::LoadCModule (const char* path, const char* name) {
	unsigned fpcontrol = fp_precision_double();
	int stacktop = lua_gettop(state);

	// Running code is like a function call
	stack.PushLuaFrame();

	// Check if package module is loaded, and load it if necessary
	lua_getfield(state, LUA_GLOBALSINDEX, "package");
	if (!lua_istable(state, -1)) {
		if (lua_cpcall(state, luaopen_package, 0) != 0) {
			RaiseError(lua_tostring(state, -1));
			lua_settop(state, stacktop);

			stack.PopFrame();
			fp_precision_restore(fpcontrol);
			return;
		}
	}

	lua_getfield(state, LUA_GLOBALSINDEX, "x_requirelib");
	lua_pushstring(state, name);
	lua_pushstring(state, path);

	if (lua_pcall(state, 2, 1, 0) != 0) {
		RaiseError(lua_tostring(state, -1));
	}

	stack.PopFrame();
	
	lua_settop(state, stacktop);
	fp_precision_restore(fpcontrol);
}

//-----------------------------------------------------------------------------

void XLuaState::EnableJIT () {
	if (lua_cpcall(state, &XLuaState::LuaC_SetupJIT, 0)) {
		RaiseError(lua_tostring(state, -1));
	}
}

//-----------------------------------------------------------------------------

void XLuaState::RegisterFunction (const char* func, int minParams, int funcGroup) {
	unsigned fpcontrol = fp_precision_double();

	if (!funcGroup) {
		funcGroup = MF_FUNCTION_GROUP_DEF;
	}

	lua_pushstring(state, func);

	lua_pushstring(state, func);
	lua_pushinteger(state, funcGroup);
	lua_pushinteger(state, minParams);
	lua_pushcclosure(state, XLuaState::LuaC_RegCall, 3);

	lua_rawset(state, LUA_GLOBALSINDEX);

	fp_precision_restore(fpcontrol);
}

#ifdef XLUA_LEGACY
void XLuaState::ExportObject (int id, LPRO obj) {
	unsigned fpcontrol = fp_precision_double();
	int stacktop = lua_gettop(state);

	MFObject opair;
	opair.obj = obj;
	opair.fixed = (obj->roHo.hoCreationId<<16)+obj->roHo.hoNumber;
	opair.frame = obj->roHo.hoAdRunHeader->rhFrame;

	objects[id] = opair;

	lua_getglobal(state, "mmf");
	if (lua_isnil(state, -1)) {
		lua_settop(state, stacktop);
		fp_precision_restore(fpcontrol);
		return;
	}
	lua_getfield(state, -1, "ExportCallback");
	if (lua_isnil(state, -1)) {
		lua_settop(state, stacktop);
		fp_precision_restore(fpcontrol);
		return;
	}

	lua_pushinteger(state, id);
	if (lua_pcall(state, 1, 0, 0)) {
		RaiseError("Could not call user-defined export callback");
	}

	lua_settop(state, stacktop);
	fp_precision_restore(fpcontrol);
}
#endif

#ifdef XLUA_LEGACY
void XLuaState::ExportObjectByFixed (int id, int fixed, LPRDATA rdPtr) {
	unsigned fpcontrol = fp_precision_double();
	int stacktop = lua_gettop(state);

	LPOBL objList = rdPtr->rHo.hoAdRunHeader->rhObjectList;
	objectsList e = objList[0x0000FFFF&fixed];
	if (!e.oblOffset) {
		lua_settop(state, stacktop);
		fp_precision_restore(fpcontrol);
		return;
	}

	MFObject opair;
	opair.obj = (LPRO)e.oblOffset;
	opair.fixed = fixed;
	opair.frame = rdPtr->rHo.hoAdRunHeader->rhFrame;

	objects[id] = opair;

	lua_getglobal(state, "mmf");
	if (lua_isnil(state, -1)) {
		lua_settop(state, stacktop);
		fp_precision_restore(fpcontrol);
		return;
	}
	lua_getfield(state, -1, "ExportCallback");
	if (lua_isnil(state, -1)) {
		lua_settop(state, stacktop);
		fp_precision_restore(fpcontrol);
		return;
	}

	lua_pushinteger(state, id);
	if (lua_pcall(state, 1, 0, 0)) {
		RaiseError("Could not call user-defined export callback");
	}

	lua_settop(state, stacktop);
	fp_precision_restore(fpcontrol);
}
#endif

#ifdef XLUA_LEGACY
LPRO XLuaState::GetExport (int id) {
	IObjects iter = objects.find(id);
	if (iter == objects.end()) {
		std::stringstream str;
		str << "No object with ID " << id;
		lua_pushstring(state, str.str().c_str());
		lua_error(state);
		return NULL;
	}

	return iter->second.obj;
}
#endif

#ifdef XLUA_LEGACY
void XLuaState::ClearExport (int id) {
	IObjects iter = objects.find(id);
	if (iter != objects.end()) {
		objects.erase(iter);
	}
}
#endif

#ifdef XLUA_LEGACY
void XLuaState::DestroyCallback (int id) {
	unsigned fpcontrol = fp_precision_double();
	int stacktop = lua_gettop(state);

	lua_getglobal(state, "mmf");
	if (lua_isnil(state, -1)) {
		lua_settop(state, stacktop);
		fp_precision_restore(fpcontrol);
		return;
	}
	lua_getfield(state, -1, "DestroyCallback");
	if (lua_isnil(state, -1)) {
		lua_settop(state, stacktop);
		fp_precision_restore(fpcontrol);
		return;
	}

	lua_pushinteger(state, id);
	if (lua_pcall(state, 1, 0, 0)) {
		RaiseError("Could not call user-defined destroy callback");
	}

	lua_settop(state, stacktop);
	fp_precision_restore(fpcontrol);
}
#endif

//-----------------------------------------------------------------------------

// Following selection code based on DynaSoft's Select Object.
// Thanks for all information on the selection system!

void XLuaState::SaveObjectSelection (const char* loc) {
	unsigned fpcontrol = fp_precision_double();
	int stacktop = lua_gettop(state);

	LPRDATA rdPtr = rdList.front()->rdPtr;
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPOIL oiList = rhPtr->rhOiList;
	LPOBL objList = rhPtr->rhObjectList;

	int selectionCount = 0;
	lua_newtable(state);

	for (int i = 0; i < rhPtr->rhNumberOi; i++) {
		LPOIL currentOi = oiList + i;
		if (currentOi->oilEventCount != rhPtr->rh2.rh2EventCount) {
			continue;
		}
		if (currentOi->oilNumOfSelected <= 0) {
			continue;
		}

		headerObject* curObject = objList[currentOi->oilListSelected].oblOffset;
		while (1) {
			if (curObject->hoIdentifier == 'XLUA') {
				break;
			}

			lua_pushinteger(state, (curObject->hoCreationId << 16) + curObject->hoNumber);
			lua_rawseti(state, -2, ++selectionCount);

			if (curObject->hoNextSelected >= 0) {
				curObject = objList[curObject->hoNextSelected].oblOffset;
			}
			else {
				break;
			}
		}
	}

	SetField(loc);

	lua_settop(state, stacktop);
	fp_precision_restore(fpcontrol);
}

void XLuaState::SaveObjectSelectionType (eventParam* param, const char* loc) {
	unsigned fpcontrol = fp_precision_double();

	short oiNum = param->evp.evpW.evpW0;

	LPRDATA rdPtr = rdList.front()->rdPtr;
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPOIL oiList = rhPtr->rhOiList;
	LPOBL objList = rhPtr->rhObjectList;
	LPQOI qualList = rhPtr->rhQualToOiList;

	int selectionCount = 0;
	lua_newtable(state);
	
	if (oiNum & 0x8000) {
		LPQOI curQualStart = PtrAddBytes(qualList, oiNum & 0x7FFF);
		LPQOI curQual = curQualStart;

		for (curQual; curQual->qoiOiList >= 0; curQual = PtrAddBytes(curQual, 4)) {
			LPOIL currentOi = oiList + curQual->qoiOiList;
			if (currentOi->oilEventCount != rhPtr->rh2.rh2EventCount) {
				continue;
			}
			if (currentOi->oilNumOfSelected <= 0) {
				continue;
			}

			headerObject* curObject = objList[currentOi->oilListSelected].oblOffset;
			while (1) {
				lua_pushinteger(state, (curObject->hoCreationId << 16) + curObject->hoNumber);
				lua_rawseti(state, -2, ++selectionCount);

				if (curObject->hoNextSelected >= 0) {
					curObject = objList[curObject->hoNextSelected].oblOffset;
				}
				else {
					break;
				}
			}
		}
	}
	else {
		while (1) {
			LPOIL currentOi = oiList + oiNum;
			if (currentOi->oilEventCount != rhPtr->rh2.rh2EventCount) {
				break;
			}
			if (currentOi->oilNumOfSelected <= 0) {
				break;
			}

			headerObject* curObject = objList[currentOi->oilListSelected].oblOffset;
			while (1) {
				lua_pushinteger(state, (curObject->hoCreationId << 16) + curObject->hoNumber);
				lua_rawseti(state, -2, ++selectionCount);

				if (curObject->hoNextSelected >= 0) {
					curObject = objList[curObject->hoNextSelected].oblOffset;
				}
				else {
					break;
				}
			}

			break;
		}
	}

	SetField(loc);

	fp_precision_restore(fpcontrol);
}

void XLuaState::RestoreObjectSelection (const char* loc) {
	unsigned fpcontrol = fp_precision_double();
	int stacktop = lua_gettop(state);

	LPRDATA rdPtr = rdList.front()->rdPtr;
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPOIL oiList = rhPtr->rhOiList;
	LPOBL objList = rhPtr->rhObjectList;

	if (!GetField(loc)) {
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return;
	}

	if (!lua_istable(state, -1)) {
		RaiseError(std::string(loc) + " not a table");
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return;
	}

	int sp = lua_gettop(state);
	stdext::hash_map<short, short> oiMap;

	lua_pushnil(state);  /* first key */
	while (lua_next(state, sp)) {
		if (lua_type(state, -1) == LUA_TNUMBER) {
			int fixed = lua_tointeger(state, -1);
			lua_pop(state, 1);

			headerObject* obj = (headerObject*) objList[0x0000FFFF&fixed].oblOffset;
			if (obj == 0) {
				continue;
			}

			LPOIL objOi = oiList + obj->hoOi;
			oiMap[obj->hoOi]++;

			if (oiMap[obj->hoOi] <= 1) {
				obj->hoNextSelected = -1;
			}
			else {
				obj->hoNextSelected = objOi->oilListSelected;
			}

			objOi->oilEventCount = rhPtr->rh2.rh2EventCount;
			objOi->oilListSelected = obj->hoNumber;
			objOi->oilNumOfSelected = oiMap[obj->hoOi];
		}
	}

	lua_pop(state, 1);

	fp_precision_restore(fpcontrol);
}

//-----------------------------------------------------------------------------

#ifdef XLUA_LEGACY
void XLuaState::SetLocalValue (unsigned id, double v) {
	localValues[id] = v;
}

void XLuaState::SetLocalString (unsigned id, const std::string& str) {
	localStrings[id] = str;
}

double XLuaState::GetLocalValue (unsigned id) {
	return localValues[id];
}

const std::string& XLuaState::GetLocalString (unsigned id) {
	return localStrings[id];
}
#endif

//-----------------------------------------------------------------------------

void XLuaState::SetNumericVariable (const char* name, int n) {
	unsigned fpcontrol = fp_precision_double();

	lua_pushinteger(state, n);
	SetField(name);

	fp_precision_restore(fpcontrol);
}

void XLuaState::SetFloatingVariable (const char* name, double v) {
	unsigned fpcontrol = fp_precision_double();

	lua_pushnumber(state, v);
	SetField(name);

	fp_precision_restore(fpcontrol);
}

void XLuaState::SetStringVariable (const char* name, const char* str) {
	unsigned fpcontrol = fp_precision_double();

	lua_pushstring(state, str);
	SetField(name);

	fp_precision_restore(fpcontrol);
}

void XLuaState::SetBooleanVariable (const char* name, bool b) {
	unsigned fpcontrol = fp_precision_double();

	lua_pushboolean(state, b);
	SetField(name);

	fp_precision_restore(fpcontrol);
}

void XLuaState::SetNilVariable (const char* name) {
	unsigned fpcontrol = fp_precision_double();

	lua_pushnil(state);
	SetField(name);

	fp_precision_restore(fpcontrol);
}

int XLuaState::GetNumericVariable (const char* name) {
	unsigned fpcontrol = fp_precision_double();

	if (!GetField(name)) {
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return 0;
	}

	if (!lua_isnumber(state, -1)) {
		RaiseError(std::string(name) + " not a number");
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return 0;
	}

	int result = lua_tointeger(state, -1);
	lua_pop(state, 1);

	fp_precision_restore(fpcontrol);
	return result;
}

double XLuaState::GetFloatingVariable (const char* name) {
	unsigned fpcontrol = fp_precision_double();

	if (!GetField(name)) {
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return 0;
	}

	if (!lua_isnumber(state, -1)) {
		RaiseError(std::string(name) + " not a number");
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return 0;
	}

	double result = lua_tonumber(state, -1);
	lua_pop(state, 1);

	fp_precision_restore(fpcontrol);
	return result;
}

std::string XLuaState::GetStringVariable (const char* name) {
	unsigned fpcontrol = fp_precision_double();

	if (!GetField(name)) {
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return "";
	}

	if (!lua_isstring(state, -1)) {
		RaiseError(std::string(name) + " not a string");
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return "";
	}

	const char* result = lua_tostring(state, -1);
	lua_pop(state, 1);

	fp_precision_restore(fpcontrol);
	return result;
}

bool XLuaState::GetBooleanVariable (const char* name) {
	unsigned fpcontrol = fp_precision_double();

	if (!GetField(name)) {
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return 0;
	}

	int result = lua_toboolean(state, -1);
	lua_pop(state, 1);

	fp_precision_restore(fpcontrol);
	return result != 0;
}

std::string XLuaState::GetVariableType (const char* name) {
	unsigned fpcontrol = fp_precision_double();

	if (!GetField(name)) {
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return "nil";
	}

	const char* result = luaL_typename(state, -1);
	lua_pop(state, 1);

	fp_precision_restore(fpcontrol);
	return result;
}

int XLuaState::GetTableLength (const char* name) {
	unsigned fpcontrol = fp_precision_double();

	if (!GetField(name)) {
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return 0;
	}

	if (!lua_istable(state, -1)) {
		RaiseError(std::string(name) + " not a table");
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return 0;
	}

	int result = lua_objlen(state, -1);
	lua_pop(state, 1);

	fp_precision_restore(fpcontrol);
	return result;
}

int XLuaState::GetTableMaxIndex (const char* name) {
	unsigned fpcontrol = fp_precision_double();

	if (!GetField(name)) {
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return 0;
	}

	if (!lua_istable(state, -1)) {
		RaiseError(std::string(name) + " not a table");
		lua_pop(state, 1);
		fp_precision_restore(fpcontrol);
		return 0;
	}

	int sp = lua_gettop(state);

	// Taken from ltablib.c

	int max = 0;
	lua_pushnil(state);  /* first key */
	while (lua_next(state, sp)) {
		lua_pop(state, 1);  /* remove value */
		if (lua_type(state, -1) == LUA_TNUMBER) {
			int v = lua_tointeger(state, -1);
			if (v > max) max = v;
		}
	}

	lua_pop(state, 1);

	fp_precision_restore(fpcontrol);
	return max;
}

//-----------------------------------------------------------------------------

#ifdef XLUA_LEGACY
void XLuaState::ResetLocalStorage () {
	localValues.clear();
	localStrings.clear();
}

void XLuaState::ResetObjectExports () {
	objects.clear();
}

void XLuaState::ResetWinExports () {
	winExports.clear();
}
#endif

//-----------------------------------------------------------------------------

//-----------
// FieldEntry
//-----------

TaggedType XLuaState::FieldEntry (const std::string& name) {
	if (name[name.size()-1] == ']') {
		short off = 0;
		if (name[0] == '[') {
			off = 1;
		}

		if (name[off] == '\'' || name[off] == '"') {
			return TaggedType(name.substr((1 + off), name.size() - (3 + off)));
		}
		else {
			return TaggedType(atoi(name.substr(off, name.size() - (1 + off)).c_str()));
		}
	}

	return TaggedType(name);
}

//----------
// SeekField
//----------
// Takes a fully qualified variables name (e.g. x.y.z), sets the top of the stack equal to the table
// x.y, and returns z as the field name.  If necessary, x or y are created in the process.  If x or y
// are non-table values, an error is raised and the empty string is returned.

TaggedType XLuaState::SeekField (const std::string& name) {
	int dotIndex = name.find_first_of(".[");
	int prevIndex = 0;
	int top = lua_gettop(state);

	// Put global table onto stack
	lua_getglobal(state, "_G");

	// If no . found, stay put

	while (dotIndex != std::string::npos) {
		// Silently ignore erroneous sequences like .. or .[
		if (dotIndex == prevIndex) {
			prevIndex = dotIndex + 1;
			dotIndex = name.find_first_of(".[", prevIndex);
			continue;
		}

		std::string element = name.substr(prevIndex, (dotIndex - prevIndex));
		TaggedType entry = FieldEntry(element);

		// get current.element
		switch (entry.type) {
			case TaggedType::FIELD_STRING:
				lua_pushstring(state, entry.cstr);
				lua_gettable(state, -2);
				break;
			case TaggedType::FIELD_INT:
				lua_rawgeti(state, -1, entry.i);
				break;
		}

		// If table is absent
		if (lua_isnil(state, -1)) {

			// Create the table
			lua_pop(state, 1);
			lua_newtable(state);

			// Push the table to the top of the stack
			switch (entry.type) {
				case TaggedType::FIELD_STRING:
					lua_setfield(state, -2, entry.cstr);
					lua_getfield(state, -1, entry.cstr);
					break;
				case TaggedType::FIELD_INT:
					lua_rawseti(state, -2, entry.i);
					lua_rawgeti(state, -1, entry.i);
					break;
			}
		}

		// If field exists but is not a table, report error and abort
		else if (!lua_istable(state, -1)) {
			std::stringstream str;
			str << "Attempt to index field '" << element << "' (a " << TranslateLuaType(lua_type(state, -1)) << " value)";
			RaiseError(str.str());
			return TaggedType();
		}

		lua_remove(state, top + 1);

		// Move onto next value
		prevIndex = dotIndex + 1;
		dotIndex = name.find_first_of(".[", prevIndex);
	}

	return FieldEntry(name.substr(prevIndex, name.size() - prevIndex));
}

//----------
// GetField
//----------
// Takes a fully qualified variables name (e.g. x.y.z), performs a SeekField,
// then returns the value of the leaf field on the stack

bool XLuaState::GetField (const std::string& name) {
	int top = lua_gettop(state);
	TaggedType field = SeekField(name);

	switch (field.type) {
		case TaggedType::FIELD_STRING:
			lua_getfield(state, -1, field.cstr);
			break;
		case TaggedType::FIELD_INT:
			lua_rawgeti(state, -1, field.i);
			break;
		default:
			lua_pop(state, 1);
			lua_pushnil(state);
			return false;
	}

	lua_remove(state, top + 1);

	return true;
}

//----------
// SetField
//----------
// Takes a fully qualified variables name (e.g. x.y.z), performs a SeekField,
// then sets the value of the leaf field from the top stack value

bool XLuaState::SetField (const std::string& name) {
	int top = lua_gettop(state);
	TaggedType field = SeekField(name);

	lua_pushvalue(state, top);

	switch (field.type) {
		case TaggedType::FIELD_STRING:
			lua_setfield(state, -2, field.cstr);
			break;
		case TaggedType::FIELD_INT:
			lua_rawseti(state, -2, field.i);
			break;
		default:
			lua_pop(state, 2);
			return false;
	}

	lua_pop(state, 2);

	return true;
}

std::string XLuaState::TranslateLuaType (int type) {
	switch (type) {
		case LUA_TNIL: return "nil";
		case LUA_TBOOLEAN: return "boolean";
		case LUA_TLIGHTUSERDATA: return "lightuserdata";
		case LUA_TNUMBER: return "number";
		case LUA_TSTRING: return "string";
		case LUA_TTABLE: return "table";
		case LUA_TFUNCTION: return "function";
		case LUA_TUSERDATA: return "userdata";
		case LUA_TTHREAD: return "thread";
		default: return "unknown";
	}
}

//-----------------------------------------------------------------------------

int XLuaState::LuaC_RegCall (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	const char* func = lua_tostring(L, lua_upvalueindex(1));
	int funcGroup = lua_tointeger(L, lua_upvalueindex(2));

	int minParams = lua_tointeger(L, lua_upvalueindex(3));
	int actualParams = lua_gettop(L);
	if (actualParams < minParams) {
		luaL_error(L, "Function %s expected %d parameters, got %d", func, minParams, actualParams);
	}

	return state->stack.CallMMFFunction (func, funcGroup, L);
}

int XLuaState::DoCall (lua_State *L, int funcGroup) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	if (lua_gettop(L) < 1) {
		luaL_error(L, "DoCall: No function identifier passed");
		return 0;
	}

	const char* func = lua_tostring(L, 1);
	lua_remove(L, 1);

	return state->stack.CallMMFFunction(func, funcGroup, L);
}

int XLuaState::LuaC_DoCall (lua_State *L) {
	return XLuaState::DoCall(L, MF_FUNCTION_GROUP_DEF);
}

int XLuaState::LuaC_DoCallA (lua_State *L) {
	return XLuaState::DoCall(L, MF_FUNCTION_GROUP_A);
}

int XLuaState::LuaC_DoCallB (lua_State *L) {
	return XLuaState::DoCall(L, MF_FUNCTION_GROUP_B);
}

int XLuaState::LuaC_DoCallC (lua_State *L) {
	return XLuaState::DoCall(L, MF_FUNCTION_GROUP_C);
}

int XLuaState::LuaC_DoCallD (lua_State *L) {
	return XLuaState::DoCall(L, MF_FUNCTION_GROUP_D);
}

int XLuaState::LuaC_DoCallE (lua_State *L) {
	return XLuaState::DoCall(L, MF_FUNCTION_GROUP_E);
}

int XLuaState::LuaC_DoCallF (lua_State *L) {
	return XLuaState::DoCall(L, MF_FUNCTION_GROUP_F);
}

int XLuaState::LuaC_DoCallG (lua_State *L) {
	return XLuaState::DoCall(L, MF_FUNCTION_GROUP_G);
}

int XLuaState::LuaC_Print (lua_State* L) {
	int paramCount = lua_gettop(L);
	std::stringstream str;

	lua_getglobal(L, "tostring");
	for (int i = 1; i <= paramCount; i++) {
		lua_pushvalue(L, -1);
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		const char* s = lua_tostring(L, -1);  /* get result */

		if (s == NULL) {
			return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
		}

		if (i > 1) {
			str << "\t";
		}
		str << s;
		lua_pop(L, 1);
	}
	str << "\r\n";

	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	state->RaisePrint(str.str());

	return 0;
}

extern "C" int db_errorfb (lua_State *L);

int XLuaState::LuaC_Error (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL)
		return 0;

	if (!lua_isstring(L, 1))
		return 0;

	std::string message = lua_tostring(L, 1);
	std::string bt;

	// Manually raise errors depending on XLua settings
	for (XLuaState::IRdList i_rd = state->rdList.begin(); i_rd != state->rdList.end(); i_rd++) {
		XLuaObject* lm = *i_rd;
		if (lm != NULL) {
			if (lm->useBacktrace) {
				// Let's be lazy about any backtrace calculations
				if (bt.empty()) {
					lua_pushinteger(L, 1);
					db_errorfb(L);
					bt = lua_tostring(L, 1);
				}

				lm->RaiseError(bt + "\r\n");
			}
			else {
				lm->RaiseError(message + "\r\n");
			}
		}
	}
	//state->RaiseError(str.str());

	return 0;
}

#include "Lua/opt.h"
#include "Lua/opt_inline.h"

int XLuaState::LuaC_SetupJIT (lua_State* L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	luaopen_base(L);
	luaopen_package(L);
	luaopen_string(L);
	luaopen_jit(L);

	// Turn on JIT Engine
	lua_getglobal(L, "jit");
	lua_getfield(L, -1, "on");
	lua_call(L, 0, 0);

	// Load opt modules into preload
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");

	luaL_loadbuffer(L, (const char*)(&opt_lua_bytes), opt_lua_sz, "jit.opt");
	lua_setfield(L, -2, "jit.opt");

	luaL_loadbuffer(L, (const char*)(&opt_inline_lua_bytes), opt_inline_lua_sz, "jit.opt_inline");
	lua_setfield(L, -2, "jit.opt_inline");

	// Formally load optimizer
	lua_getglobal(L, "require");
	lua_pushliteral(L, "jit.opt");
	if (lua_pcall(L, 1, 1, 0)) {
		state->RaiseError("Could not load module jit.opt: Optimization is disabled.");
		return 0;
	}
	
	lua_getfield(L, -1, "start");
	if (lua_pcall(L, 0, 0, 0)) {
		state->RaiseError("Could not call jit.opt init function: Optimization is disabled.");
		return 0;
	}

	return 0;
}
