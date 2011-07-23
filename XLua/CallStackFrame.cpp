#include <string>

#include "Common.h"

//---------------------------------------------------------------------------------------
// CallStackFrame

std::string CallStackFrame::_emptyString = "";

//---------------------------------------------------------------------------------------
// EventFrame

EventFrame::EventFrame ()
	: CallStackFrame(), _params(0), _returns(0), _paramBuffer(0), _returnBuffer(0) {
	_params = new ParamSet();
	_returns = new ParamSet();
	_paramBuffer = new ParamSet();
	_returnBuffer = new ParamSet();
}

EventFrame::~EventFrame () {
	delete _params;
	delete _returns;
	delete _paramBuffer;
	delete _returnBuffer;
}

/**
 * Transfer the entire param buffer to a new Event Frame, then create a new
 * blank one locally.  The old param buffer on the target frame will be destroyed.
 */

void EventFrame::MoveParamBufferToParams (EventFrame* f) {
	delete f->_params;
	f->_params = _params;
	_params = new ParamSet();
}

/**
 * Transfer the entire return buffer to a new Event Frame, then create a new
 * blank one locally.  The old return buffer on the target frame will be destroyed.
 */

void EventFrame::MoveReturnBufferToReturns (EventFrame* f) {
	delete f->_returns;
	f->_returns = _returns;
	_returns = new ParamSet();
}

//---------------------------------------------------------------------------------------
// LuaFrame
