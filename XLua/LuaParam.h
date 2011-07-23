#ifndef LUA_PARAM_H_
#define LUA_PARAM_H_

#include <list>
#include <string>
#include <vector>

#include "Lua/lua.hpp"

enum LuaParamType {
	LUA_PARAM_NIL = 0x1,
	LUA_PARAM_INT = 0x2,
	LUA_PARAM_NUM = 0x4, 
	LUA_PARAM_STRING = 0x8, 
	LUA_PARAM_BOOL = 0x10, 
	LUA_PARAM_TABLE = 0x20,
};

// =====================================================================================
// Lua Parameter
// =====================================================================================

/**
 * LuaParam is the base type of parameters used in MMF events to send or recieve data
 * to Lua.  Values passed to MMF functions as parameters, or returned from Lua function
 * calls, are stored on the call stack.
 */

class LuaParam {
public:

	LuaParam ();
	LuaParam (const LuaParam& lp);

	virtual ~LuaParam ();

	// `Virtual constructors`
	virtual LuaParam*	Create () const;
	virtual LuaParam*	Clone () const;

	// Value Conversion
	virtual int			GetBoolean () const;
	virtual double		GetNumber () const;
	virtual std::string	GetString () const;

	virtual int			Type () const;
	virtual void		PushParam (lua_State* L) const;
};

/**
 * LuaParamNumber holds integer and floating point data.
 */

class LuaParamNumber : public LuaParam {
protected:

	double _value;

public:

	LuaParamNumber ();
	LuaParamNumber (double v);
	LuaParamNumber (const LuaParam& lp);

	~LuaParamNumber () { };

	// `Virtual constructors`
	LuaParamNumber*	Create () const;
	LuaParamNumber*	Clone () const;

	// Value conversion
	int				GetBoolean () const;
	double			GetNumber () const;
	std::string		GetString () const;

	// Push value onto Lua stack
	inline	void	PushParam (lua_State* L) const;

	inline	int		Type () const;
};

/**
 * LuaParamString holds string data
 */

class LuaParamString : public LuaParam {
protected:

	std::string _value;

public:

	LuaParamString ();
	LuaParamString (const char* str);
	LuaParamString (const LuaParam& lp);

	~LuaParamString () { };

	// `Virtual constructors`
	LuaParamString*	Create () const;
	LuaParamString*	Clone () const;

	// Value conversion
	int				GetBoolean () const;
	double			GetNumber () const;
	std::string		GetString () const;

	// Push value onto Lua stack
	inline	void	PushParam (lua_State* L) const;

	inline	int		Type () const;
};

/**
 * LuaParamBool holds a boolean value, mainly this is to differentiate types
 * and give Lua true booleans when requested.
 */

class LuaParamBool : public LuaParam {
protected:

	bool _value;

public:

	LuaParamBool ();
	LuaParamBool (bool b);
	LuaParamBool (const LuaParam& lp);

	~LuaParamBool () { }

	// `Virtual constructors`
	LuaParamBool*	Create () const;
	LuaParamBool*	Clone () const;

	// Value conversion
	int				GetBoolean () const;
	double			GetNumber () const;
	std::string		GetString () const;

	// Push value onto Lua stack
	inline	void	PushParam (lua_State* L) const;

	int				Type () const;
};

/**
 * LuaParamTable is a compound paramter type, holding a collection of
 * other parameters, including nested tables.
 */

class LuaParamTable : public LuaParam {
protected:

	std::list<LuaParam*>	_table;
	LuaParamTable*			_parent;

	typedef std::list<LuaParam*>::iterator IParams;
	typedef std::list<LuaParam*>::const_iterator CIParams;

public:

	LuaParamTable ();
	LuaParamTable (LuaParamTable* p);
	LuaParamTable (const LuaParam& lp);
	LuaParamTable (lua_State* L, int i);

	~LuaParamTable ();

	// `Virtual constructors`
	LuaParamTable*	Create () const;
	LuaParamTable*	Clone () const;

	// Insert takes ownership of *lp and will take care of deleting it
	void			Insert (LuaParam* lp);

	// Get parent table if nested
	LuaParamTable*	Parent () const;

	// Push value onto Lua stack
	inline	void	PushParam (lua_State* L) const;

	int				Type () const;
};

// =====================================================================================
// Parameter Set
// =====================================================================================

/**
 * ParamSet is a single-dimentional collection of parameters (although it can
 * contain tables and nested tables), used by event frames to hold parameters
 * and parameter buffers.
 *
 * Parameters can be pushed into ParamSets arbitrarily, and they keep track
 * internally whether a table is currently being written to and at what
 * nesting level.
 */

class ParamSet {
protected:

	std::vector<LuaParam*>	_params;
	LuaParamTable*			_tablePtr;

	typedef std::vector<LuaParam*>::iterator IParams;
	typedef std::vector<LuaParam*>::const_iterator CIParams;

public:

	ParamSet ();
	~ParamSet ();

	// Directly pushing values into set
	void			PushNil ();
	void			PushNumber (double v);
	void			PushBoolean (bool b);
	void			PushString (const char* str);
	void			PushTable (lua_State* L, int i);

	// Table control
	void			StartTable ();
	void			EndTable ();

	// Get info
	const LuaParam*	GetParam (unsigned index) const;
	unsigned		Count () const;

	// List maintenance
	void			Clear ();
	
	// Push copy of data into Lua stack
	void			PushParamsToLua (lua_State* L) const;
};


//---------------------------------------------------------------------------------------
//
// Inline Implementation Below
//

//---------------------------------------------------------------------------------------
// ParamSet

inline ParamSet::ParamSet ()
	: _params(), _tablePtr(0) { }

inline ParamSet::~ParamSet () {
	Clear();
}

inline unsigned ParamSet::Count () const {
	return _params.size();
}

//---------------------------------------------------------------------------------------
// LuaParam

inline LuaParam::LuaParam () { };

inline LuaParam::LuaParam (const LuaParam& lp) { };

inline LuaParam::~LuaParam () { };

inline LuaParam* LuaParam::Create () const { 
	return new LuaParam(); 
}

inline LuaParam* LuaParam::Clone () const { 
	return new LuaParam(*this); 
}

inline int	LuaParam::GetBoolean () const {
	return 0;
}

inline double LuaParam::GetNumber () const {
	return 0;
}

inline std::string LuaParam::GetString () const {
	return "";
}

inline void LuaParam::PushParam (lua_State* L) const {
	lua_pushnil(L);
}

inline int LuaParam::Type () const {
	return LUA_PARAM_NIL;
}

//---------------------------------------------------------------------------------------
// LuaParamNumber

inline LuaParamNumber::LuaParamNumber () 
	: _value(0) { };

inline LuaParamNumber::LuaParamNumber (double v) 
	: _value(v) { };

inline LuaParamNumber* LuaParamNumber::Create () const { 
	return new LuaParamNumber(); 
}

inline LuaParamNumber* LuaParamNumber::Clone () const { 
	return new LuaParamNumber(*this); 
}

inline void LuaParamNumber::PushParam (lua_State* L) const {
	lua_pushnumber(L, _value);
}

inline int LuaParamNumber::Type () const {
	return LUA_PARAM_NUM;
}

//---------------------------------------------------------------------------------------
// LuaParamString

inline LuaParamString::LuaParamString () 
	: _value("") { };

inline LuaParamString::LuaParamString (const char* str) 
	: _value(str) { };

inline LuaParamString* LuaParamString::Create () const { 
	return new LuaParamString(); 
}

inline LuaParamString* LuaParamString::Clone () const { 
	return new LuaParamString(*this); 
}

inline void LuaParamString::PushParam (lua_State* L) const {
	lua_pushstring(L, _value.c_str());
}

inline int LuaParamString::Type () const {
	return LUA_PARAM_STRING;
}

//---------------------------------------------------------------------------------------
// LuaParamBool

inline LuaParamBool::LuaParamBool () 
	: _value(false) { };

inline LuaParamBool::LuaParamBool (bool b) 
	: _value(b) { };

inline LuaParamBool* LuaParamBool::Create () const { 
	return new LuaParamBool(); 
}

inline LuaParamBool* LuaParamBool::Clone () const { 
	return new LuaParamBool(*this); 
}

inline void LuaParamBool::PushParam (lua_State* L) const {
	lua_pushboolean(L, _value);
}

inline int LuaParamBool::Type () const {
	return LUA_PARAM_BOOL;
}

//---------------------------------------------------------------------------------------
// LuaParamTable

inline LuaParamTable::LuaParamTable () 
	: _table(), _parent(0) { };

inline LuaParamTable::LuaParamTable (LuaParamTable* p) 
	: _table(), _parent(p) { };

inline LuaParamTable* LuaParamTable::Create () const { 
	return new LuaParamTable(); 
}

inline LuaParamTable* LuaParamTable::Clone () const { 
	return new LuaParamTable(*this); 
}

inline int LuaParamTable::Type () const {
	return LUA_PARAM_TABLE;
}

#endif
