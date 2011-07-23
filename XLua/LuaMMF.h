#ifndef LUAMMF_H_
#define LUAMMF_H_

#include	<vector>
#include	<string>

#include	<windows.h>

#include	"Lua/lua.hpp"

class XLuaState;

typedef struct tagRO RunObject;
typedef RunObject* LPRO;

class LuaMMF {
protected:

	XLuaState*	_state;

public:

	static const int MF_ANY_LAYER = -1;
	static const int MF_SAME_LAYER = -2;
	static const int MF_DIFF_LAYER = -3;

	static const int MF_ANY_CLASS = -1;

	LuaMMF ()
		: _state(NULL) {
	}

	void SetState (XLuaState* L) {
		_state = L;
	}

	void SetEnumInt (const char* name, int val);
	void SetEnumFloat (const char* name, double val);
	void SetEnumBool (const char* name, int val);

	void Init ();
	void Register ();
	void RegisterWin ();

	// Windows Interface
	static int WinExportMessage (lua_State *L);

	void MessageCallback (UINT msg, WPARAM wParam, LPARAM lParam);

	// Local Store
	static int LocalStoreGetValue (lua_State *L);
	static int LocalStoreGetString (lua_State *L);
	static int LocalStoreSetValue (lua_State *L);
	static int LocalStoreSetString (lua_State *L);

	// General Management
	static int ObjectUpdate (lua_State *L);

	// General Info
	static int ObjectGetFixed (lua_State *L);
	static int ObjectGetName (lua_State *L);
	static int ObjectGetIdentifier (lua_State *L);
	static int ObjectGetClass (lua_State *L);

	static int ObjectDestroy (lua_State *L);
	static int ObjectExport (lua_State *L);

	static int GetCurrentEvent (lua_State *L);

	// Positional access
	static int ObjectGetX (lua_State *L);
	static int ObjectGetY (lua_State *L);
	static int ObjectGetPosition (lua_State *L);

	static int ObjectGetXLeft (lua_State *L);
	static int ObjectGetXRight (lua_State *L);
	static int ObjectGetYTop (lua_State *L);
	static int ObjectGetYBottom (lua_State *L);

	static int ObjectSetX (lua_State *L);
	static int ObjectSetY (lua_State *L);
	static int ObjectSetPosition (lua_State *L);

	static int ObjectGetWidth (lua_State *L);
	static int ObjectGetHeight (lua_State *L);

	static int ObjectGetLayer (lua_State *L);
	static int ObjectSetLayer (lua_State *L);

	static int ObjectGetZOrder (lua_State *L);
	static int ObjectMoveToFront (lua_State *L);
	static int ObjectMoveToBack (lua_State *L);
	static int ObjectMoveAbove (lua_State *L);
	static int ObjectMoveBelow (lua_State *L);
	static int ObjectIsAbove (lua_State *L);
	static int ObjectIsBelow (lua_State *L);
	static int ObjectSwapOrder (lua_State *L);

	// Hotspot access
	static int ObjectGetSpotX (lua_State *L);
	static int ObjectGetSpotY (lua_State *L);
	static int ObjectGetHotspot (lua_State *L);

	static int ObjectGetActionX (lua_State *L);
	static int ObjectGetActionY (lua_State *L);
	static int ObjectGetActionPoint (lua_State *L);

	// Animation & Direction access
	static int ObjectGetDirection (lua_State *L);
	static int ObjectSetDirection (lua_State *L);

	static int ObjectGetAnimation (lua_State *L);
	static int ObjectGetAnimationFrame (lua_State *L);
	static int ObjectSetAnimation (lua_State *L);
	static int ObjectSetAnimationFrame (lua_State *L);

	// Scale & Rotation
	static int ObjectGetAngle (lua_State *L);
	static int ObjectSetAngle (lua_State *L);

	static int ObjectGetScaleX (lua_State *L);
	static int ObjectGetScaleY (lua_State *L);
	static int ObjectGetScale (lua_State *L);

	static int ObjectSetScaleX (lua_State *L);
	static int ObjectSetScaleY (lua_State *L);
	static int ObjectSetScale (lua_State *L);

	// Visibility & Display
	static int ObjectGetVisibility (lua_State *L);
	static int ObjectSetVisibility (lua_State *L);

	// Alterable Values
	static int ObjectGetValue (lua_State *L);
	static int ObjectGetString (lua_State *L);
	static int ObjectGetFlag (lua_State *L);

	static int ObjectSetValue (lua_State *L);
	static int ObjectSetString (lua_State *L);
	static int ObjectSetFlag (lua_State *L);

	// Collision Testing (Experimental)
	static int BackdropTestPoint (lua_State *L);
	static int BackdropTestRect (lua_State *L);
	static int BackdropTestObject (lua_State *L);

	static int ObjectTestPoint (lua_State *L);
	static int ObjectTestRect (lua_State *L);
	static int ObjectTestObject (lua_State *L);

	static int ObjectClassTestPoint (lua_State *L);
	static int ObjectClassTestRect (lua_State *L);
	static int ObjectClassTestObject (lua_State *L);

	static int GetObjectsOverPoint (lua_State *L);
	static int GetObjectsOverRect (lua_State *L);
	static int GetObjectsOverObject (lua_State *L);

	/*static int TestObjectOverlap (lua_State *L);
	static int TestObjectClassOverlap (lua_State *L);
	static int GetObjectOverlap (lua_State *L);*/

	// Global Values
	static int GetGlobalValue (lua_State *L);
	static int GetGlobalString (lua_State *L);
	static int SetGlobalValue (lua_State *L);
	static int SetGlobalString (lua_State *L);

	// Frame
	static int FrameGetXLeft (lua_State *L);
	static int FrameGetXRight (lua_State *L);
	static int FrameGetYTop (lua_State *L);
	static int FrameGetYBottom (lua_State *L);
	static int FrameGetWidth (lua_State *L);
	static int FrameGetHeight (lua_State *L);
	static int FrameGetVWidth (lua_State *L);
	static int FrameGetVHeight (lua_State *L);

	// Window
	static int WindowGetWidth (lua_State *L);
	static int WindowGetHeight (lua_State *L);
	static int WindowGetClientWidth (lua_State *L);
	static int WindowGetClientHeight (lua_State *L);
	static int WindowGetFrameWidth (lua_State *L);
	static int WindowGetFrameHeight (lua_State *L);
	static int WindowSetFrameWidth (lua_State *L);
	static int WindowSetFrameHeight (lua_State *L);
	static int WindowGetMainHandle (lua_State *L);
	static int WindowGetEditHandle (lua_State *L);

	// Mouse
	static int MouseGetX (lua_State *L);
	static int MouseGetY (lua_State *L);
	static int MouseGetWindowX (lua_State *L);
	static int MouseGetWindowY (lua_State *L);
	static int MouseIsButtonDown (lua_State *L);
	static int MouseIsButtonUp (lua_State *L);

	// Keyboard
	static int KeyboardIsKeyDown (lua_State *L);
	static int KeyboardIsKeyUp (lua_State *L);

protected:

	struct LuaMemFile {
		const char* text;
		unsigned size;
	};

	static const struct luaL_Reg mmf_funcs[];

	//static const char* ReadMemFile (lua_State*, void* ud, unsigned* size);

	static LPRO ObjectLookup (lua_State* L, int params);
	static bool ParamCheck (lua_State* L, int params);
};

#endif