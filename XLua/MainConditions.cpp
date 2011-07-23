#include "Common.h"

// Quick memo: content of the eventInformations arrays
// ---------------------------------------------------
// Menu ID
// String ID
// Code
// Flags
// Number_of_parameters
// Parameter_type [Number_of_parameters]
// Parameter_TitleString [Number_of_parameters]

//	Menu ID								String ID						Code							Flags			Param Count, Parameters
short conditionsInfos[] = {
	IDMN_CND_ON_ERROR,					M_CND_ON_ERROR,					CND_ON_ERROR,					0,				0,
	IDMN_CND_ON_FUNCTION,				M_CND_ON_FUNCTION,				CND_ON_FUNCTION,				0,				1,	PARAM_EXPSTRING, M_FUNCTION_NAME,
	IDMN_CND_ON_PRINT,					M_CND_ON_PRINT,					CND_ON_PRINT,					0,				0,
	IDMN_CND_MMFI_ENABLED,				M_CND_MMFI_ENABLED,				CND_MMFI_ENABLED,				EVFLAGS_ALWAYS,	0,
	IDMN_CND_WINI_ENABLED,				M_CND_WINI_ENABLED,				CND_WINI_ENABLED,				EVFLAGS_ALWAYS,	0,
	IDMN_CND_BOUND_LUA_STATE,			M_CND_BOUND_LUA_STATE,			CND_BOUND_LUA_STATE,			EVFLAGS_ALWAYS,	0,
	IDMN_CND_CAPTURE_SEL_OBJECTS,		M_CND_CAPTURE_SEL_OBJECTS,		CND_CAPTURE_SEL_OBJECTS,		EVFLAGS_ALWAYS,	2,	PARAM_OBJECT, PARAM_EXPSTRING, M_OBJECT_TYPE, M_DEST_VAR,
	IDMN_CND_CAPTURE_SEL_LIST,			M_CND_CAPTURE_SEL_LIST,			CND_CAPTURE_SEL_LIST,			EVFLAGS_ALWAYS, 1,	PARAM_EXPSTRING, M_DEST_VAR,
	IDMN_CND_RESTORE_SEL_LIST,			M_CND_RESTORE_SEL_LIST,			CND_RESTORE_SEL_LIST,			EVFLAGS_ALWAYS, 1,	PARAM_EXPSTRING, M_SRC_VAR,
	IDMN_CND_ON_FUNCTION_A,				M_CND_ON_FUNCTION_A,			CND_ON_FUNCTION_A,				0,				1,	PARAM_STRING, M_FUNCTION_NAME,
	IDMN_CND_ON_FUNCTION_B,				M_CND_ON_FUNCTION_B,			CND_ON_FUNCTION_B,				0,				1,	PARAM_EXPSTRING, M_FUNCTION_NAME,
	IDMN_CND_ON_FUNCTION_C,				M_CND_ON_FUNCTION_C,			CND_ON_FUNCTION_C,				0,				1,	PARAM_EXPSTRING, M_FUNCTION_NAME,
	IDMN_CND_ON_FUNCTION_D,				M_CND_ON_FUNCTION_D,			CND_ON_FUNCTION_D,				0,				1,	PARAM_EXPSTRING, M_FUNCTION_NAME,
	IDMN_CND_ON_FUNCTION_E,				M_CND_ON_FUNCTION_E,			CND_ON_FUNCTION_E,				0,				1,	PARAM_EXPSTRING, M_FUNCTION_NAME,
	IDMN_CND_ON_FUNCTION_F,				M_CND_ON_FUNCTION_F,			CND_ON_FUNCTION_F,				0,				1,	PARAM_EXPSTRING, M_FUNCTION_NAME,
	IDMN_CND_ON_FUNCTION_G,				M_CND_ON_FUNCTION_G,			CND_ON_FUNCTION_G,				0,				1,	PARAM_EXPSTRING, M_FUNCTION_NAME,
};

// CondOnError ()
long WINAPI DLLExport CondOnError (LPRDATA rdPtr, long param1, long param2) {
	return true;
}

// CondOnFunction (char* funcName)
long WINAPI DLLExport CondOnFunction (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->autoReg) {
		rdPtr->luaMan->state->RegisterFunction(p1, 0);
		return false;
	}

	if (rdPtr->luaMan->state && rdPtr->luaMan->state->stack.GetFunction().compare(p1) == 0) {
		return true;
	}

	return false;
}

// CondOnPrint ()
long WINAPI DLLExport CondOnPrint (LPRDATA rdPtr, long param1, long param2) {
	return true;
}

// CondMMFIEnabled ()
long WINAPI DLLExport CondMMFIEnabled (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	if (rdPtr->luaMan->state) {
		return rdPtr->luaMan->state->mmfiReady;
	}
#endif
	return false;
}

// CondWINIEnabled ()
long WINAPI DLLExport CondWINIEnabled (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	if (rdPtr->luaMan->state) {
		return rdPtr->luaMan->state->winiReady;
	}
#endif
	return false;
}

// CondBoundXLuaState ()
long WINAPI DLLExport CondBoundXLuaState (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		return true;
	}
	return false;
}

// CondCaptureSelObject (Obj objType, char* destVar)
long WINAPI DLLExport CondCaptureSelObject (LPRDATA rdPtr, long param1, long param2) {
	eventParam* p1 = (eventParam*) param1;
	char* p2 = (char*) param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->SaveObjectSelectionType(p1, p2);
	}

	return true;
}

// CondCaptureSelList (char* destVar)
long WINAPI DLLExport CondCaptureSelList (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->SaveObjectSelection(p1);
	}

	return true;
}

// CondRestoreSelList (char* destVar)
long WINAPI DLLExport CondRestoreSelList (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->RestoreObjectSelection(p1);
	}

	return true;
}

// CondOnFunctionA (char* funcName)
long WINAPI DLLExport CondOnFunctionA (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->autoReg) {
		rdPtr->luaMan->state->RegisterFunction(p1, 0, MF_FUNCTION_GROUP_A);
		return false;
	}

	if (rdPtr->luaMan->state && rdPtr->luaMan->state->stack.GetFunction().compare(p1) == 0) {
		return true;
	}

	return false;
}

// CondOnFunctionB (char* funcName)
long WINAPI DLLExport CondOnFunctionB (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->autoReg) {
		rdPtr->luaMan->state->RegisterFunction(p1, 0, MF_FUNCTION_GROUP_B);
		return false;
	}

	if (rdPtr->luaMan->state && rdPtr->luaMan->state->stack.GetFunction().compare(p1) == 0) {
		return true;
	}

	return false;
}

// CondOnFunctionC (char* funcName)
long WINAPI DLLExport CondOnFunctionC (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->autoReg) {
		rdPtr->luaMan->state->RegisterFunction(p1, 0, MF_FUNCTION_GROUP_C);
		return false;
	}

	if (rdPtr->luaMan->state && rdPtr->luaMan->state->stack.GetFunction().compare(p1) == 0) {
		return true;
	}

	return false;
}

// CondOnFunctionD (char* funcName)
long WINAPI DLLExport CondOnFunctionD (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->autoReg) {
		rdPtr->luaMan->state->RegisterFunction(p1, 0, MF_FUNCTION_GROUP_D);
		return false;
	}

	if (rdPtr->luaMan->state && rdPtr->luaMan->state->stack.GetFunction().compare(p1) == 0) {
		return true;
	}

	return false;
}

// CondOnFunctionE (char* funcName)
long WINAPI DLLExport CondOnFunctionE (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->autoReg) {
		rdPtr->luaMan->state->RegisterFunction(p1, 0, MF_FUNCTION_GROUP_E);
		return false;
	}

	if (rdPtr->luaMan->state && rdPtr->luaMan->state->stack.GetFunction().compare(p1) == 0) {
		return true;
	}

	return false;
}

// CondOnFunctionF (char* funcName)
long WINAPI DLLExport CondOnFunctionF (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->autoReg) {
		rdPtr->luaMan->state->RegisterFunction(p1, 0, MF_FUNCTION_GROUP_F);
		return false;
	}

	if (rdPtr->luaMan->state && rdPtr->luaMan->state->stack.GetFunction().compare(p1) == 0) {
		return true;
	}

	return false;
}

// CondOnFunctionG (char* funcName)
long WINAPI DLLExport CondOnFunctionG (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->autoReg) {
		rdPtr->luaMan->state->RegisterFunction(p1, 0, MF_FUNCTION_GROUP_G);
		return false;
	}

	if (rdPtr->luaMan->state && rdPtr->luaMan->state->stack.GetFunction().compare(p1) == 0) {
		return true;
	}

	return false;
}

// ----------------------------------------------------------
// Condition jump table
// ----------------------------------------------------------
// Contains the address inside the extension of the different
// routines that handle the action, conditions and expressions.
// Located at the end of the source for convinience
// Must finish with a 0
//
long (WINAPI * ConditionJumps[]) (LPRDATA rdPtr, long param1, long param2) = {
	CondOnError,
	CondOnFunction,
	CondOnPrint,
	CondMMFIEnabled,
	CondWINIEnabled,
	CondBoundXLuaState,
	CondCaptureSelObject,
	CondCaptureSelList,
	CondRestoreSelList,
	CondOnFunctionA,
	CondOnFunctionB,
	CondOnFunctionC,
	CondOnFunctionD,
	CondOnFunctionE,
	CondOnFunctionF,
	CondOnFunctionG,
	0
};
