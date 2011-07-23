#include "Common.h"

bool XLuaGlobal::CreateState (int sid) {
	DeleteState(sid);

	XLuaState* state = new XLuaState();
	_stateTable[sid] = state;

	if (!state->ready) {
		return false;
	}
	state->stateId = sid;

	_idLookup[state] = sid;
	_stateLookup[state->state] = state;

	return true;
}

bool XLuaGlobal::DeleteState (int sid) {
	IStateMap slot = _stateTable.find(sid);
	if (slot != _stateTable.end() && slot->second != NULL) {
		if (slot->second->stack.Level() > 0) {
			slot->second->queueDelete = true;
			return false;
		}

		_idLookup[slot->second] = NULL;
		_stateLookup[slot->second->state] = NULL;
		delete slot->second;

		_stateTable[sid] = NULL;
		return true;
	}

	return false;
}

bool XLuaGlobal::DeleteState (XLuaState* s) {
	IIdMap slot = _idLookup.find(s);
	if (slot == _idLookup.end())
		return false;

	return DeleteState(slot->second);
}
