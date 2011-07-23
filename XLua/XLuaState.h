#ifndef XLUA_STATE_H_
#define XLUA_STATE_H_

#include	<hash_map>
#include	<list>

#include    "CallStack.h"
#include	"TaggedType.h"

#include	"Lua/lua.hpp"
#include	"Lua/lstate.h"

#ifdef XLUA_LEGACY
	#include	"LuaXLua.h"
	#include	"LuaMMF.h"
#endif

typedef struct tagRO RunObject;
typedef RunObject* LPRO;

typedef struct tagRDATA RUNDATA;
typedef RUNDATA* LPRDATA;

// =====================================================================================
// Lua State Manager
// =====================================================================================

class XLuaObject;
class CRunFrame;
class eventParam;

/**
 * Stores information related to each exported object.
 */

#ifdef XLUA_LEGACY
struct MFObject {
	LPRO			obj;
	DWORD			fixed;
	CRunFrame*		frame;
};
#endif

/**
 * XLuaState is the main Lua manager in XLua, and in addition to holding a valid
 * lua_State*, contains additional state-specific configuration and data used by XLua.
 * A single XLuaState may be bound to multiple XLuaObjects, and any action that interacts
 * with an XLuaObject will be repeated for each of them.
 */

class XLuaState {
public:

	// Lua vm state
	lua_State*		state;

	// Function call stack for this sate
	CallStack		stack;

	// List of bound XLuaObjects
	std::list<XLuaObject*>	rdList;

	// Assigned state identifier
	int				stateId;

	// Count of number of anonymous strings executed
	int				dsCount;

	// Initialization checks
	bool			ready;

	XLuaObject*		queueRd;
	bool			queueDelete;

	typedef std::list<XLuaObject*>::iterator	IRdList;

#ifdef XLUA_LEGACY
	// Additional interfaces to Lua
	LuaMMF			mmfi;
	LuaXLua			xlua;

	// Exported object list
	stdext::hash_map<int, MFObject>			objects;

	// Local storage value lists
	stdext::hash_map<int, double>			localValues;
	stdext::hash_map<int, std::string>		localStrings;

	// WIN Interface exported callback list
	stdext::hash_map<unsigned, unsigned>	winExports;

	// Initialization checks
	bool			mmfiReady;
	bool			winiReady;

	typedef stdext::hash_map<int, MFObject>::iterator		IObjects;
	typedef stdext::hash_map<unsigned, unsigned>::iterator	IWinExports;
#endif

	enum LuaPackage {
		PACKAGE_ALL, PACKAGE_BASE, PACKAGE_IO, PACKAGE_TABLE, PACKAGE_PACKAGE, PACKAGE_OS, PACKAGE_STRING, 
		PACKAGE_DEBUG, PACKAGE_MATH, PACKAGE_JIT, PACKAGE_BIT, PACKAGE_GL, PACKAGE_XLUA
	};

public:

	XLuaState ();
	~XLuaState ();

	// Binding to an XLuaObject
	void				BindRd (XLuaObject* rd);
	void				UnbindRd (XLuaObject* rd);
	void				ReleaseRd ();

	// Triggered error and print actions
	void				RaiseError (const std::string& str);
	void				RaisePrint (const std::string& str);

	// Source and module loading
	void				LoadFile (const char* str);
	void				LoadString (const char* str, const char* sname = 0);
	void				LoadEmbedded (const char* name, LPRDATA rdPtr);
	void				LoadDefaultLib (LuaPackage pkg);
	void				LoadCModule (const char* path, const char* name);

	// JIT compiler
	void				EnableJIT ();

	// MMF Function registration
	void				RegisterFunction (const char* func, int minParams, int funcGroup = 0);

	// MMF Internal seletion list manipulation
	void				SaveObjectSelection (const char* loc);
	void				SaveObjectSelectionType (eventParam* param, const char* loc);
	void				RestoreObjectSelection (const char* loc);

	// Directly update Lua global variables
	void				SetNumericVariable (const char* name, int n);
	void				SetFloatingVariable (const char* name, double v);
	void				SetStringVariable (const char* name, const char* str);
	void				SetBooleanVariable (const char* name, bool b);
	void				SetNilVariable (const char* name);

	// Directly read Lua global variables
	int					GetNumericVariable (const char* name);
	double				GetFloatingVariable (const char* name);
	std::string			GetStringVariable (const char* name);
	bool				GetBooleanVariable (const char* name);
	
	// Get additional lua variable information
	std::string			GetVariableType (const char* name);
	int					GetTableLength (const char* name);
	int					GetTableMaxIndex (const char* name);

	// Process Lua variable names
	TaggedType			FieldEntry (const std::string& name);
	TaggedType			SeekField (const std::string& name);
	bool				GetField (const std::string& name);
	bool				SetField (const std::string& name);

	// Translation
	std::string			TranslateLuaType (int type);

#ifdef XLUA_LEGACY
	// Object Exports
	void				ExportObject (int id, LPRO obj);
	void				ExportObjectByFixed (int id, int fixed, LPRDATA rdPtr);
	LPRO				GetExport (int id);
	void				ClearExport (int id);
	void				ResetObjectExports ();

	// WIN Interface
	void				DestroyCallback (int id);
	void				ResetWinExports ();

	// Local Storage
	void				SetLocalValue (unsigned id, double v);
	void				SetLocalString (unsigned id, const std::string& str);
	double				GetLocalValue (unsigned id);
	const std::string&	GetLocalString (unsigned id);
	void				ResetLocalStorage ();
#endif

	//---

	// Call registered MMF function from Lua
	static int			LuaC_RegCall (lua_State *L);

	// Call unregistered MMF function from Lua
	static int			DoCall (lua_State *L, int funcGroup);
	static int			LuaC_DoCall (lua_State *L);
	static int			LuaC_DoCallA (lua_State *L);
	static int			LuaC_DoCallB (lua_State *L);
	static int			LuaC_DoCallC (lua_State *L);
	static int			LuaC_DoCallD (lua_State *L);
	static int			LuaC_DoCallE (lua_State *L);
	static int			LuaC_DoCallF (lua_State *L);
	static int			LuaC_DoCallG (lua_State *L);

	// Updated Lua print and error capabilities
	static int			LuaC_Print (lua_State* L);
	static int			LuaC_Error (lua_State *L);

	// Turn on JIT
	static int			LuaC_SetupJIT (lua_State* L);

};

#endif
