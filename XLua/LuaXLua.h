#ifndef LUAXLUA_H_
#define LUAXLUA_H_

#include <set>
#include <map>

class ExportEntry {
public:

	int func_id;
	std::set<int> allowed_states;

	ExportEntry () : func_id(0) { }
};

class LuaXLua {
public:

	enum ControlledAccess {
		REMOTE_LOAD = 1,
		REMOTE_DESTROY = 2,
	};

	XLuaState*	_state;
	std::map<std::string, ExportEntry> exports;

	std::set<int> rload_states;
	std::set<int> destroy_states;

	typedef std::map<std::string, ExportEntry>::iterator IExports;
	typedef std::set<int>::iterator ISid;

	LuaXLua () : _state(0), exports(), rload_states() { }

	void SetState (XLuaState* L) {
		_state = L;
	}

	void Register ();

	void SetEnumInt (const char* name, int val);
	
	static int pass (lua_State* L);
	static int fail (lua_State* L);

	static int exportFunction (lua_State* L);
	static int clearExport (lua_State* L);

	static int grant (lua_State* L);
	static int revoke (lua_State* L);

	static int rpc (lua_State* L);

	static int rload (lua_State* L);
	static int rloadFile (lua_State* L);

	static int newState (lua_State* L);
	static int deleteState (lua_State* L);

protected:

	static std::map<const void*, int> tableRef;
	typedef std::map<const void*, int>::iterator ITableRef;

	static void stackCopy (lua_State* src, lua_State* dst, int si, int ei);
	static void tableCopy (lua_State* src, lua_State* dst, int si);
};

#endif