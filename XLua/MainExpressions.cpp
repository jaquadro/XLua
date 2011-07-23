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
short expressionsInfos[] = {
	IDMN_EXP_ERROR_STRING,				M_EXP_ERROR_STRING,				EXP_ERROR_STRING,				EXPFLAG_STRING,	0,
	IDMN_EXP_INT_RETURN,				M_EXP_INT_RETURN,				EXP_INT_RETURN,					0,				1,	EXPPARAM_LONG, M_INDEX_1,
	IDMN_EXP_STRING_RETURN,				M_EXP_STRING_RETURN,			EXP_STRING_RETURN,				EXPFLAG_STRING,	1,	EXPPARAM_LONG, M_INDEX_1,
	IDMN_EXP_BOOL_RETURN,				M_EXP_BOOL_RETURN,				EXP_BOOL_RETURN,				0,				1,	EXPPARAM_LONG, M_INDEX_1,
	IDMN_EXP_RETURN_COUNT,				M_EXP_RETURN_COUNT,				EXP_RETURN_COUNT,				0,				0,
	IDMN_EXP_INT_PARAM,					M_EXP_INT_PARAM,				EXP_INT_PARAM,					0,				1,	EXPPARAM_LONG, M_INDEX_1,
	IDMN_EXP_STRING_PARAM,				M_EXP_STRING_PARAM,				EXP_STRING_PARAM,				EXPFLAG_STRING,	1,	EXPPARAM_LONG, M_INDEX_1,
	IDMN_EXP_BOOL_PARAM,				M_EXP_BOOL_PARAM,				EXP_BOOL_PARAM,					0,				1,	EXPPARAM_LONG, M_INDEX_1,
	IDMN_EXP_PARAM_COUNT,				M_EXP_PARAM_COUNT,				EXP_PARAM_COUNT,				0,				0,
	IDMN_EXP_INT_VAR,					M_EXP_INT_VAR,					EXP_INT_VAR,					0,				1,	EXPPARAM_STRING, M_VAR_NAME,
	IDMN_EXP_STRING_VAR,				M_EXP_STRING_VAR,				EXP_STRING_VAR,					EXPFLAG_STRING,	1,	EXPPARAM_STRING, M_VAR_NAME,
	IDMN_EXP_BOOL_VAR,					M_EXP_BOOL_VAR,					EXP_BOOL_VAR,					0,				1,	EXPPARAM_STRING, M_VAR_NAME,
	IDMN_EXP_LOCAL_VAL,					M_EXP_LOCAL_VAL,				EXP_LOCAL_VAL,					EXPFLAG_DOUBLE,	1,	EXPPARAM_LONG, M_IDENTIFIER,
	IDMN_EXP_LOCAL_STRING,				M_EXP_LOCAL_STRING,				EXP_LOCAL_STRING,				EXPFLAG_STRING,	1,	EXPPARAM_LONG, M_IDENTIFIER,
	IDMN_EXP_PRINT_STRING,				M_EXP_PRINT_STRING,				EXP_PRINT_STRING,				EXPFLAG_STRING,	0,
	IDMN_EXP_INT_FUNCTION,				M_EXP_INT_FUNCTION,				EXP_INT_FUNCTION,				0,				1,	EXPPARAM_STRING, M_FUNCTION_NAME,
	IDMN_EXP_STRING_FUNCTION,			M_EXP_STRING_FUNCTION,			EXP_STRING_FUNCTION,			EXPFLAG_STRING,	1,	EXPPARAM_STRING, M_FUNCTION_NAME,
	IDMN_EXP_INT_FUNCTION_NP,			M_EXP_INT_FUNCTION_NP,			EXP_INT_FUNCTION_NP,			0,				2,	EXPPARAM_STRING, EXPPARAM_LONG, M_FUNCTION_NAME, M_INTEGER,
	IDMN_EXP_INT_FUNCTION_SP,			M_EXP_INT_FUNCTION_SP,			EXP_INT_FUNCTION_SP,			0,				2,	EXPPARAM_STRING, EXPPARAM_STRING, M_FUNCTION_NAME, M_STRING,
	IDMN_EXP_STRING_FUNCTION_NP,		M_EXP_STRING_FUNCTION_NP,		EXP_STRING_FUNCTION_NP,			EXPFLAG_STRING,	2,	EXPPARAM_STRING, EXPPARAM_LONG, M_FUNCTION_NAME, M_INTEGER,
	IDMN_EXP_STRING_FUNCTION_SP,		M_EXP_STRING_FUNCTION_SP,		EXP_STRING_FUNCTION_SP,			EXPFLAG_STRING, 2,	EXPPARAM_STRING, EXPPARAM_STRING, M_FUNCTION_NAME, M_STRING,
	IDMN_EXP_FLOAT_RETURN,				M_EXP_FLOAT_RETURN,				EXP_FLOAT_RETURN,				EXPFLAG_DOUBLE,	1,	EXPPARAM_LONG, M_INDEX_1,
	IDMN_EXP_FLOAT_PARAM,				M_EXP_FLOAT_PARAM,				EXP_FLOAT_PARAM,				EXPFLAG_DOUBLE,	1,	EXPPARAM_LONG, M_INDEX_1,
	IDMN_EXP_FLOAT_FUNCTION,			M_EXP_FLOAT_FUNCTION,			EXP_FLOAT_FUNCTION,				EXPFLAG_DOUBLE, 1,	EXPPARAM_STRING, M_FUNCTION_NAME,
	IDMN_EXP_FLOAT_FUNCTION_FP,			M_EXP_FLOAT_FUNCTION_FP,		EXP_FLOAT_FUNCTION_FP,			EXPFLAG_DOUBLE, 2,	EXPPARAM_STRING, EXPPARAM_LONG, M_FUNCTION_NAME, M_FLOAT,
	IDMN_EXP_FLOAT_FUNCTION_SP,			M_EXP_FLOAT_FUNCTION_SP,		EXP_FLOAT_FUNCTION_SP,			EXPFLAG_DOUBLE,	2,	EXPPARAM_STRING, EXPPARAM_STRING, M_FUNCTION_NAME, M_STRING,
	IDMN_EXP_FLOAT_VAR,					M_EXP_FLOAT_VAR,				EXP_FLOAT_VAR,					EXPFLAG_DOUBLE,	1,	EXPPARAM_STRING, M_VAR_NAME,
	IDMN_EXP_TABLE_LENGTH,				M_EXP_TABLE_LENGTH,				EXP_TABLE_LENGTH,				0,				1,	EXPPARAM_STRING, M_TABLE_NAME,
	IDMN_EXP_TABLE_MAXN,				M_EXP_TABLE_MAXN,				EXP_TABLE_MAXN,					0,				1,	EXPPARAM_STRING, M_TABLE_NAME,
	IDMN_EXP_EMBEDDED_DEP,				M_EXP_EMBEDDED_DEP,				EXP_EMBEDDED_DEP,				EXPFLAG_STRING,	0,
	IDMN_EXP_ERROR_MODE,				M_EXP_ERROR_MODE,				EXP_ERROR_MODE,					EXPFLAG_STRING, 0,
	IDMN_EXP_PRINT_MODE,				M_EXP_PRINT_MODE,				EXP_PRINT_MODE,					EXPFLAG_STRING, 0,
	IDMN_EXP_VAR_TYPE,					M_EXP_VAR_TYPE,					EXP_VAR_TYPE,					EXPFLAG_STRING, 1,	EXPPARAM_STRING, M_VAR_NAME,
	IDMN_EXP_EMBEDDED,					M_EXP_EMBEDDED,					EXP_EMBEDDED,					EXPFLAG_STRING, 1,	EXPPARAM_STRING, M_SCRIPT_NAME,
};

// char* ExpErrorStr ()
long WINAPI DLLExport ExpErrorStr (LPRDATA rdPtr,long param1) {
	const std::string& ret = rdPtr->luaMan->GetError();
	
	rdPtr->rHo.hoFlags |= HOF_STRING;
	return (long) ret.c_str();
}

// int ExpIntReturn (int index)
long WINAPI DLLExport ExpIntReturn (LPRDATA rdPtr,long param1) {
	long p1 = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	unsigned fpcontrol = fp_precision_double();
	int ret = (int) rdPtr->luaMan->state->stack.GetNumberReturn(p1);
	fp_precision_restore(fpcontrol);

	return ret;
}

// char* ExpStrReturn (int index)
long WINAPI DLLExport ExpStrReturn (LPRDATA rdPtr, long param1) {
	long p1 = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	rdPtr->rHo.hoFlags |= HOF_STRING;

	if (rdPtr->luaMan->state == NULL) {
		return (long) "";
	}

	const std::string& ret = rdPtr->luaMan->state->stack.GetStringReturn(p1);
	
	
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
}

// int ExpBoolReturn (int index)
long WINAPI DLLExport ExpBoolReturn (LPRDATA rdPtr,long param1) {
	long p1 = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	return rdPtr->luaMan->state->stack.GetBooleanReturn(p1);
}

// int ExpReturnCount ()
long WINAPI DLLExport ExpReturnCount (LPRDATA rdPtr,long param1) {
	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	return rdPtr->luaMan->state->stack.GetReturnCount();
}

// int ExpIntParam (int index)
long WINAPI DLLExport ExpIntParam (LPRDATA rdPtr,long param1) {
	long p1 = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	unsigned fpcontrol = fp_precision_double();
	int ret = (int) rdPtr->luaMan->state->stack.GetNumberParam(p1);
	fp_precision_restore(fpcontrol);

	return ret;
}

// char* ExpStrParam (int index)
long WINAPI DLLExport ExpStrParam (LPRDATA rdPtr,long param1) {
	long p1 = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	rdPtr->rHo.hoFlags |= HOF_STRING;

	if (rdPtr->luaMan->state == NULL) {
		return (long) "";
	}

	const std::string& ret = rdPtr->luaMan->state->stack.GetStringParam(p1);
	
	
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
}

// int ExpBoolParam (int index)
long WINAPI DLLExport ExpBoolParam (LPRDATA rdPtr,long param1) {
	long p1 = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	return rdPtr->luaMan->state->stack.GetBooleanParam(p1);
}

// int ExpParamCount ()
long WINAPI DLLExport ExpParamCount (LPRDATA rdPtr,long param1) {
	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	return rdPtr->luaMan->state->stack.GetParamCount();
}

// int ExpIntVar (char* var)
long WINAPI DLLExport ExpIntVar (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	return rdPtr->luaMan->state->GetNumericVariable(p1);
}

// char* ExpStrVar (char* var)
long WINAPI DLLExport ExpStrVar (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);

	rdPtr->rHo.hoFlags |= HOF_STRING;

	if (rdPtr->luaMan->state == NULL) {
		return (long) "";
	}

	const std::string& ret = rdPtr->luaMan->state->GetStringVariable(p1);
	
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
}

// int ExpBoolVar (char* var)
long WINAPI DLLExport ExpBoolVar (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	return rdPtr->luaMan->state->GetBooleanVariable(p1);
}

// float ExpLocalVal (int id)
long WINAPI DLLExport ExpLocalVal (LPRDATA rdPtr,long param1) {
	long p1 = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	rdPtr->rHo.hoFlags |= HOF_FLOAT;

#ifdef XLUA_LEGACY
	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	float ret = (float) rdPtr->luaMan->state->GetLocalValue(p1);

	return *((int*)&ret);
#else
	return 0;
#endif
}

// char* ExpLocalStr (char* var)
long WINAPI DLLExport ExpLocalStr (LPRDATA rdPtr,long param1) {
	long p1 = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	rdPtr->rHo.hoFlags |= HOF_STRING;

#ifdef XLUA_LEGACY
	if (rdPtr->luaMan->state == NULL) {
		return (long) "";
	}

	const std::string& ret = rdPtr->luaMan->state->GetLocalString(p1);
	
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
#else
	return (long) "";
#endif
}

// char* ExpPrintStr ()
long WINAPI DLLExport ExpPrintStr (LPRDATA rdPtr,long param1) {
	const std::string& ret = rdPtr->luaMan->GetPrint();
	
	rdPtr->rHo.hoFlags |= HOF_STRING;
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
	//return (long) ret.c_str();
}

// int ExpIntFunc (char* func)
long WINAPI DLLExport ExpIntFunc (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	rdPtr->luaMan->state->stack.CallLuaFunction(p1);

	unsigned fpcontrol = fp_precision_double();
	int ret = (int) rdPtr->luaMan->state->stack.GetNumberReturn(1);
	fp_precision_restore(fpcontrol);

	return ret;
}

// char* ExpStrFunc (char* func)
long WINAPI DLLExport ExpStrFunc (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);

	rdPtr->rHo.hoFlags |= HOF_STRING;

	if (rdPtr->luaMan->state == NULL) {
		return (long) "";
	}

	rdPtr->luaMan->state->stack.CallLuaFunction(p1);

	const std::string& ret = rdPtr->luaMan->state->stack.GetStringReturn(1);
	
	
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
}

// int ExpIntFuncNP (char* func)
long WINAPI DLLExport ExpIntFuncNP (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	long p2 = CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_INT);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	rdPtr->luaMan->state->stack.ClearLFParameters();
	rdPtr->luaMan->state->stack.PushParam(p2);
	rdPtr->luaMan->state->stack.CallLuaFunction(p1);

	unsigned fpcontrol = fp_precision_double();
	int ret = (int) rdPtr->luaMan->state->stack.GetNumberReturn(1);
	fp_precision_restore(fpcontrol);

	return ret;
}

// int ExpIntFuncSP (char* func)
long WINAPI DLLExport ExpIntFuncSP (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	char* p2 = (char*) CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	rdPtr->luaMan->state->stack.ClearLFParameters();
	rdPtr->luaMan->state->stack.PushParam(p2);
	rdPtr->luaMan->state->stack.CallLuaFunction(p1);

	unsigned fpcontrol = fp_precision_double();
	int ret = (int) rdPtr->luaMan->state->stack.GetNumberReturn(1);
	fp_precision_restore(fpcontrol);

	return ret;
}

// char* ExpStrFuncNP (char* func)
long WINAPI DLLExport ExpStrFuncNP (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	long p2 = CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_INT);

	rdPtr->rHo.hoFlags |= HOF_STRING;

	if (rdPtr->luaMan->state == NULL) {
		return (long) "";
	}

	rdPtr->luaMan->state->stack.ClearLFParameters();
	rdPtr->luaMan->state->stack.PushParam(p2);
	rdPtr->luaMan->state->stack.CallLuaFunction(p1);

	const std::string& ret = rdPtr->luaMan->state->stack.GetStringReturn(1);
	
	
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
}

// char* ExpStrFuncSP (char* func)
long WINAPI DLLExport ExpStrFuncSP (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	char* p2 = (char*) CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);

	rdPtr->rHo.hoFlags |= HOF_STRING;

	if (rdPtr->luaMan->state == NULL) {
		return (long) "";
	}

	rdPtr->luaMan->state->stack.ClearLFParameters();
	rdPtr->luaMan->state->stack.PushParam(p2);
	rdPtr->luaMan->state->stack.CallLuaFunction(p1);

	const std::string& ret = rdPtr->luaMan->state->stack.GetStringReturn(1);
	
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
}

// float ExpFloatReturn (int index)
long WINAPI DLLExport ExpFloatReturn (LPRDATA rdPtr,long param1) {
	long p1 = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	float ret = (float) rdPtr->luaMan->state->stack.GetNumberReturn(p1);

	rdPtr->rHo.hoFlags |= HOF_FLOAT;
	return *((int*)&ret);
}

// float ExpFloatParam (int index)
long WINAPI DLLExport ExpFloatParam (LPRDATA rdPtr,long param1) {
	long p1 = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	rdPtr->rHo.hoFlags |= HOF_FLOAT;

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	float ret = (float) rdPtr->luaMan->state->stack.GetNumberParam(p1);
	return *((int*)&ret);
}

// float ExpFloatFunc (int index)
long WINAPI DLLExport ExpFloatFunc (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);

	rdPtr->rHo.hoFlags |= HOF_FLOAT;

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	rdPtr->luaMan->state->stack.CallLuaFunction(p1);

	float ret = (float) rdPtr->luaMan->state->stack.GetNumberReturn(1);
	return *((int*)&ret);
}

// float ExpFloatFuncFP (int index)
long WINAPI DLLExport ExpFloatFuncFP (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	long t2 = CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_FLOAT);

	float p2 = *((float*)&t2);

	rdPtr->rHo.hoFlags |= HOF_FLOAT;

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	rdPtr->luaMan->state->stack.ClearLFParameters();
	rdPtr->luaMan->state->stack.PushParam(p2);
	rdPtr->luaMan->state->stack.CallLuaFunction(p1);

	float ret = (float) rdPtr->luaMan->state->stack.GetNumberReturn(1);
	return *((int*)&ret);
}

// char* ExpStrFuncFP (char* func)
long WINAPI DLLExport ExpStrFuncFP (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	long t2 = CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_FLOAT);

	float p2 = *((float*)&t2);

	rdPtr->rHo.hoFlags |= HOF_STRING;

	if (rdPtr->luaMan->state == NULL) {
		return (long) "";
	}

	rdPtr->luaMan->state->stack.ClearLFParameters();
	rdPtr->luaMan->state->stack.PushParam(p2);
	rdPtr->luaMan->state->stack.CallLuaFunction(p1);

	const std::string& ret = rdPtr->luaMan->state->stack.GetStringReturn(1);
	
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
}

// float ExpFloatVar (char* var)
long WINAPI DLLExport ExpFloatVar (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);

	rdPtr->rHo.hoFlags |= HOF_FLOAT;

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	float ret = (float) rdPtr->luaMan->state->GetFloatingVariable(p1);
	return *((int*)&ret);
}

// int ExpTableLength ()
long WINAPI DLLExport ExpTableLength (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	return rdPtr->luaMan->state->GetTableLength(p1);
}

// int ExpTableMaxn ()
long WINAPI DLLExport ExpTableMaxn (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);

	if (rdPtr->luaMan->state == NULL) {
		return 0;
	}

	return rdPtr->luaMan->state->GetTableMaxIndex(p1);
}

// char* ExpEmbeddedDep ()
long WINAPI DLLExport ExpEmbeddedDep (LPRDATA rdPtr,long param1) {
#ifdef XLUA_LEGACY
	const std::string& ret = rdPtr->luaMan->GetEmbeddedScript("Main");
	
	rdPtr->rHo.hoFlags |= HOF_STRING;
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
#else
	return (long) "";
#endif
}

// char* ExpErrMode ()
long WINAPI DLLExport ExpErrMode (LPRDATA rdPtr,long param1) {
	char* str = 0;

	rdPtr->rHo.hoFlags |= HOF_STRING;

	if (rdPtr->luaMan->errMode == 0) {
		return (long) "Immediate";
	}
	else if (rdPtr->luaMan->errMode == 1) {
		return (long) "Queued";
	}

	return (long) "";
}

// char* ExpPrintMode ()
long WINAPI DLLExport ExpPrintMode (LPRDATA rdPtr,long param1) {
	char* str = 0;

	rdPtr->rHo.hoFlags |= HOF_STRING;

	if (rdPtr->luaMan->printMode == 0) {
		return (long) "Immediate";
	}
	else if (rdPtr->luaMan->printMode == 1) {
		return (long) "Queued";
	}

	return (long) "";
}

// char* ExpVarType (char* var)
long WINAPI DLLExport ExpVarType (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);

	rdPtr->rHo.hoFlags |= HOF_STRING;

	if (rdPtr->luaMan->state == NULL) {
		return (long) "";
	}

	const std::string& ret = rdPtr->luaMan->state->GetVariableType(p1);
	
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
}

// char* ExpEmbeddedScript (char* var)
long WINAPI DLLExport ExpEmbeddedScript (LPRDATA rdPtr,long param1) {
	char* p1 = (char*) CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);

	const std::string& ret = rdPtr->luaMan->GetEmbeddedScript(p1);
	
	rdPtr->rHo.hoFlags |= HOF_STRING;
	char* str = (char*) callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, 0, ret.size() + 1);
	memcpy(str, ret.c_str(), ret.size() + 1);
	return (long) str;
}

// ----------------------------------------------------------
// Expression jump table
// ----------------------------------------------------------
// Contains the address inside the extension of the different
// routines that handle the action, conditions and expressions.
// Located at the end of the source for convinience
// Must finish with a 0
//
long (WINAPI * ExpressionJumps[])(LPRDATA rdPtr, long param) = {
	ExpErrorStr,
	ExpIntReturn,
	ExpStrReturn,
	ExpBoolReturn,
	ExpReturnCount,
	ExpIntParam,
	ExpStrParam,
	ExpBoolParam,
	ExpParamCount,
	ExpIntVar,
	ExpStrVar,
	ExpBoolVar,
	ExpLocalVal,
	ExpLocalStr,
	ExpPrintStr,
	ExpIntFunc,
	ExpStrFunc,
	ExpIntFuncNP,
	ExpIntFuncSP,
	ExpStrFuncNP,
	ExpStrFuncSP,
	ExpFloatReturn,
	ExpFloatParam,
	ExpFloatFunc,
	ExpFloatFuncFP,
	ExpStrFuncFP,
	ExpFloatVar,
	ExpTableLength,
	ExpTableMaxn,
	ExpEmbeddedDep,
	ExpErrMode,
	ExpPrintMode,
	ExpVarType,
	ExpEmbeddedScript,
	0
};
