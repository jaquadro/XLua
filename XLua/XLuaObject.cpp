#include "Common.h"

// =====================================================================================
//
// Lua Manager (2)
// 
// =====================================================================================

XLuaObject::XLuaObject ()
	: state(NULL), errorQueue(), printQueue() {
}

XLuaObject::~XLuaObject () { 
	if (state) {
		state->UnbindRd(this);
		state = NULL;
	}
}

//-----------------------------------------------------------------------------

const std::string& XLuaObject::GetError () {
	if (!errorQueue.empty()) {
		errorString = errorQueue.front();
		errorQueue.pop();

		return errorString;
	}

	errorString.clear();
	return errorString;
}

const std::string& XLuaObject::GetPrint () {
	if (!printQueue.empty()) {
		printString = printQueue.front();
		printQueue.pop();

		return printString;
	}

	printString.clear();
	return printString;
}

void XLuaObject::RaiseError (const std::string& str) {
	if (errorQueue.size() >= 20) {
		errorQueue.pop();
	}

	errorQueue.push(str);
	lastErrorString = str;

	if (rdPtr == NULL) return;

	if (errMode == 0) {
		callRunTimeFunction(rdPtr, RFUNCTION_GENERATEEVENT, 0, 0);
		//rdPtr->rRd->GenerateEvent(0);
	}
	else if (errMode == 1) {
		callRunTimeFunction(rdPtr, RFUNCTION_PUSHEVENT, 0, 0);
		//rdPtr->rRd->PushEvent(0);
	}
}

void XLuaObject::RaisePrint (const std::string& str) {
	if (printQueue.size() >= 2000) {
		printQueue.pop();
	}
	printQueue.push(str);
	lastPrintString = str;

	if (rdPtr == NULL) return;

	if (printMode == 0) {
		callRunTimeFunction(rdPtr, RFUNCTION_GENERATEEVENT, 2, 0);
		//rdPtr->rRd->GenerateEvent(2);
	}
	else if (printMode == 1) {
		callRunTimeFunction(rdPtr, RFUNCTION_PUSHEVENT, 2, 0);
		//rdPtr->rRd->PushEvent(2);
	}
}

//-----------------------------------------------------------------------------

void XLuaObject::BindState (int sid) {
	//XLuaGlobal* glob = GetLG(rdPtr);
	
	if (state != NULL) {
		state->UnbindRd(this);
	}

	state = XLuaGlobal::Get().GetState(sid);
	if (state == NULL) {
		XLuaGlobal::Get().CreateState(sid);
		state = XLuaGlobal::Get().GetState(sid);
		stateId = sid;
	}

	if (state != NULL) {
		state->BindRd(this);
	}
}

void XLuaObject::UnbindState () {
	if (state != NULL) {
		state->UnbindRd(this);
		state = NULL;
	}
}

std::string XLuaObject::GetEmbeddedScript (const char* name) {
	IScripts si = scripts.find(name);
	if (si == scripts.end())
		return "";

	return si->second.script;
}

//-----------------------------------------------------------------------------

LPRO xlua_get_object (lua_State* L, int objid) {
#ifdef XLUA_LEGACY
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	
	XLuaState::IObjects iter = state->objects.find(objid);
	if (iter == state->objects.end()) {
		return NULL;
	}

	return iter->second.obj;
#else
	return 0;
#endif
}

LPRH xlua_get_run_header (lua_State* L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);

	return state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
}
