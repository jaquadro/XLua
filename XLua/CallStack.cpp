#include <sstream>

#include "Common.h"

//---------------------------------------------------------------------------------------
// CallStack

/**
 * Returns the event number where MMF execution last left off.
 */

int CallStack::EventNumber () const {
	return _state->rdList.front()->rdPtr->rHo.hoAdRunHeader->rhEventGroup->evgIdentifier;
}

/**
 * Gets the bottom (base) frame of the callstack.  It's linear time but we
 * only want it for backtraces
 */

CallStackFrame* CallStack::BaseFrame () const {
	CallStackFrame* f = TopFrame();
	while (f && f->Prev()) {
		f = f->Prev();
	}

	return f;
}

/**
 * Constructs a new event frame, and pushes it onto the stack.  Also updates the
 * stored top event frame reference.
 */

EventFrame* CallStack::PushEventFrame () {
	EventFrame* f = new EventFrame();

	if (_stack) {
		f->BindPrev(_stack);
		_stack->BindNext(f);
	}

	_stack = f;
	_topEventFrame = f;
	_size++;

	return f;
}

/**
 * Constructs a new Lua Frame and pushes it onto the stack.
 */

LuaFrame* CallStack::PushLuaFrame () {
	LuaFrame* f = new LuaFrame();

	if (_stack) {
		f->BindPrev(_stack);
		_stack->BindNext(f);
	}

	_stack = f;
	_size++;

	return f;
}

/**
 * Pop off the top stack frame and destroy it.  Recalculate where the new top event
 * frame actually is.
 */

void CallStack::PopFrame () {
	if (_stack) {
		CallStackFrame* f = _stack;
		_stack = _stack->Prev();
		_stack->BindNext(0);
		delete f;

		CallStackFrame* t = _stack;
		while (t && t->Type() != EVENT_FRAME_TYPE) {
			t = t->Prev();
		}
		_topEventFrame = dynamic_cast<EventFrame*>(t);
	}
}

/**
 * Destroy all frames on the stack
 * Beware, this will violate our assumption that there is always a base event frame.
 */

void CallStack::ClearStack () {
	while (_stack) {
		CallStackFrame* f = _stack;
		_stack = _stack->Prev();
		delete f;
	}
	_size = 0;
}

/**
 * CallLuaFunction performs an M->L function call.
 * The event frame is assumed to already exist on the top of the stack, as
 * MMF is the current context.  A Lua frame will be created before invoking
 * the Lua function, and torn down after.
 *
 * The event frame's param buffer will be push onto the Lua stack and flushed.
 * After execution, any values returned from the Lua function will be read
 * into the event frame's returns list, and will persist until either the
 * next M->L call, or the next M->M call.
 */

int CallStack::CallLuaFunction (const char* func) {
	_level++;

	// Require safe FP environment
	_fpcontrol = fp_precision_double();

	// Clear stack at beginning of function call
	lua_settop(_state->state, 0);

	// Get M frame
	EventFrame* mFrame = TopEventFrame();

	// Check stack size, abort if insufficient space
	int paramCount = mFrame->ParamBuffer()->Count();
	int paramAdjust = 0;
	if (!lua_checkstack(_state->state, paramCount + 2)) {
		_state->RaiseError("Stack error: cannot grow stack");
		return 0;
	}

	// For a nested table call, seek the parent table and place it on the stack
	std::string element = _state->SeekField(func).cstr;
	int stackAdjust = lua_gettop(_state->state);

	// If a : operator is present, we must push the extra table parameter
	int opIndex = element.find_last_of(":");
	if (opIndex != std::string::npos) {
		std::string parentTable = element.substr(0, opIndex);
		std::string fieldFunc = element.substr(opIndex + 1, (element.size() - opIndex - 1));

		// Push Table to stack
		lua_pushstring(_state->state, parentTable.c_str());		// ] parentTable 
		lua_gettable(_state->state, -2);
		if (lua_isnil(_state->state, -1)) {
			std::stringstream str; str << "Event " << EventNumber() << ": Attempt to call a nil value";
			_state->RaiseError(str.str());
			return 0;
		}
		stackAdjust++;

		// Push function onto stack
		lua_pushstring(_state->state, fieldFunc.c_str());		// ] parentTable , fieldFunc
		lua_gettable(_state->state, -2);

		// Push hidden argument onto stack
		lua_pushstring(_state->state, parentTable.c_str());		// ] parentTable , fieldFunc , parentTable
		lua_gettable(_state->state, -4);
		paramAdjust++;
	}

	// If : operator is not present, just push the function
	else {
		lua_pushstring(_state->state, element.c_str());
		lua_gettable(_state->state, -2);
	}

	// Create L frame
	LuaFrame* lFrame = PushLuaFrame();
	lFrame->SetUnitName(func);

	// Push all parameters onto stack
	mFrame->ParamBuffer()->PushParamsToLua(_state->state);
	mFrame->ParamBuffer()->Clear();

	// Setup error handler
	int base = lua_gettop(_state->state) - (paramCount + paramAdjust);
	lua_pushcfunction(_state->state, CallStack::LuaC_StackError);
	lua_insert(_state->state, base);

	// Do protected call
	if (lua_pcall(_state->state, paramCount + paramAdjust, LUA_MULTRET, base) != 0) {
		lua_settop(_state->state, 0);
	}
	else {
		lua_remove(_state->state, base);
	}

	// No longer need L frame since Lua buffers on its own stack
	PopFrame();

	// Clean out returns list
	mFrame->Returns()->Clear();

	// Get count of returned items, accounting for overhead from initial function call
	int returnCount = lua_gettop(_state->state) - stackAdjust;

	// Capture all returned items
	for (int i = 1; i <= returnCount; i++) {
		int stackIndex = stackAdjust + i;
		switch (lua_type(_state->state, stackIndex)) {
			case LUA_TNUMBER:	mFrame->Returns()->PushNumber(lua_tonumber(_state->state, stackIndex)); break;
			case LUA_TSTRING:	mFrame->Returns()->PushString(lua_tostring(_state->state, stackIndex)); break;
			case LUA_TBOOLEAN:	mFrame->Returns()->PushBoolean(lua_toboolean(_state->state, stackIndex) != 0); break;
			//case LUA_TTABLE:	mFrame->Returns()->PushTable(_state->state, stackIndex); break;

			// In case of invalid return type, report error and push an empty value
			default:
				std::stringstream str;
				str << "Event " << EventNumber() << ": Invalid return type at index " << i << " in function '" << func << "'";
				_state->RaiseError(str.str());
				mFrame->Returns()->PushNil();
				break;
		}
	}

	// The return values will remain with this stack frame until either the frame is destroyed, the user explicitly
	// clears the return values, or another function call is made from this entry.

	// Restore FP environment
	fp_precision_restore(_fpcontrol);

	_level--;

	// If we're waiting to die and we finished unwinding
	if (_level == 0 && _state->queueDelete) {
		XLuaGlobal::Get().DeleteState(_state);
	}

	return returnCount;
}

/**
 * CallMMFFunction performs an L->M function call, that is it invokes one (or more)
 * events that have the XLua "On Function" condition set for the function name.
 * A Lua frame is assumed to be on the top of the stack, but is not directly used.
 * A new event frame is created before invoking the MMF event(s), and torn down
 * after reading back any return values.
 *
 * Once the new event frame is created, any parameters are read off the Lua stack
 * into the frame's params list, and will persist until the frame is destroyed.
 * After the events return, any values in the frame's return buffer will be pushed
 * onto the Lua stack.
 *
 * If multiple XLua objects have been bound to the current state, then CallMMFFunction
 * will attempt to invoke events for each of the bound XLua objects.
 */

int CallStack::CallMMFFunction (const char* func, int funcGroup, lua_State* state) {
	_level++;

	if (!state)
		state = _state->state;

	// Get number of parameters
	int paramCount = lua_gettop(state);

	// Create new event frame
	EventFrame* mFrame = PushEventFrame();
	mFrame->SetUnitName(func);

	//luaL_where(state, 1);
	//_paramRPtr->calledBy = lua_tostring(state, -1);
	//lua_pop(state, 1);

	// Capture all parameters
	for (int i = 1; i <= paramCount; i++) {
		switch (lua_type(state, i)) {
			case LUA_TNUMBER:	mFrame->Params()->PushNumber(lua_tonumber(state, i)); break;
			case LUA_TSTRING:	mFrame->Params()->PushString(lua_tostring(state, i)); break;
			case LUA_TBOOLEAN:	mFrame->Params()->PushBoolean(lua_toboolean(state, i) != 0); break;
			//case LUA_TTABLE:	mFrame->Params()->PushTable(state, i); break;

			// In case of invalid parameter type, report error and push an empty value
			default:
				std::stringstream str;
				str << "Invalid parameter type at index " << i << " in function '" << mFrame->GetUnitName() << "'";
				_state->RaiseError(str.str());
				mFrame->Params()->PushNil();
				break;
		}
	}

	// Going back to MMF, restore fp state
	fp_precision_restore(_fpcontrol);

	// Call MMF event(s) corresponding to this function call
	for (XLuaState::IRdList i_rd = _state->rdList.begin(); i_rd != _state->rdList.end(); i_rd++) {
		if (*i_rd != NULL) {
			callRunTimeFunction((*i_rd)->rdPtr, RFUNCTION_GENERATEEVENT, funcGroup, 0);
			//(*i_rd)->rdPtr->rRd->GenerateEvent(funcGroup);
		}
	}

	// Get safe FP environment again
	fp_precision_double();

	// Handle a disconnect request if we received it in the above calls
	_state->ReleaseRd();

	// Push collected return values onto the stack
	int returnCount = mFrame->ReturnBuffer()->Count();
	mFrame->ReturnBuffer()->PushParamsToLua(state);

	// Don't need event frame anymore, going back to Lua
	PopFrame();

	_level--;
	return returnCount;
}

/**
 * CallMMFFunctionFromMMF performs an M->M function call, that is one event directly
 * invokes another event without switching control to Lua.  When handling parameters
 * and return values, the calling context assumes it is invoking a Lua function, and
 * the callee context assumes it is returning to a Lua function.  A new event frame
 * will be temporarily created for the callee context, and destroyed after the events
 * have returned and any return values transferred.
 *
 * If multiple XLua objects have been bound to the current state, then
 * CallMMFFunctionFromMMF will attempt to invoke events for each of the bound XLua objects.
 */

int CallStack::CallMMFFunctionFromMMF (const char* func) {
	_level++;

	// Require safe FP environment
	_fpcontrol = fp_precision_double();

	// Clear stack at beginning of function call
	lua_settop(_state->state, 0);

	// Get current event frame
	EventFrame* mFrame = TopEventFrame();

	// Check stack size, abort if insufficient space
	int paramCount = mFrame->ParamBuffer()->Count();

	// Create new event frame
	EventFrame* mFrame2 = PushEventFrame();
	mFrame2->SetUnitName(func);

	// Transfer param buffer of current frame to param list of new frame
	mFrame->MoveParamBufferToParams(mFrame2);

	// Going back to MMF, restore fp state
	fp_precision_restore(_fpcontrol);

	// Call MMF event(s) corresponding to this function call
	for (XLuaState::IRdList i_rd = _state->rdList.begin(); i_rd != _state->rdList.end(); i_rd++) {
		if (*i_rd != NULL) {
			callRunTimeFunction((*i_rd)->rdPtr, RFUNCTION_GENERATEEVENT, 1, 0);
			//(*i_rd)->rdPtr->rRd->GenerateEvent(1);
		}
	}

	// Get safe FP environment again
	fp_precision_double();

	// Get return count
	int returnCount = mFrame2->ReturnBuffer()->Count();

	// Transfer return buffer of new frame to return list of current frame
	mFrame2->MoveReturnBufferToReturns(mFrame);

	// Pop "new" frame off stack since it's out of scope
	PopFrame();

	// Restore FP environment
	fp_precision_restore(_fpcontrol);

	_level--;
	return returnCount;
}

/**
 * GetFunction returns the function name of the event or lua frame, if a frame exists
 * (which it should).
 */

const std::string& CallStack::GetFunction () const {
	if (!TopEventFrame()) {
		std::stringstream str; str << "Event " << EventNumber() << ": Invalid place to get MMF function name";
		_state->RaiseError(str.str());
		return _emptyString;
	}

	return TopEventFrame()->GetUnitName();
}

/**
 * GetParam returns a generic parameter from the current event frame's params list
 * at the requested index, if the index is valid.  The empty parameter is returned
 * in case of error.
 */

const LuaParam& CallStack::GetParam (unsigned index) const {
	EventFrame* f = TopEventFrame();
	if (!f) {
		std::stringstream str; str << "Event " << EventNumber() << ": Invalid place to get MMF parameter";
		_state->RaiseError(str.str());
		return _emptyParam;
	}

	if (f->Params()->Count() <= (index - 1)) {
		std::stringstream str; str << "Event " << EventNumber() << ": Invalid parameter index requested";
		_state->RaiseError(str.str());
		return _emptyParam;
	}

	return *f->Params()->GetParam(index - 1);
}

/*LuaParam* CallStack::GetLastTableParam () {
	if (_stack.size() < 1)
		return 0;

	if (_paramNest <= 0)
		return 0;

	int sz = _stack.back().mf_params.size();
	if (sz < 1)
		return 0;

	if (_stack.back().mf_params[sz - 1].GetType() != LUA_PARAM_TABLE)
		return 0;

	return &_stack.back().mf_params[sz - 1];
}*/

/**
 * GetReturn returns a generic parameter from the current event frame's returns list
 * at the requested index, if the index is valid.  The empty parameter is returned
 * in case of error.
 */

const LuaParam& CallStack::GetReturn (unsigned index) const {
	EventFrame* f = TopEventFrame();
	if (f->Returns()->Count() <= (index - 1)) {
		int event = EventNumber();
		std::stringstream str; str << "Event " << EventNumber() << ": Invalid return index requested";
		_state->RaiseError(str.str());
		return _emptyParam;
	}

	return *f->Returns()->GetParam(index - 1);
}

/**
 * The following set of functions push values onto the current event frame's return buffer.
 * It is considered an error to try and push a return value from the base event frame.
 */

void CallStack::PushReturn (int n) {
	if (CheckPushReturn())
		TopEventFrame()->ReturnBuffer()->PushNumber(n);
}

void CallStack::PushReturn (double v) {
	if (CheckPushReturn())
		TopEventFrame()->ReturnBuffer()->PushNumber(v);
}

void CallStack::PushReturn (const char* s) {
	if (CheckPushReturn())
		TopEventFrame()->ReturnBuffer()->PushString(s);
}

void CallStack::PushReturn (bool b) {
	if (CheckPushReturn())
		TopEventFrame()->ReturnBuffer()->PushBoolean(b);
}

void CallStack::PushReturn () {
	if (CheckPushReturn())
		TopEventFrame()->ReturnBuffer()->PushNil();
}

/**
 * The following set of functions control table creation.  Starting a table causes
 * any further pushed values to be a member of a table.  A nested call causes a
 * nested table to be started.  Ending a table closes the table and further pushed
 * values will be placed outside the table.
 */

void CallStack::StartReturnTable () {
	if (CheckPushReturn())
		TopEventFrame()->ReturnBuffer()->StartTable();
}

void CallStack::EndReturnTable () {
	if (CheckPushReturn())
		TopEventFrame()->ReturnBuffer()->EndTable();
}

/**
 * CheckPushReturn does a quick check that it's safe to push a return value (that we're
 * not pushing a value off of the base event frame.
 */

bool CallStack::CheckPushReturn () {
	if (FrameCount() <= 1) {
		std::stringstream str; str << "Event " << EventNumber() << ": Invalid place to push MMF return value";
		_state->RaiseError(str.str());
		return false;
	}

	return true;
}

extern "C" int db_errorfb (lua_State *L);

/**
 * LuaC_StackError is a special error handler used before invoking a Lua function in
 * the M->L function call routine.
 *
 * If the option is specified, this function will attempt to construct a full backtrace
 * to include with the reported error, to aid end-user debugging.
 */

int CallStack::LuaC_StackError (lua_State *L) {
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
					
					std::stringstream bt_build;
					bt_build << "Event " << state->stack.EventNumber() << ": " << lua_tostring(L, 1) << "\r\n";

					// Bad formatting alert - built from excrement and dark magic
					bt = bt_build.str();
					CallStackFrame* frame = state->stack.BaseFrame();

					while (frame) {
						int index1 = bt.find("<[string");
						int index2 = bt.find(">", index1);

						if (index1 != std::string::npos && index2 != std::string::npos) {
							bt = bt.substr(0, index1) + "'" + frame->GetUnitName() + "'" + 
								 bt.substr(index1 + (index2 - index1) + 1);
						}

						frame = frame->Next();
					}
				}

				lm->RaiseError(bt);
			}
			else {
				lm->RaiseError(message + "\r\n");
			}
		}
	}

	return 0;
}
