#ifndef CALL_STACK_H_
#define CALL_STACK_H_

#include	<vector>
#include	<string>

#include	"LuaParam.h"
#include	"CallStackFrame.h"

#include	"Lua/lua.hpp"

enum {
	MF_FUNCTION_GROUP_DEF = 1,
	MF_FUNCTION_GROUP_A = 9,
	MF_FUNCTION_GROUP_B = 10,
	MF_FUNCTION_GROUP_C = 11,
	MF_FUNCTION_GROUP_D = 12,
	MF_FUNCTION_GROUP_E = 13,
	MF_FUNCTION_GROUP_F = 14,
	MF_FUNCTION_GROUP_G = 15
};

// =====================================================================================
// Lua Call Stack
// =====================================================================================

class XLuaState;

/**
 * CallStack represents the entire call stack, and serves the dual purpose of
 * both holding the stack frames, and exposing an interface to the main action
 * and expression routines for manipulating the call stack.
 *
 * The CallStack will _always_ contain an EventFrame as its base element, representing
 * the main thread of execution in the application.
 */

class CallStack {
protected:

	XLuaState*		_state;

	// Top stack frame
	CallStackFrame*	_stack;

	// Top event stack frame (may be the same as above)
	EventFrame*		_topEventFrame;

	// Current number of stack frames
	unsigned		_size;

	// Nesting level of calls (XXX: redundant now?)
	unsigned		_level;

	mutable bool	_exception;

	// Dummy values to return in cases of error
	LuaParam		_emptyParam;
	std::string		_emptyString;

	// Stores the floating-point mode of the application when forcing double-precision
	unsigned		_fpcontrol;

public:

	CallStack ();
	CallStack (XLuaState* L);

	~CallStack();

	// Initialization
	void			Init ();
	void			Reinit ();

	// Configuration
	void			SetState (XLuaState* L);

	// Error Handling
	bool			Exception () const;
	void			ClearException ();

	// Misc
	int				Level () const;
	int				EventNumber () const;

	// Accessing the Stack
	CallStackFrame*	TopFrame () const;
	EventFrame*		TopEventFrame () const;
	CallStackFrame*	BaseFrame () const;
	unsigned		FrameCount () const;

	// Manipulating the Stack
	EventFrame*		PushEventFrame ();
	LuaFrame*		PushLuaFrame ();
	void			PopFrame ();
	void			ClearStack ();

	// Function Call Interface
	int				CallLuaFunction (const char* func);
	int				CallMMFFunction (const char* func, int funcGroup = MF_FUNCTION_GROUP_DEF, lua_State* state = NULL);
	int				CallMMFFunctionFromMMF (const char* func);

	// Function Name
	const			std::string& GetFunction () const;

	// Clearing the param lists / buffers of event frames
	void			ClearMFParameters ();
	void			ClearMFReturns ();
	void			ClearLFParameters ();
	void			ClearLFReturns ();

	// Parameter getting functions, intended for getting MMF parameters
	bool			GetBooleanParam (unsigned index) const;
	double			GetNumberParam (unsigned index) const;
	std::string		GetStringParam (unsigned index) const;

	const LuaParam&	GetParam (unsigned index) const;
	int				GetParamCount () const;

	LuaParam*		GetLastTableParam ();

	// Return value getting functions, intended for getting Lua return values
	double			GetNumberReturn (unsigned index) const;
	std::string		GetStringReturn (unsigned index) const;
	bool			GetBooleanReturn (unsigned index) const;

	const LuaParam&	GetReturn (unsigned index) const;
	int				GetReturnCount () const;

	// Parameter pushing functions, intended for pushing Lua parameters
	void			PushParam (int n);
	void			PushParam (double v);
	void			PushParam (const char* s);
	void			PushParam (bool b);
	void			PushParam ();

	void			StartParamTable ();
	void			EndParamTable ();

	// Return value pushing functions, intended for pushing MMF return values
	void			PushReturn (int n);
	void			PushReturn (double v);
	void			PushReturn (const char* s);
	void			PushReturn (bool b);
	void			PushReturn ();

	void			StartReturnTable ();
	void			EndReturnTable ();

	bool			CheckPushReturn ();

	// Low-level Lua interface
	static int		LuaC_StackError (lua_State* L);
};



//---------------------------------------------------------------------------------------
//
// Inline Implementation Below
//

//---------------------------------------------------------------------------------------
// CallStack

inline CallStack::CallStack ()
	: _state(NULL), _stack(0), _topEventFrame(0), _size(0), _level(0) {
	Init();
}

inline CallStack::CallStack (XLuaState* L)
	: _state(L), _stack(0), _topEventFrame(0), _size(0), _level(0) {
	Init();
}

inline CallStack::~CallStack () {
	ClearStack();
}

inline void CallStack::Init () {
	PushEventFrame();
}

inline void CallStack::Reinit () {
	ClearStack();
	PushEventFrame();
}

inline void CallStack::SetState (XLuaState* L) {
	_state = L;
}

inline bool CallStack::Exception () const {
	return _exception;
}

inline void CallStack::ClearException () {
	_exception = false;
}

inline int CallStack::Level () const {
	return _level;
}

inline CallStackFrame* CallStack::TopFrame () const {
	return _stack;
}

inline EventFrame* CallStack::TopEventFrame () const {
	return _topEventFrame;
}

inline unsigned CallStack::FrameCount () const {
	return _size;
}

inline void CallStack::ClearMFParameters () {
	TopEventFrame()->Params()->Clear();
}

inline void CallStack::ClearMFReturns () {
	TopEventFrame()->Returns()->Clear();
}

inline void CallStack::ClearLFParameters () {
	TopEventFrame()->ParamBuffer()->Clear();
}

inline void CallStack::ClearLFReturns () {
	TopEventFrame()->ReturnBuffer()->Clear();
}

inline double CallStack::GetNumberParam (unsigned index) const {
	return GetParam(index).GetNumber();
}

inline std::string CallStack::GetStringParam (unsigned index) const {
	return GetParam(index).GetString();
}

inline bool CallStack::GetBooleanParam (unsigned index) const {
	return GetParam(index).GetBoolean() != 0;
}

inline int CallStack::GetParamCount () const {
	return TopEventFrame()->Params()->Count();
}

inline double CallStack::GetNumberReturn (unsigned index) const {
	return GetReturn(index).GetNumber();
}

inline std::string CallStack::GetStringReturn (unsigned index) const {
	return GetReturn(index).GetString();
}

inline bool CallStack::GetBooleanReturn (unsigned index) const {
	return GetReturn(index).GetBoolean() != 0;
}

inline int CallStack::GetReturnCount () const {
	return TopEventFrame()->Returns()->Count();
}

inline void CallStack::PushParam (int n) {
	TopEventFrame()->ParamBuffer()->PushNumber(n);
}

inline void CallStack::PushParam (double v) {
	TopEventFrame()->ParamBuffer()->PushNumber(v);
}

inline void CallStack::PushParam (const char* s) {
	TopEventFrame()->ParamBuffer()->PushString(s);
}

inline void CallStack::PushParam (bool b) {
	TopEventFrame()->ParamBuffer()->PushBoolean(b);
}

inline void CallStack::PushParam () {
	TopEventFrame()->ParamBuffer()->PushNil();
}

inline void CallStack::StartParamTable () {
	TopEventFrame()->ParamBuffer()->StartTable();
}

inline void CallStack::EndParamTable () {
	TopEventFrame()->ParamBuffer()->EndTable();
}

#endif