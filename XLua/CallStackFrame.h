#ifndef CALL_STACK_FRAME_H_
#define CALL_STACK_FRAME_H_

#include <string>
#include <vector>

#include "LuaParam.h"

// =====================================================================================
// Call Stack Frames
// =====================================================================================

enum FrameType {
	BASE_FRAME_TYPE, 
	EVENT_FRAME_TYPE, 
	LUA_FRAME_TYPE
};

/**
 * CallStackFrame is the base type for representing a call stack position, when control
 * passes between Lua and MMF and vis-versa.  Subtypes of CallStackFrame may have additional
 * data associated, like parameters.
 */

class CallStackFrame {
protected:
	
	CallStackFrame*	_prevFrame;
	CallStackFrame*	_nextFrame;

	std::string		_unitName;
	
	typedef std::vector<LuaParam*>::iterator IParams;
	typedef std::vector<LuaParam*>::const_iterator CIParams;

	// Dummy string
	static std::string	_emptyString;

public:

	CallStackFrame ();
	virtual ~CallStackFrame ();

	// Frame Linking
	CallStackFrame*		Next () const;
	CallStackFrame*		Prev () const;
	void				BindNext (CallStackFrame* f);
	void				BindPrev (CallStackFrame* f);

	// Function Name / Caller Name
	void				SetUnitName (const char* str);
	const std::string&	GetUnitName () const;
	const std::string&	CalledBy () const;

	virtual	FrameType	Type () const;
};

/**
 * EventFrame is the "main" frame type, and represents a context where MMF
 * currently has control and is executing events.  It contains the following
 * additional state:
 *
 *	 Params: List of parameters passed to the context from a calling parent,
 *		either a chunk of Lua code, or another Event via local MMF function
 *		invocation.  Alias: MF_Params.
 *	 Returns: List of return values passed back from the last call the context
 *		made to either a Lua function, or another Event via local MMF function
 *		invocation.  Previous entries are flushed.  Alias: MF_Returns.
 *	 Param Buffer: Storage set aside for pushing parameters that will be used
 *		in the next function call.  This is flushed when the call is made.
 *		Alias: LF_Params.
 *	 Return Buffer: Storage set aside for pushing return values to be sent
 *		back to the calling parent.  Alias: LF_Returns.
 */

class EventFrame : public CallStackFrame {
protected:
	
	ParamSet*	_params;
	ParamSet*	_returns;
	ParamSet*	_paramBuffer;
	ParamSet*	_returnBuffer;

public:
	
	EventFrame ();
	~EventFrame ();

	// List / Buffer Access
	ParamSet*	Params () const;
	ParamSet*	Returns () const;
	ParamSet*	ParamBuffer () const;
	ParamSet*	ReturnBuffer () const;

	// For directly transfering buffers in M->M calls
	void		MoveParamBufferToParams (EventFrame* f);
	void		MoveReturnBufferToReturns (EventFrame* f);

	FrameType	Type () const;
};

/**
 * LuaFrame represents a context where Lua code is being executed.  It can be
 * created either by calling an existing Lua function, or by executing a new
 * chunk of Lua code.  Because Lua has its own stack for storing values, it
 * does not contain any extra parameter lists or buffers.
 */

class LuaFrame : public CallStackFrame {
public:

	LuaFrame ();
	~LuaFrame ();

	FrameType	Type () const;
};



//---------------------------------------------------------------------------------------
//
// Inline Implementation Below
//

//---------------------------------------------------------------------------------------
// CallStackFrame

inline CallStackFrame::CallStackFrame ()
	: _prevFrame(0), _nextFrame(0), _unitName("") { }

inline CallStackFrame::~CallStackFrame () { }

inline FrameType CallStackFrame::Type () const { 
	return BASE_FRAME_TYPE; 
}

inline void CallStackFrame::BindNext (CallStackFrame* f) {
	_nextFrame = f;
}

inline void CallStackFrame::BindPrev (CallStackFrame* f) {
	_prevFrame = f;
}

inline CallStackFrame* CallStackFrame::Next () const {
	return _nextFrame;
}

inline CallStackFrame* CallStackFrame::Prev () const {
	return _prevFrame;
}

inline void CallStackFrame::SetUnitName (const char* str) {
	_unitName = str;
}

inline const std::string& CallStackFrame::GetUnitName () const {
	return _unitName;
}

inline const std::string& CallStackFrame::CalledBy () const {
	return _emptyString;
}

//---------------------------------------------------------------------------------------
// EventFrame

inline FrameType EventFrame::Type () const { 
	return EVENT_FRAME_TYPE; 
}

inline ParamSet* EventFrame::Params () const {
	return _params;
}

inline ParamSet* EventFrame::Returns () const {
	return _returns;
}

inline ParamSet* EventFrame::ParamBuffer () const {
	return _paramBuffer;
}

inline ParamSet* EventFrame::ReturnBuffer () const {
	return _returnBuffer;
}

//---------------------------------------------------------------------------------------
// LuaFrame

inline LuaFrame::LuaFrame ()
	: CallStackFrame() { }

inline LuaFrame::~LuaFrame () { }

inline FrameType LuaFrame::Type () const {
	return LUA_FRAME_TYPE;
}

#endif
