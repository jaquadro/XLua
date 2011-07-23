// Include guard
#ifndef COMMON_H
#define COMMON_H

#define EF_ISHWA 112

// Suppress the deprecated warnings for VC2005
// #define _CRT_SECURE_NO_WARNINGS

#include <float.h>

#pragma fenv_access (on)

// General includes
#include	"ccxhdr.h"

// Specific to this extension
#include	"XLuaGlobal.h"
#include	"XLuaState.h"
#include	"XLuaObject.h"

#include	"Resource.h"
#include	"Main.h"

// Globals and prototypes
extern HINSTANCE hInstLib;
extern short conditionsInfos[];
extern short actionsInfos[];
extern short expressionsInfos[];

extern long (WINAPI * ConditionJumps[])(LPRDATA rdPtr, long param1, long param2);
extern short (WINAPI * ActionJumps[])(LPRDATA rdPtr, long param1, long param2);
extern long (WINAPI * ExpressionJumps[])(LPRDATA rdPtr, long param);

extern PropData Properties[];
extern WORD DebugTree[];

// DirectX mode sets fp precision to single, but Lua needs double
// precision.

inline unsigned fp_current_state () {
	unsigned control_word;
	__control87_2(0, 0, &control_word, 0);

	return control_word;
}

inline unsigned fp_precision_double () {
	unsigned control_word, control_ret;
	__control87_2(0, 0, &control_word, 0);
	__control87_2(_PC_53, MCW_PC, &control_ret, 0);

	return control_word;
}

inline void fp_precision_restore (unsigned control_word) {
	unsigned control_ret;
	__control87_2(control_word, MCW_PC, &control_ret, 0);
}

// Used to ensure the MMF version is 1.5, you can safely ignore this
#if defined(MMFEXT)
#define	IS_COMPATIBLE(v) (v->mvGetVersion != NULL && (v->mvGetVersion() & MMFBUILD_MASK) >= MINBUILD && (v->mvGetVersion() & MMFVERSION_MASK) >= MMFVERSION_20 && ((v->mvGetVersion() & MMFVERFLAG_MASK) & MMFVERFLAG_HOME) == 0)
#elif defined(PROEXT)
#define IS_COMPATIBLE(v) (v->mvGetVersion != NULL && (v->mvGetVersion() & MMFBUILD_MASK) >= MINBUILD && (v->mvGetVersion() & MMFVERSION_MASK) >= MMFVERSION_20 && ((v->mvGetVersion() & MMFVERFLAG_MASK) & MMFVERFLAG_PRO) != 0)
#else
#define	IS_COMPATIBLE(v) (v->mvGetVersion != NULL && (v->mvGetVersion() & MMFBUILD_MASK) >= MINBUILD && (v->mvGetVersion() & MMFVERSION_MASK) >= MMFVERSION_20)
#endif

// End include guard
#endif
