#ifndef XLUA_OBJECT_H_
#define XLUA_OBJECT_H_

#include	<string>
#include	<queue>
#include	<hash_map>

#include	<windows.h>

#include	"Lua/lua.hpp"
#include	"Lua/lstate.h"

typedef struct tagRDATA RUNDATA;
typedef RUNDATA* LPRDATA;

typedef struct tagRO RunObject;
typedef RunObject* LPRO;

// =====================================================================================
// XLua Object Manager
// =====================================================================================

class XLuaState;

/**
 * ScriptRecord is the storage container for embedding scripts directly into the XLua
 * object.  It is separate but related to the ScriptData object which is used at edittime
 * to represent scripts being edited.
 */

class ScriptRecord {
public:

	std::string		name;
	std::string		script;
	int				loadAtStart;

	ScriptRecord ()
		: loadAtStart(0) {
	}
};

/**
 * XLuaObject represents a single XLua object, but does not in itself deal with Lua or
 * Lua state.  It stores runtime settings that apply on a per-object basis instead of a
 * per-state basis, as well as the print and error queues.  It can be bound to a single
 * XLuaState object.
 */

class XLuaObject {
public:

	// Bound XLuaState
	XLuaState*		state;

	// Pointer to XLua object
	LPRDATA			rdPtr;

	// Print and error queues + related
	std::queue<std::string>		errorQueue;
	std::queue<std::string>		printQueue;
	std::string					errorString;
	std::string					lastErrorString;
	std::string					printString;
	std::string					lastPrintString;

	// Runtime configuration
	int					errMode;
	int					printMode;
	bool				runScript;
	bool				autoReg;
	bool				useBacktrace;

	// Bound XLuaState identifier
	int					stateId;

	// Collection of embedded scripts keyed by script name
	stdext::hash_map<std::string, ScriptRecord>	scripts;

	typedef stdext::hash_map<std::string, ScriptRecord>::iterator	IScripts;

public:

	XLuaObject ();
	~XLuaObject ();

	void				RaiseError (const std::string& str);
	void				RaisePrint (const std::string& str);
	const std::string&	GetError ();
	const std::string&	GetPrint ();

	void				BindState (int sid);
	void				UnbindState ();

	std::string			GetEmbeddedScript (const char* name);
};

// Global Interface for Modules

LPRO xlua_get_object (lua_State* L, int objid);

// Helper Functions

template<class T>
inline T* PtrAddBytes(T *ptr, int bytes)
{
	return (T*)( (__int8*)ptr + bytes );
}

#endif /* XLuaObject_H_ */