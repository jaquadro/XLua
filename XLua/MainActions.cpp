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
short actionsInfos[] = {
	IDMN_ACT_RUN_SOURCE_FILE,			M_ACT_RUN_SOURCE_FILE,			ACT_RUN_SOURCE_FILE,			0,				1,	PARAM_FILENAME2, M_FILE,
	IDMN_ACT_RUN_SOURCE_STRING,			M_ACT_RUN_SOURCE_STRING,		ACT_RUN_SOURCE_STRING,			0,				1,	PARAM_EXPSTRING, M_STRING,
	IDMN_ACT_PUSH_INT_PARAM,			M_ACT_PUSH_INT_PARAM,			ACT_PUSH_INT_PARAM,				0,				1,	PARAM_EXPRESSION, M_INTEGER,
	IDMN_ACT_PUSH_STRING_PARAM,			M_ACT_PUSH_STRING_PARAM,		ACT_PUSH_STRING_PARAM,			0,				1,	PARAM_EXPSTRING, M_STRING,
	IDMN_ACT_PUSH_BOOL_PARAM,			M_ACT_PUSH_BOOL_PARAM,			ACT_PUSH_BOOL_PARAM,			0,				1,	PARAM_EXPRESSION, M_BOOLEAN,
	IDMN_ACT_CALL_FUNCTION,				M_ACT_CALL_FUNCTION,			ACT_CALL_FUNCTION,				0,				1,	PARAM_EXPSTRING, M_FUNCTION_NAME,
	IDMN_ACT_PUSH_INT_RETURN,			M_ACT_PUSH_INT_RETURN,			ACT_PUSH_INT_RETURN,			0,				1,	PARAM_EXPRESSION, M_INTEGER,
	IDMN_ACT_PUSH_STRING_RETURN,		M_ACT_PUSH_STRING_RETURN,		ACT_PUSH_STRING_RETURN,			0,				1,	PARAM_EXPSTRING, M_STRING,
	IDMN_ACT_PUSH_BOOL_RETURN,			M_ACT_PUSH_BOOL_RETURN,			ACT_PUSH_BOOL_RETURN,			0,				1,	PARAM_EXPRESSION, M_BOOLEAN,
	IDMN_ACT_OPEN_LIBS_ALL,				M_ACT_OPEN_LIBS_ALL,			ACT_OPEN_LIBS_ALL,				0,				0,
	IDMN_ACT_OPEN_LIBS_BASE,			M_ACT_OPEN_LIBS_BASE,			ACT_OPEN_LIBS_BASE,				0,				0,
	IDMN_ACT_OPEN_LIBS_PACKAGE,			M_ACT_OPEN_LIBS_PACKAGE,		ACT_OPEN_LIBS_PACKAGE,			0,				0,
	IDMN_ACT_OPEN_LIBS_TABLE,			M_ACT_OPEN_LIBS_TABLE,			ACT_OPEN_LIBS_TABLE,			0,				0,
	IDMN_ACT_OPEN_LIBS_IO,				M_ACT_OPEN_LIBS_IO,				ACT_OPEN_LIBS_IO,				0,				0,
	IDMN_ACT_OPEN_LIBS_OS,				M_ACT_OPEN_LIBS_OS,				ACT_OPEN_LIBS_OS,				0,				0,
	IDMN_ACT_OPEN_LIBS_STRING,			M_ACT_OPEN_LIBS_STRING,			ACT_OPEN_LIBS_STRING,			0,				0,
	IDMN_ACT_OPEN_LIBS_MATH,			M_ACT_OPEN_LIBS_MATH,			ACT_OPEN_LIBS_MATH,				0,				0,
	IDMN_ACT_OPEN_LIBS_DEBUG,			M_ACT_OPEN_LIBS_DEBUG,			ACT_OPEN_LIBS_DEBUG,			0,				0,
	IDMN_ACT_OPEN_LIBS_BIT,				M_ACT_OPEN_LIBS_BIT,			ACT_OPEN_LIBS_BIT,				0,				0,
	IDMN_ACT_REGISTER_FUNCTION,			M_ACT_REGISTER_FUNCTION,		ACT_REGISTER_FUNCTION,			0,				2,	PARAM_EXPSTRING, PARAM_EXPRESSION, M_FUNCTION_NAME, M_MIN_PARAMS,
	IDMN_ACT_SET_INT_VAR,				M_ACT_SET_INT_VAR,				ACT_SET_INT_VAR,				0,				2,	PARAM_EXPSTRING, PARAM_EXPRESSION, M_VAR_NAME, M_INTEGER,
	IDMN_ACT_SET_STRING_VAR,			M_ACT_SET_STRING_VAR,			ACT_SET_STRING_VAR,				0,				2,	PARAM_EXPSTRING, PARAM_EXPSTRING, M_VAR_NAME, M_STRING,
	IDMN_ACT_SET_BOOL_VAR,				M_ACT_SET_BOOL_VAR,				ACT_SET_BOOL_VAR,				0,				2,	PARAM_EXPSTRING, PARAM_EXPRESSION, M_VAR_NAME, M_BOOLEAN,
	IDMN_ACT_EXPORT_OBJECT,				M_ACT_EXPORT_OBJECT,			ACT_EXPORT_OBJECT,				0,				2,	PARAM_OBJECT, PARAM_EXPRESSION, M_OBJECT_EXPORT, M_IDENTIFIER,
	IDMN_ACT_SET_LOCAL_VAL,				M_ACT_SET_LOCAL_VAL,			ACT_SET_LOCAL_VAL,				0,				2,	PARAM_EXPRESSION, PARAM_EXPRESSION, M_IDENTIFIER, M_VALUE,
	IDMN_ACT_SET_LOCAL_STRING,			M_ACT_SET_LOCAL_STRING,			ACT_SET_LOCAL_STRING,			0,				2,	PARAM_EXPRESSION, PARAM_EXPSTRING, M_IDENTIFIER, M_STRING,
	IDMN_ACT_ENABLE_MMFI,				M_ACT_ENABLE_MMFI,				ACT_ENABLE_MMFI,				0,				0,
	IDMN_ACT_ENABLE_JIT,				M_ACT_ENABLE_JIT,				ACT_ENABLE_JIT,					0,				0,
	IDMN_ACT_OPEN_LIBS_JIT,				M_ACT_OPEN_LIBS_JIT,			ACT_OPEN_LIBS_JIT,				0,				0,
	IDMN_ACT_ENABLE_WINI,				M_ACT_ENABLE_WINI,				ACT_ENABLE_WINI,				0,				0,
	IDMN_ACT_RESET_STATE,				M_ACT_RESET_STATE,				ACT_RESET_STATE,				0,				0,
	IDMN_ACT_RESET_LOCAL_STORE,			M_ACT_RESET_LOCAL_STORE,		ACT_RESET_LOCAL_STORE,			0,				0,
	IDMN_ACT_RESET_MMFI_EXPORTS,		M_ACT_RESET_MMFI_EXPORTS,		ACT_RESET_MMFI_EXPORTS,			0,				0,
	IDMN_ACT_RESET_WINI_EXPORTS,		M_ACT_RESET_WINI_EXPORTS,		ACT_RESET_WINI_EXPORTS,			0,				0,
	IDMN_ACT_CREATE_STATE,				M_ACT_CREATE_STATE,				ACT_CREATE_STATE,				0,				1,	PARAM_EXPRESSION, M_IDENTIFIER,
	IDMN_ACT_DESTROY_STATE,				M_ACT_DESTROY_STATE,			ACT_DESTROY_STATE,				0,				1,	PARAM_EXPRESSION, M_IDENTIFIER,
	IDMN_ACT_BIND_STATE,				M_ACT_BIND_STATE,				ACT_BIND_STATE,					0,				1,	PARAM_EXPRESSION, M_IDENTIFIER,
	IDMN_ACT_UNBIND_STATE,				M_ACT_UNBIND_STATE,				ACT_UNBIND_STATE,				0,				0,
	IDMN_ACT_INVOKE_MMF_FUNCTION,		M_ACT_INVOKE_MMF_FUNCTION,		ACT_INVOKE_MMF_FUNCTION,		0,				1,	PARAM_EXPSTRING, M_FUNCTION_NAME,
	IDMN_ACT_PUSH_FLOAT_PARAM,			M_ACT_PUSH_FLOAT_PARAM,			ACT_PUSH_FLOAT_PARAM,			0,				1,	PARAM_EXPRESSION, M_FLOAT,
	IDMN_ACT_PUSH_FLOAT_RETURN,			M_ACT_PUSH_FLOAT_RETURN,		ACT_PUSH_FLOAT_RETURN,			0,				1,	PARAM_EXPRESSION, M_FLOAT,
	IDMN_ACT_PUSH_NIL_PARAM,			M_ACT_PUSH_NIL_PARAM,			ACT_PUSH_NIL_PARAM,				0,				0,
	IDMN_ACT_PUSH_NIL_RETURN,			M_ACT_PUSH_NIL_RETURN,			ACT_PUSH_NIL_RETURN,			0,				0,
	IDMN_ACT_BIND_OPENGL_OBJECT,		M_ACT_BIND_OPENGL_OBJECT,		ACT_BIND_OPENGL_OBJECT,			0,				1,	PARAM_OBJECT, 0,
	IDMN_ACT_OPEN_LIBS_OPENGL,			M_ACT_OPEN_LIBS_OPENGL,			ACT_OPEN_LIBS_OPENGL,			0,				0,
	IDMN_ACT_OPEN_C_LIB,				M_ACT_OPEN_C_LIB,				ACT_OPEN_C_LIB,					0,				2,	PARAM_FILENAME2, PARAM_EXPSTRING, M_DLL_PATH, M_PACKAGE_NAME,
	IDMN_ACT_SET_FLOAT_VAR,				M_ACT_SET_FLOAT_VAR,			ACT_SET_FLOAT_VAR,				0,				2,	PARAM_EXPSTRING, PARAM_EXPRESSION, M_VAR_NAME, M_FLOAT,
	IDMN_ACT_CLEAR_MMFI_EXPORT,			M_ACT_CLEAR_MMFI_EXPORT,		ACT_CLEAR_MMFI_EXPORT,			0,				1,	PARAM_EXPRESSION, M_IDENTIFIER,
	IDMN_ACT_DELETE_VAR,				M_ACT_DELETE_VAR,				ACT_DELETE_VAR,					0,				1,	PARAM_EXPSTRING, M_VAR_NAME,
	IDMN_ACT_CALL_FUNC_INLINE_1,		M_ACT_CALL_FUNC_INLINE_1,		ACT_CALL_FUNC_INLINE_1,			0,				2,	PARAM_EXPSTRING, PARAM_EXPSTRING, M_FUNCTION_NAME, M_PARAM_1,
	IDMN_ACT_CALL_FUNC_INLINE_2,		M_ACT_CALL_FUNC_INLINE_2,		ACT_CALL_FUNC_INLINE_2,			0,				3,	PARAM_EXPSTRING, PARAM_EXPSTRING, PARAM_EXPSTRING, M_FUNCTION_NAME, M_PARAM_1, M_PARAM_2,
	IDMN_ACT_CALL_FUNC_INLINE_3,		M_ACT_CALL_FUNC_INLINE_3,		ACT_CALL_FUNC_INLINE_3,			0,				4,	PARAM_EXPSTRING, PARAM_EXPSTRING, PARAM_EXPSTRING, PARAM_EXPSTRING, M_FUNCTION_NAME, M_PARAM_1, M_PARAM_2, M_PARAM_3,
	IDMN_ACT_CALL_FUNC_INLINE_4,		M_ACT_CALL_FUNC_INLINE_4,		ACT_CALL_FUNC_INLINE_4,			0,				5,	PARAM_EXPSTRING, PARAM_EXPSTRING, PARAM_EXPSTRING, PARAM_EXPSTRING, PARAM_EXPSTRING, M_FUNCTION_NAME, M_PARAM_1, M_PARAM_2, M_PARAM_3, M_PARAM_4,
	IDMN_ACT_CALL_FUNC_INLINE_DELIM,	M_ACT_CALL_FUNC_INLINE_DELIM,	ACT_CALL_FUNC_INLINE_DELIM,		0,				2,	PARAM_EXPSTRING, PARAM_EXPSTRING, M_FUNCTION_NAME, M_PARAM_DELIM,
	IDMN_ACT_SET_ERROR_MODE_I,			M_ACT_SET_ERROR_MODE_I,			ACT_SET_ERROR_MODE_I,			0,				0,
	IDMN_ACT_SET_ERROR_MODE_Q,			M_ACT_SET_ERROR_MODE_Q,			ACT_SET_ERROR_MODE_Q,			0,				0,
	IDMN_ACT_SET_PRINT_MODE_I,			M_ACT_SET_PRINT_MODE_I,			ACT_SET_PRINT_MODE_I,			0,				0,
	IDMN_ACT_SET_PRINT_MODE_Q,			M_ACT_SET_PRINT_MODE_Q,			ACT_SET_PRINT_MODE_Q,			0,				0,
	IDMN_ACT_CAPTURE_SEL_LIST,			M_ACT_CAPTURE_SEL_LIST,			ACT_CAPTURE_SEL_LIST,			0,				1,	PARAM_EXPSTRING, M_DEST_VAR,
	IDMN_ACT_CAPTURE_SEL_OBJECTS,		M_ACT_CAPTURE_SEL_OBJECTS,		ACT_CAPTURE_SEL_OBJECTS,		0,				2,	PARAM_OBJECT, PARAM_EXPSTRING, M_OBJECT_TYPE, M_DEST_VAR,
	IDMN_ACT_RUN_SOURCE_STRING_NAMED,	M_ACT_RUN_SOURCE_STRING_NAMED,	ACT_RUN_SOURCE_STRING_NAMED,	0,				2,	PARAM_EXPSTRING, PARAM_EXPSTRING, M_STRING, M_NAME,
	IDMN_ACT_ENABLE_BACKTRACE,			M_ACT_ENABLE_BACKTRACE,			ACT_ENABLE_BACKTRACE,			0,				0,
	IDMN_ACT_DISABLE_BACKTRACE,			M_ACT_DISABLE_BACKTRACE,		ACT_DISABLE_BACKTRACE,			0,				0,
	IDMN_ACT_REGISTER_FUNCTION_A,		M_ACT_REGISTER_FUNCTION_A,		ACT_REGISTER_FUNCTION_A,		0,				2,	PARAM_EXPSTRING, PARAM_EXPRESSION, M_FUNCTION_NAME, M_MIN_PARAMS,
	IDMN_ACT_REGISTER_FUNCTION_B,		M_ACT_REGISTER_FUNCTION_B,		ACT_REGISTER_FUNCTION_B,		0,				2,	PARAM_EXPSTRING, PARAM_EXPRESSION, M_FUNCTION_NAME, M_MIN_PARAMS,
	IDMN_ACT_REGISTER_FUNCTION_C,		M_ACT_REGISTER_FUNCTION_C,		ACT_REGISTER_FUNCTION_C,		0,				2,	PARAM_EXPSTRING, PARAM_EXPRESSION, M_FUNCTION_NAME, M_MIN_PARAMS,
	IDMN_ACT_REGISTER_FUNCTION_D,		M_ACT_REGISTER_FUNCTION_D,		ACT_REGISTER_FUNCTION_D,		0,				2,	PARAM_EXPSTRING, PARAM_EXPRESSION, M_FUNCTION_NAME, M_MIN_PARAMS,
	IDMN_ACT_REGISTER_FUNCTION_E,		M_ACT_REGISTER_FUNCTION_E,		ACT_REGISTER_FUNCTION_E,		0,				2,	PARAM_EXPSTRING, PARAM_EXPRESSION, M_FUNCTION_NAME, M_MIN_PARAMS,
	IDMN_ACT_REGISTER_FUNCTION_F,		M_ACT_REGISTER_FUNCTION_F,		ACT_REGISTER_FUNCTION_F,		0,				2,	PARAM_EXPSTRING, PARAM_EXPRESSION, M_FUNCTION_NAME, M_MIN_PARAMS,
	IDMN_ACT_REGISTER_FUNCTION_G,		M_ACT_REGISTER_FUNCTION_G,		ACT_REGISTER_FUNCTION_G,		0,				2,	PARAM_EXPSTRING, PARAM_EXPRESSION, M_FUNCTION_NAME, M_MIN_PARAMS,
	IDMN_ACT_EXPORT_OBJECT_FIXED,		M_ACT_EXPORT_OBJECT_FIXED,		ACT_EXPORT_OBJECT_FIXED,		0,				2,	PARAM_EXPRESSION, PARAM_EXPRESSION, M_OBJECT_EXPORT_FIXED, M_IDENTIFIER,
	IDMN_ACT_RUN_SOURCE_EMBEDDED,		M_ACT_RUN_SOURCE_EMBEDDED,		ACT_RUN_SOURCE_EMBEDDED,		0,				1,	PARAM_EXPSTRING, M_SCRIPT_NAME,
	IDMN_ACT_PUSH_TABLE_PARAM_BEGIN,	M_ACT_PUSH_TABLE_PARAM_BEGIN,	ACT_PUSH_TABLE_PARAM_BEGIN,		0,				0,
	IDMN_ACT_PUSH_TABLE_PARAM_END,		M_ACT_PUSH_TABLE_PARAM_END,		ACT_PUSH_TABLE_PARAM_END,		0,				0,
	IDMN_ACT_PUSH_TABLE_RETURN_BEGIN,	M_ACT_PUSH_TABLE_RETURN_BEGIN,	ACT_PUSH_TABLE_RETURN_BEGIN,	0,				0,
	IDMN_ACT_PUSH_TABLE_RETURN_END,		M_ACT_PUSH_TABLE_RETURN_END,	ACT_PUSH_TABLE_RETURN_END,		0,				0,
};

// ActRunSourceFile (char* filename)
short WINAPI DLLExport ActRunSourceFile (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadFile(p1);
	}

	return 0;
}

// ActRunSourceString (char* source)
short WINAPI DLLExport ActRunSourceString (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadString(p1);
	}

	return 0;
}

// ActPushIntParam (int val)
short WINAPI DLLExport ActPushIntParam (LPRDATA rdPtr, long param1, long param2) {
	int p1 = param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.PushParam(p1);
	}

	return 0;
}

// ActPushStringParam (char* val)
short WINAPI DLLExport ActPushStringParam (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.PushParam(p1);
	}

	return 0;
}

// ActPushBoolParam (bool val)
short WINAPI DLLExport ActPushBoolParam (LPRDATA rdPtr, long param1, long param2) {
	int p1 = param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.PushParam(p1 != 0);
	}

	return 0;
}

// ActCallFunction (char* func)
short WINAPI DLLExport ActCallFunction (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.CallLuaFunction(p1);
	}

	return 0;
}

// ActPushIntReturn (int val)
short WINAPI DLLExport ActPushIntReturn (LPRDATA rdPtr, long param1, long param2) {
	int p1 = param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.PushReturn(p1);
	}

	return 0;
}

// ActPushStringReturn (char* val)
short WINAPI DLLExport ActPushStringReturn (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.PushReturn(p1);
	}

	return 0;
}

// ActPushBoolReturn (bool val)
short WINAPI DLLExport ActPushBoolReturn (LPRDATA rdPtr, long param1, long param2) {
	int p1 = param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.PushReturn(p1 != 0);
	}

	return 0;
}

// ActOpenAllLibs ()
short WINAPI DLLExport ActOpenAllLibs (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadDefaultLib(XLuaState::PACKAGE_ALL);
	}

	return 0;
}

// ActOpenBaseLib ()
short WINAPI DLLExport ActOpenBaseLib (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadDefaultLib(XLuaState::PACKAGE_BASE);
	}

	return 0;
}

// ActOpenPackageLib ()
short WINAPI DLLExport ActOpenPackageLib (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadDefaultLib(XLuaState::PACKAGE_PACKAGE);
	}

	return 0;
}

// ActOpenTableLib ()
short WINAPI DLLExport ActOpenTableLib (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadDefaultLib(XLuaState::PACKAGE_TABLE);
	}

	return 0;
}

// ActOpenIoLib ()
short WINAPI DLLExport ActOpenIoLib (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadDefaultLib(XLuaState::PACKAGE_IO);
	}

	return 0;
}

// ActOpenOsLib ()
short WINAPI DLLExport ActOpenOsLib (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadDefaultLib(XLuaState::PACKAGE_OS);
	}

	return 0;
}

// ActOpenStringLib ()
short WINAPI DLLExport ActOpenStringLib (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadDefaultLib(XLuaState::PACKAGE_STRING);
	}

	return 0;
}

// ActOpenMathLib ()
short WINAPI DLLExport ActOpenMathLib (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadDefaultLib(XLuaState::PACKAGE_MATH);
	}

	return 0;
}

// ActOpenDebugLib ()
short WINAPI DLLExport ActOpenDebugLib (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadDefaultLib(XLuaState::PACKAGE_DEBUG);
	}

	return 0;
}

// ActOpenBitLib ()
short WINAPI DLLExport ActOpenBitLib (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadDefaultLib(XLuaState::PACKAGE_BIT);
	}

	return 0;
}

// ActRegisterFunc (char* func, int minParams)
short WINAPI DLLExport ActRegisterFunc (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	int p2 = param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->RegisterFunction(p1, p2);
	}

	return 0;
}

// ActSetIntVar (char* var, int val)
short WINAPI DLLExport ActSetIntVar (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	int p2 = param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->SetNumericVariable(p1, p2);
	}

	return 0;
}

// ActSetStringVar (char* var, char* val)
short WINAPI DLLExport ActSetStringVar (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	char* p2 = (char*) param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->SetStringVariable(p1, p2);
	}

	return 0;
}

// ActSetBoolVar (char* var, bool val)
short WINAPI DLLExport ActSetBoolVar (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	int p2 = param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->SetBooleanVariable(p1, p2 != 0);
	}

	return 0;
}

// ActExportObj (Obj object, int id)
short WINAPI DLLExport ActExportObj (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	LPRO p1 = (LPRO) param1;
	int p2 = param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->ExportObject(p2, p1);
	}
#endif
	return 0;
}

// ActSetLocalVal (int id, int val)
short WINAPI DLLExport ActSetLocalVal (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	long  p1 = (long) CNC_GetParameter(rdPtr);
	long  t2 = (long) CNC_GetParameter(rdPtr);

	float p2 = *(float*) &t2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->SetLocalValue(p1, p2);
	}
#endif
	return 0;
}

// ActSetLocalStr (int id, char* val)
short WINAPI DLLExport ActSetLocalStr (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	int p1 = param1;
	char* p2 = (char*) param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->SetLocalString(p1, p2);
	}
#endif
	return 0;
}

// ActEnableMMFI ()
short WINAPI DLLExport ActEnableMMFI (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->mmfi.Register();
	}
#endif
	return 0;
}

// ActEnableJit ()
short WINAPI DLLExport ActEnableJit (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->EnableJIT();
	}

	return 0;
}

// ActOpenJitLib ()
short WINAPI DLLExport ActOpenJitLib (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadDefaultLib(XLuaState::PACKAGE_JIT);
	}

	return 0;
}

// ActEnableWINI ()
short WINAPI DLLExport ActEnableWINI (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->mmfi.RegisterWin();
	}
#endif
	return 0;
}

// ActResetState ()
short WINAPI DLLExport ActResetState (LPRDATA rdPtr, long param1, long param2) {
	return 0;
}

// ActResetLocalStore ()
short WINAPI DLLExport ActResetLocalStore (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->ResetLocalStorage();
	}
#endif
	return 0;
}

// ActResetMMFI ()
short WINAPI DLLExport ActResetMMFI (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->ResetObjectExports();
	}
#endif
	return 0;
}

// ActResetWINI ()
short WINAPI DLLExport ActResetWINI (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->ResetWinExports();
	}
#endif
	return 0;
}

// ActCreateState (int id)
short WINAPI DLLExport ActCreateState (LPRDATA rdPtr, long param1, long param2) {
	int p1 = param1;
	XLuaGlobal::Get().CreateState(p1);

	return 0;
}

// ActDestroyState (int id)
short WINAPI DLLExport ActDestroyState (LPRDATA rdPtr, long param1, long param2) {
	int p1 = param1;
	XLuaGlobal::Get().DeleteState(p1);

	return 0;
}

// ActBindState (int id)
short WINAPI DLLExport ActBindState (LPRDATA rdPtr, long param1, long param2) {
	int p1 = param1;
	rdPtr->luaMan->BindState(p1);

	return 0;
}

// ActUnbindState ()
short WINAPI DLLExport ActUnbindState (LPRDATA rdPtr, long param1, long param2) {
	rdPtr->luaMan->UnbindState();

	return 0;
}

// ActInvokeMmfFunc (char* func)
short WINAPI DLLExport ActInvokeMmfFunc (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.CallMMFFunctionFromMMF(p1);
	}

	return 0;
}

// ActPushFloatParam (float val)
short WINAPI DLLExport ActPushFloatParam (LPRDATA rdPtr, long param1, long param2) {
	long  t1 = (long) CNC_GetParameter(rdPtr);

	float p1 = *(float*) &t1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.PushParam(p1);
	}

	return 0;
}

// ActPushFloatReturn (float val)
short WINAPI DLLExport ActPushFloatReturn (LPRDATA rdPtr, long param1, long param2) {
	long  t1 = (long) CNC_GetParameter(rdPtr);

	float p1 = *(float*) &t1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.PushReturn(p1);
	}

	return 0;
}

// ActPushNilParam ()
short WINAPI DLLExport ActPushNilParam (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.PushParam();
	}

	return 0;
}

// ActPushNilReturn ()
short WINAPI DLLExport ActPushNilReturn (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.PushReturn();
	}

	return 0;
}

// ActBindOpengl ()
short WINAPI DLLExport ActBindOpengl (LPRDATA rdPtr, long param1, long param2) {
	return 0;
}

// ActOpenGlLib ()
short WINAPI DLLExport ActOpenGlLib (LPRDATA rdPtr, long param1, long param2) {
	return 0;
}

// ActOpenCLib (char* path, char* packagename)
short WINAPI DLLExport ActOpenCLib (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	char* p2 = (char*) param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadCModule(p1, p2);
	}

	return 0;
}

// ActSetFloatVar (char* var, float val)
short WINAPI DLLExport ActSetFloatVar (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) CNC_GetParameter(rdPtr);
	long  t2 = (long) CNC_GetParameter(rdPtr);

	float p2 = *(float*) &t2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->SetFloatingVariable(p1, p2);
	}

	return 0;
}

// ActClearExportObj (int id)
short WINAPI DLLExport ActClearExportObj (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	int p1 = param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->ClearExport(p1);
	}
#endif
	return 0;
}

// ActDeleteVar (char* var)
short WINAPI DLLExport ActDeleteVar (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->SetNilVariable(p1);
	}

	return 0;
}

// ActCallFuncInline1 (char* func, char* v1)
short WINAPI DLLExport ActCallFucnInline1 (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	char* p2 = (char*) param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.ClearLFParameters();
		rdPtr->luaMan->state->stack.PushParam(p2);
		rdPtr->luaMan->state->stack.CallLuaFunction(p1);
	}

	return 0;
}

// ActCallFuncInline2 (char* func, char* v1, char* v2)
short WINAPI DLLExport ActCallFucnInline2 (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) CNC_GetParameter(rdPtr);
	char* p2 = (char*) CNC_GetParameter(rdPtr);
	char* p3 = (char*) CNC_GetParameter(rdPtr);

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.ClearLFParameters();
		rdPtr->luaMan->state->stack.PushParam(p2);
		rdPtr->luaMan->state->stack.PushParam(p3);
		rdPtr->luaMan->state->stack.CallLuaFunction(p1);
	}

	return 0;
}

// ActCallFuncInline3 (char* func, char* v1, char* v2, char* v3)
short WINAPI DLLExport ActCallFucnInline3 (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) CNC_GetParameter(rdPtr);
	char* p2 = (char*) CNC_GetParameter(rdPtr);
	char* p3 = (char*) CNC_GetParameter(rdPtr);
	char* p4 = (char*) CNC_GetParameter(rdPtr);

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.ClearLFParameters();
		rdPtr->luaMan->state->stack.PushParam(p2);
		rdPtr->luaMan->state->stack.PushParam(p3);
		rdPtr->luaMan->state->stack.PushParam(p4);
		rdPtr->luaMan->state->stack.CallLuaFunction(p1);
	}

	return 0;
}

// ActCallFuncInline4 (char* func, char* v1, char* v2, char* v3, char* v4)
short WINAPI DLLExport ActCallFucnInline4 (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) CNC_GetParameter(rdPtr);
	char* p2 = (char*) CNC_GetParameter(rdPtr);
	char* p3 = (char*) CNC_GetParameter(rdPtr);
	char* p4 = (char*) CNC_GetParameter(rdPtr);
	char* p5 = (char*) CNC_GetParameter(rdPtr);

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.ClearLFParameters();
		rdPtr->luaMan->state->stack.PushParam(p2);
		rdPtr->luaMan->state->stack.PushParam(p3);
		rdPtr->luaMan->state->stack.PushParam(p4);
		rdPtr->luaMan->state->stack.PushParam(p5);
		rdPtr->luaMan->state->stack.CallLuaFunction(p1);
	}

	return 0;
}

// ActCallFuncInlineDelim (char* func, char* v1)
short WINAPI DLLExport ActCallFucnInlineDelim (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	char* p2 = (char*) param2;

	if (!rdPtr->luaMan->state) {
		return 0;
	}

	rdPtr->luaMan->state->stack.ClearLFParameters();
	std::string str(p2);

	int nidx, pidx = 0;
	
	do {
		nidx = str.find_first_of("|", pidx);
		rdPtr->luaMan->state->stack.PushParam(str.substr(pidx, nidx - pidx).c_str());
		pidx = nidx + 1;
	} while (nidx != std::string::npos);

	rdPtr->luaMan->state->stack.CallLuaFunction(p1);

	return 0;
}

// ActSetErrModeI ()
short WINAPI DLLExport ActSetErrModeI (LPRDATA rdPtr, long param1, long param2) {
	rdPtr->luaMan->errMode = 0;
	return 0;
}

// ActSetErrModeQ ()
short WINAPI DLLExport ActSetErrModeQ (LPRDATA rdPtr, long param1, long param2) {
	rdPtr->luaMan->errMode = 1;
	return 0;
}

// ActSetPrintModeI ()
short WINAPI DLLExport ActSetPrintModeI (LPRDATA rdPtr, long param1, long param2) {
	rdPtr->luaMan->printMode = 0;
	return 0;
}

// ActSetPrintModeQ ()
short WINAPI DLLExport ActSetPrintModeQ (LPRDATA rdPtr, long param1, long param2) {
	rdPtr->luaMan->printMode = 1;
	return 0;
}

// ActCaptureSelList (char* destVar)
short WINAPI DLLExport ActCaptureSelList (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->SaveObjectSelection(p1);
	}

	return 0;
}

// ActCaptureSelObject (Obj object, char* destVar)
short WINAPI DLLExport ActCaptureSelObject (LPRDATA rdPtr, long param1, long param2) {
	return 0;
}

// ActRunNamedSourceStr (char* source, char* name)
short WINAPI DLLExport ActRunNamedSourceStr (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	char* p2 = (char*) param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadString(p1, p2);
	}

	return 0;
}

// ActEnableBacktrace ()
short WINAPI DLLExport ActEnableBacktrace (LPRDATA rdPtr, long param1, long param2) {
	rdPtr->luaMan->useBacktrace = true;
	return 0;
}

// ActDisableBacktrace ()
short WINAPI DLLExport ActDisableBacktrace (LPRDATA rdPtr, long param1, long param2) {
	rdPtr->luaMan->useBacktrace = false;
	return 0;
}

// ActRegisterFuncA (char* func, int minparams)
short WINAPI DLLExport ActRegisterFuncA (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	int p2 = param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->RegisterFunction(p1, p2, MF_FUNCTION_GROUP_A);
	}

	return 0;
}

// ActRegisterFuncB (char* func, int minparams)
short WINAPI DLLExport ActRegisterFuncB (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	int p2 = param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->RegisterFunction(p1, p2, MF_FUNCTION_GROUP_B);
	}

	return 0;
}

// ActRegisterFuncC (char* func, int minparams)
short WINAPI DLLExport ActRegisterFuncC (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	int p2 = param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->RegisterFunction(p1, p2, MF_FUNCTION_GROUP_C);
	}

	return 0;
}

// ActRegisterFuncD (char* func, int minparams)
short WINAPI DLLExport ActRegisterFuncD (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	int p2 = param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->RegisterFunction(p1, p2, MF_FUNCTION_GROUP_D);
	}

	return 0;
}

// ActRegisterFuncE (char* func, int minparams)
short WINAPI DLLExport ActRegisterFuncE (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	int p2 = param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->RegisterFunction(p1, p2, MF_FUNCTION_GROUP_E);
	}

	return 0;
}

// ActRegisterFuncF (char* func, int minparams)
short WINAPI DLLExport ActRegisterFuncF (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	int p2 = param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->RegisterFunction(p1, p2, MF_FUNCTION_GROUP_F);
	}

	return 0;
}

// ActRegisterFuncG (char* func, int minparams)
short WINAPI DLLExport ActRegisterFuncG (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;
	int p2 = param2;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->RegisterFunction(p1, p2, MF_FUNCTION_GROUP_G);
	}

	return 0;
}

// ActExportObjFixed (int objid, int id)
short WINAPI DLLExport ActExportObjFixed (LPRDATA rdPtr, long param1, long param2) {
#ifdef XLUA_LEGACY
	int p1 = param1;
	int p2 = param2;

	//rdPtr->luaMan->ExportObject(p2, p1);
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->ExportObjectByFixed(p2, p1, rdPtr);
	}
#endif
	return 0;
}

// ActRunEmbedded (char* name)
short WINAPI DLLExport ActRunEmbedded (LPRDATA rdPtr, long param1, long param2) {
	char* p1 = (char*) param1;

	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->LoadEmbedded(p1, rdPtr);
	}

	return 0;
}

// ActPushTableParamStart ()
short WINAPI DLLExport ActPushTableParamStart (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.StartParamTable();
	}

	return 0;
}

// ActPushTableParamEnd ()
short WINAPI DLLExport ActPushTableParamEnd (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.EndParamTable();
	}

	return 0;
}

// ActPushTableReturnStart ()
short WINAPI DLLExport ActPushTableReturnStart (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.StartReturnTable();
	}

	return 0;
}

// ActPushTableReturnEnd ()
short WINAPI DLLExport ActPushTableReturnEnd (LPRDATA rdPtr, long param1, long param2) {
	if (rdPtr->luaMan->state) {
		rdPtr->luaMan->state->stack.EndReturnTable();
	}

	return 0;
}

// ----------------------------------------------------------
// Action jump table
// ----------------------------------------------------------
// Contains the address inside the extension of the different
// routines that handle the action, conditions and expressions.
// Located at the end of the source for convinience
// Must finish with a 0
//
short (WINAPI * ActionJumps[])(LPRDATA rdPtr, long param1, long param2) = {
	ActRunSourceFile,
	ActRunSourceString,
	ActPushIntParam,
	ActPushStringParam,
	ActPushBoolParam,
	ActCallFunction,
	ActPushIntReturn,
	ActPushStringReturn,
	ActPushBoolReturn,
	ActOpenAllLibs,
	ActOpenBaseLib,
	ActOpenPackageLib,
	ActOpenTableLib,
	ActOpenIoLib,
	ActOpenOsLib,
	ActOpenStringLib,
	ActOpenMathLib,
	ActOpenDebugLib,
	ActOpenBitLib,
	ActRegisterFunc,
	ActSetIntVar,
	ActSetStringVar,
	ActSetBoolVar,
	ActExportObj,
	ActSetLocalVal,
	ActSetLocalStr,
	ActEnableMMFI,
	ActEnableJit,
	ActOpenJitLib,
	ActEnableWINI,
	ActResetState,
	ActResetLocalStore,
	ActResetMMFI,
	ActResetWINI,
	ActCreateState,
	ActDestroyState,
	ActBindState,
	ActUnbindState,
	ActInvokeMmfFunc,
	ActPushFloatParam,
	ActPushFloatReturn,
	ActPushNilParam,
	ActPushNilReturn,
	ActBindOpengl,
	ActOpenGlLib,
	ActOpenCLib,
	ActSetFloatVar,
	ActClearExportObj,
	ActDeleteVar,
	ActCallFucnInline1,
	ActCallFucnInline2,
	ActCallFucnInline3,
	ActCallFucnInline4,
	ActCallFucnInlineDelim,
	ActSetErrModeI,
	ActSetErrModeQ,
	ActSetPrintModeI,
	ActSetPrintModeQ,
	ActCaptureSelList,
	ActCaptureSelObject,
	ActRunNamedSourceStr,
	ActEnableBacktrace,
	ActDisableBacktrace,
	ActRegisterFuncA,
	ActRegisterFuncB,
	ActRegisterFuncC,
	ActRegisterFuncD,
	ActRegisterFuncE,
	ActRegisterFuncF,
	ActRegisterFuncG,
	ActExportObjFixed,
	ActRunEmbedded,
	ActPushTableParamStart,
	ActPushTableParamEnd,
	ActPushTableReturnStart,
	ActPushTableReturnEnd,
	0
};
