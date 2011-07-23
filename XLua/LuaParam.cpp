#include <sstream>

#include "LuaParam.h"

//---------------------------------------------------------------------------------------
// LuaParam

//---------------------------------------------------------------------------------------
// LuaParamNumber

LuaParamNumber::LuaParamNumber (const LuaParam& lp) : _value(0) {
	if (lp.Type() == LUA_PARAM_NUM)
		_value = dynamic_cast<const LuaParamNumber&>(lp)._value;
}

int	LuaParamNumber::GetBoolean () const {
	return (_value != 0) ? 1 : 0;
}

double LuaParamNumber::GetNumber () const {
	return _value;
}

std::string	LuaParamNumber::GetString () const {
	std::stringstream str; str << _value;
	return str.str();
}

//---------------------------------------------------------------------------------------
// LuaParamBool

LuaParamBool::LuaParamBool (const LuaParam& lp) : _value(false) {
	if (lp.Type() == LUA_PARAM_BOOL)
		_value = dynamic_cast<const LuaParamBool&>(lp)._value;
}

int	LuaParamBool::GetBoolean () const {
	return _value;
}

double LuaParamBool::GetNumber () const {
	return _value;
}

std::string	LuaParamBool::GetString () const {
	std::stringstream str; str << _value;
	return str.str();
}

//---------------------------------------------------------------------------------------
// LuaParamString

LuaParamString::LuaParamString (const LuaParam& lp) : _value("") {
	if (lp.Type() == LUA_PARAM_STRING)
		_value = dynamic_cast<const LuaParamString&>(lp)._value;
}

int	LuaParamString::GetBoolean () const {
	return (_value.size() > 0) ? 1 : 0;
}

double LuaParamString::GetNumber () const {
	std::stringstream str; str << _value;
	double db; str >> db;
	return db;
}

std::string	LuaParamString::GetString () const {
	return _value;
}

//---------------------------------------------------------------------------------------
// LuaParamTable

// FIXME - ??
LuaParamTable::LuaParamTable (const LuaParam& lp) : _table() {
	if (lp.Type() == LUA_PARAM_TABLE) {
		const LuaParamTable& lpt = dynamic_cast<const LuaParamTable&>(lp);

		for (CIParams ip = lpt._table.begin(); ip != lpt._table.end(); ip++) {
			LuaParam* lpp = (*ip)->Clone();
			if (lpp->Type() == LUA_PARAM_TABLE)
				dynamic_cast<LuaParamTable*>(lpp)->_parent = this;

			_table.push_back(lpp);
		}
	}
}

LuaParamTable::~LuaParamTable () {
	for (IParams ip = _table.begin(); ip != _table.end(); ip++)
		delete *ip;
}

LuaParamTable* LuaParamTable::Parent () const {
	return _parent;
}

void LuaParamTable::Insert (LuaParam* lp) {
	_table.push_back(lp);
}

/**
 * Creates a new Lua table on the Lua stack, then recursively pushes all contained
 * values into the table.
 */

void LuaParamTable::PushParam (lua_State* L) const {
	lua_checkstack(L, 2);
	lua_createtable(L, _table.size(), 0);

	int i = 1;
	for (CIParams it = _table.begin(); it != _table.end(); it++) {
		(*it)->PushParam(L);
		lua_rawseti(L, -2, i++);
	}
}

//---------------------------------------------------------------------------------------
// ParamSet

/**
 * Pushes the nil value into the set, or current table if one is open.
 */

void ParamSet::PushNil () {
	LuaParam* lp = new LuaParam();

	if (!_tablePtr)
		_params.push_back(lp);
	else
		_tablePtr->Insert(lp);
}

/**
 * Pushes a number into the set, or current table if one is open.
 */

void ParamSet::PushNumber (double v) {
	LuaParam* lp = new LuaParamNumber(v);

	if (!_tablePtr)
		_params.push_back(lp);
	else
		_tablePtr->Insert(lp);
}

/**
 * Pushes a boolean into the set, or current table if one is open.
 */

void ParamSet::PushBoolean (bool b) {
	LuaParam* lp = new LuaParamBool(b);

	if (!_tablePtr)
		_params.push_back(lp);
	else
		_tablePtr->Insert(lp);
}

/**
 * Pushes a table into the set, or current table if one is open.
 * XXX: Currently unsupported, may add in the future
 */

void ParamSet::PushTable (lua_State* L, int i) {
	/*LuaParam* lp = new LuaParamTable(L, i);

	if (!_tablePtr)
		_params.push_back(lp);
	else
		_tablePtr->Insert(lp);*/
}

/**
 * Pushes a string into the set, or current table if one is open.
 */

void ParamSet::PushString (const char* str) {
	LuaParam* lp = new LuaParamString(str);

	if (!_tablePtr)
		_params.push_back(lp);
	else
		_tablePtr->Insert(lp);
}

/**
 * Opens a new table parameter.  Any new values pushed into the set will instead
 * be pushed into the table.  If this function is called again before closing the
 * table, then a nested table will be created.
 */

void ParamSet::StartTable () {
	LuaParamTable* lp = new LuaParamTable(_tablePtr);

	if (!_tablePtr)
		_params.push_back(lp);
	else
		_tablePtr->Insert(lp);

	_tablePtr = lp;
}

/**
 * Closes the currently open table.  No new values will be pushed into the table.
 */

void ParamSet::EndTable () {
	if (!_tablePtr)
		return;

	_tablePtr = _tablePtr->Parent();
}

/**
 * Returns a generic parameter at the requested index.  Returns a null pointer
 * if the requested index is out of range.
 */

const LuaParam* ParamSet::GetParam (unsigned index) const {
	if (index >= _params.size())
		return 0;

	return _params[index];
}

/**
 * Destroy all values currently in the param set.
 */

void ParamSet::Clear () {
	for (IParams ip = _params.begin(); ip != _params.end(); ip++)
		delete *ip;
	_params.clear();

	_tablePtr = 0;
}

/**
 * Push every value in the param set onto the Lua stack.  The contents of the ParamSet
 * itself will remain unchanged.
 */

void ParamSet::PushParamsToLua (lua_State* L) const {
	for (CIParams ip = _params.begin(); ip != _params.end(); ip++)
		(*ip)->PushParam(L);
}
