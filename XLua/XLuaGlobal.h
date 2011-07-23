#ifndef XLUA_GLOBAL_H_
#define XLUA_GLOBAL_H_

#include <list>
#include <hash_map>

#include "Lua/lua.hpp"
#include "Lua/lstate.h"

// =====================================================================================
// XLua Global State Manager
// =====================================================================================

class XLuaState;

/**
 * XLuaGlobal is a globally instantiated object running the lifetime of both runtimes.
 * Its purpose is to bind XLuaState objects (representing independent Lua States) to
 * state identifiers, and make them available for lookup either by id, or by the Lua
 * state that they represent.
 */

class XLuaGlobal {
public:

	// Lookup tables
	stdext::hash_map<int, XLuaState*>			_stateTable;
	stdext::hash_map<lua_State*, XLuaState*>	_stateLookup;

	// Reverse lookup table
	stdext::hash_map<XLuaState*, int>			_idLookup;

#ifdef XLUA_LEGACY
	// List of XLuaStates with active WIN interfaces
	std::list<XLuaState*>						_winiStates;

	typedef std::list<XLuaState*>::iterator		IWiniState;
#endif

	typedef	stdext::hash_map<int, XLuaState*>::iterator	IStateMap;
	typedef stdext::hash_map<XLuaState*, int>::iterator	IIdMap;
	

public:

	XLuaGlobal ();
	~XLuaGlobal ();

	bool		CreateState (int sid);

	bool		DeleteState (int sid);
	bool		DeleteState (XLuaState* s);

	XLuaState*	GetState (int sid);
	XLuaState*	GetStateByState (lua_State* state);

	static XLuaGlobal& Get ();
};


//---------------------------------------------------------------------------------------
//
// Inline Implementation Below
//

//---------------------------------------------------------------------------------------
// XLuaGlobal

inline XLuaGlobal::XLuaGlobal () { }

inline XLuaGlobal::~XLuaGlobal () { }

inline XLuaState* XLuaGlobal::GetState (int sid) {
	return _stateTable[sid];
}

inline XLuaState* XLuaGlobal::GetStateByState (lua_State* state) {
	XLuaState* ret = _stateLookup[state];
	if (!ret)
		ret = _stateLookup[state->l_G->mainthread];
	return ret;
}

inline XLuaGlobal& XLuaGlobal::Get () {
	static XLuaGlobal* xg = new XLuaGlobal();
	return *xg;
}

#endif
