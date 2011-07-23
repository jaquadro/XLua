#include "Common.h"
#include "Surface.h"

//#include <windows.h>

#include <cassert>

void LuaMMF::Init () {
	//Register();
}

void LuaMMF::RegisterWin () {

	// Create win package
	lua_newtable(_state->state);
	lua_setglobal(_state->state, "win");
	lua_getglobal(_state->state, "win");

	// Push export function
	lua_pushcfunction(_state->state, LuaMMF::WinExportMessage);
	lua_setfield(_state->state, -2, "ExportMessage");

	XLuaGlobal::Get()._winiStates.push_back(_state);
	_state->winiReady = true;
}

int LuaMMF::WinExportMessage (lua_State *L) {
	if (!ParamCheck(L, 2)) return 0;

	lua_settop(L, 2);
	unsigned message = lua_tointeger(L, 1);
	
	if (lua_type(L, 2) != LUA_TFUNCTION) {
		lua_pushstring(L, "Invalid callback function");
		lua_error(L);
	}

	unsigned ref = luaL_ref(L, LUA_REGISTRYINDEX);

	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state != NULL) {
		state->winExports[message] = ref;
	}

	return 0;
}

void LuaMMF::MessageCallback (UINT msg, WPARAM wParam, LPARAM lParam) {
	XLuaState::IWinExports iter = _state->winExports.find(msg);
	if (iter == _state->winExports.end()) {
		return;
	}

	int top = lua_gettop(_state->state);

	lua_rawgeti(_state->state, LUA_REGISTRYINDEX, iter->second);
	lua_pushnumber(_state->state, wParam);
	lua_pushnumber(_state->state, lParam);

	if (lua_pcall(_state->state, 2, 0, 0)) {
		_state->RaiseError(lua_tostring(_state->state, -1));
	}

	lua_settop(_state->state, top);
}

void LuaMMF::SetEnumInt (const char* name, int val) {
	lua_pushinteger(_state->state, val);
	lua_setfield(_state->state, -2, name);
}

void LuaMMF::SetEnumFloat (const char* name, double val) {
	lua_pushnumber(_state->state, val);
	lua_setfield(_state->state, -2, name);
}

void LuaMMF::SetEnumBool (const char* name, int val) {
	lua_pushboolean(_state->state, val);
	lua_setfield(_state->state, -2, name);
}


void LuaMMF::Register () {
	//luaL_register(_state, "mmf", mmf_funcs);

	// Create mmf package
	lua_newtable(_state->state);
	lua_setglobal(_state->state, "mmf");
	lua_getglobal(_state->state, "mmf");

	// Enumeration Values
	SetEnumInt("ALT_A", 0);
	SetEnumInt("ALT_B", 1);
	SetEnumInt("ALT_C", 2);
	SetEnumInt("ALT_D", 3);
	SetEnumInt("ALT_E", 4);
	SetEnumInt("ALT_F", 5);
	SetEnumInt("ALT_G", 6);
	SetEnumInt("ALT_H", 7);
	SetEnumInt("ALT_I", 8);
	SetEnumInt("ALT_J", 9);
	SetEnumInt("ALT_K", 10);
	SetEnumInt("ALT_L", 11);
	SetEnumInt("ALT_M", 12);
	SetEnumInt("ALT_N", 13);
	SetEnumInt("ALT_O", 14);
	SetEnumInt("ALT_P", 15);
	SetEnumInt("ALT_Q", 16);
	SetEnumInt("ALT_R", 17);
	SetEnumInt("ALT_S", 18);
	SetEnumInt("ALT_T", 19);
	SetEnumInt("ALT_U", 20);
	SetEnumInt("ALT_V", 21);
	SetEnumInt("ALT_W", 22);
	SetEnumInt("ALT_X", 23);
	SetEnumInt("ALT_Y", 24);
	SetEnumInt("ALT_Z", 25);
	SetEnumInt("ANY_CLASS", MF_ANY_CLASS);
	SetEnumInt("ANY_LAYER", MF_ANY_LAYER);
	SetEnumInt("MOUSE_LEFT", 1);
	SetEnumInt("MOUSE_MIDDLE", 2);
	SetEnumInt("MOUSE_RIGHT", 3);
	SetEnumInt("MOUSE_X1", 4);
	SetEnumInt("MOUSE_X2", 5);
	SetEnumInt("SAME_LAYER", MF_SAME_LAYER);
	SetEnumInt("DIFF_LAYER", MF_DIFF_LAYER);

	SetEnumInt("VK_BACK", 0x08);
	SetEnumInt("VK_TAB", 0x09);
	SetEnumInt("VK_RETURN", 0x0D);
	SetEnumInt("VK_SHIFT", 0x10);
	SetEnumInt("VK_CONTROL", 0x11);
	SetEnumInt("VK_ALT", 0x12);
	SetEnumInt("VK_MENU", 0x12);
	SetEnumInt("VK_PAUSE", 0x13);
	SetEnumInt("VK_CAPITAL", 0x14);
	SetEnumInt("VK_ESCAPE", 0x1B);
	SetEnumInt("VK_SPACE", 0x20);
	SetEnumInt("VK_PRIOR", 0x21);
	SetEnumInt("VK_PAGEUP", 0x21);
	SetEnumInt("VK_NEXT", 0x22);
	SetEnumInt("VK_PAGEDOWN", 0x22);
	SetEnumInt("VK_END", 0x23);
	SetEnumInt("VK_HOME", 0x24);
	SetEnumInt("VK_LEFT", 0x25);
	SetEnumInt("VK_UP", 0x26);
	SetEnumInt("VK_RIGHT", 0x27);
	SetEnumInt("VK_DOWN", 0x28);
	SetEnumInt("VK_SNAPSHOT", 0x2C);
	SetEnumInt("VK_PRINTSCREEN", 0x2C);
	SetEnumInt("VK_INSERT", 0x2D);
	SetEnumInt("VK_DELETE", 0x2E);
	SetEnumInt("VK_0", 0x30);
	SetEnumInt("VK_1", 0x31);
	SetEnumInt("VK_2", 0x32);
	SetEnumInt("VK_3", 0x33);
	SetEnumInt("VK_4", 0x34);
	SetEnumInt("VK_5", 0x35);
	SetEnumInt("VK_6", 0x36);
	SetEnumInt("VK_7", 0x37);
	SetEnumInt("VK_8", 0x38);
	SetEnumInt("VK_9", 0x39);
	SetEnumInt("VK_A", 0x41);
	SetEnumInt("VK_B", 0x42);
	SetEnumInt("VK_C", 0x43);
	SetEnumInt("VK_D", 0x44);
	SetEnumInt("VK_E", 0x45);
	SetEnumInt("VK_F", 0x46);
	SetEnumInt("VK_G", 0x47);
	SetEnumInt("VK_H", 0x48);
	SetEnumInt("VK_I", 0x49);
	SetEnumInt("VK_J", 0x4A);
	SetEnumInt("VK_K", 0x4B);
	SetEnumInt("VK_L", 0x4C);
	SetEnumInt("VK_M", 0x4D);
	SetEnumInt("VK_N", 0x4E);
	SetEnumInt("VK_O", 0x4F);
	SetEnumInt("VK_P", 0x50);
	SetEnumInt("VK_Q", 0x51);
	SetEnumInt("VK_R", 0x52);
	SetEnumInt("VK_S", 0x53);
	SetEnumInt("VK_T", 0x54);
	SetEnumInt("VK_U", 0x55);
	SetEnumInt("VK_V", 0x56);
	SetEnumInt("VK_W", 0x57);
	SetEnumInt("VK_X", 0x58);
	SetEnumInt("VK_Y", 0x59);
	SetEnumInt("VK_Z", 0x60);
	SetEnumInt("VK_LWIN", 0x5B);
	SetEnumInt("VK_RWIN", 0x5C);
	SetEnumInt("VK_NUMPAD0", 0x60);
	SetEnumInt("VK_NUMPAD1", 0x61);
	SetEnumInt("VK_NUMPAD2", 0x62);
	SetEnumInt("VK_NUMPAD3", 0x63);
	SetEnumInt("VK_NUMPAD4", 0x64);
	SetEnumInt("VK_NUMPAD5", 0x65);
	SetEnumInt("VK_NUMPAD6", 0x66);
	SetEnumInt("VK_NUMPAD7", 0x67);
	SetEnumInt("VK_NUMPAD8", 0x68);
	SetEnumInt("VK_NUMPAD9", 0x69);
	SetEnumInt("VK_MULTIPLY", 0x6A);
	SetEnumInt("VK_ADD", 0x6B);
	SetEnumInt("VK_SUBTRACT", 0x6D);
	SetEnumInt("VK_DECIMAL", 0x6E);
	SetEnumInt("VK_DIVIDE", 0x6F);
	SetEnumInt("VK_F1", 0x70);
	SetEnumInt("VK_F2", 0x71);
	SetEnumInt("VK_F3", 0x72);
	SetEnumInt("VK_F4", 0x73);
	SetEnumInt("VK_F5", 0x74);
	SetEnumInt("VK_F6", 0x75);
	SetEnumInt("VK_F7", 0x76);
	SetEnumInt("VK_F8", 0x77);
	SetEnumInt("VK_F9", 0x78);
	SetEnumInt("VK_F10", 0x79);
	SetEnumInt("VK_F11", 0x7A);
	SetEnumInt("VK_F12", 0x7B);
	SetEnumInt("VK_NUMLOCK", 0x90);
	SetEnumInt("VK_SCROLL", 0x91);
	SetEnumInt("VK_LSHIFT", 0xA0);
	SetEnumInt("VK_RSHIFT", 0xA1);
	SetEnumInt("VK_LCONTROL", 0xA2);
	SetEnumInt("VK_RCONTROL", 0xA3);
	SetEnumInt("VK_LMENU", 0xA4);
	SetEnumInt("VK_LALT", 0xA4);
	SetEnumInt("VK_RMENU", 0xA5);
	SetEnumInt("VK_RALT", 0xA5);
	
	/*lua_pushinteger(_state->state, MF_ANY_CLASS);
	lua_setfield(_state->state, -2, "ANY_CLASS");
	lua_pushinteger(_state->state, MF_SAME_LAYER);
	lua_setfield(_state->state, -2, "SAME_LAYER");
	lua_pushinteger(_state->state, MF_ANY_LAYER);
	lua_setfield(_state->state, -2, "ANY_LAYER");*/

	// Create Global table for global value functions
	lua_newtable(_state->state);
	lua_setfield(_state->state, -2, "Global");
	lua_getfield(_state->state, -1, "Global");

	// Push global value functions
	lua_pushcfunction(_state->state, LuaMMF::GetGlobalValue);
	lua_setfield(_state->state, -2, "GetValue");
	lua_pushcfunction(_state->state, LuaMMF::GetGlobalString);
	lua_setfield(_state->state, -2, "GetString");
	lua_pushcfunction(_state->state, LuaMMF::SetGlobalValue);
	lua_setfield(_state->state, -2, "SetValue");
	lua_pushcfunction(_state->state, LuaMMF::SetGlobalString);
	lua_setfield(_state->state, -2, "SetString");

	// Create Local Store table for local store functions
	lua_pop(_state->state, 1);
	lua_newtable(_state->state);
	lua_setfield(_state->state, -2, "LocalStore");
	lua_getfield(_state->state, -1, "LocalStore");

	// Push local storage functions
	lua_pushcfunction(_state->state, LuaMMF::LocalStoreGetValue);
	lua_setfield(_state->state, -2, "GetValue");
	lua_pushcfunction(_state->state, LuaMMF::LocalStoreGetString);
	lua_setfield(_state->state, -2, "GetString");
	lua_pushcfunction(_state->state, LuaMMF::LocalStoreSetValue);
	lua_setfield(_state->state, -2, "SetValue");
	lua_pushcfunction(_state->state, LuaMMF::LocalStoreSetString);
	lua_setfield(_state->state, -2, "SetString");

	// Create CollisionMask table for collision mask functions
	lua_pop(_state->state, 1);
	lua_newtable(_state->state);
	lua_setfield(_state->state, -2, "CollisionMask"); // deprecated
	lua_getfield(_state->state, -1, "CollisionMask");

	// Push collision mask functions
	lua_pushcfunction(_state->state, LuaMMF::BackdropTestPoint);
	lua_setfield(_state->state, -2, "TestPoint"); // deprecated
	lua_pushcfunction(_state->state, LuaMMF::BackdropTestRect);
	lua_setfield(_state->state, -2, "TestRect"); // deprecated
	lua_pushcfunction(_state->state, LuaMMF::BackdropTestObject);
	lua_setfield(_state->state, -2, "TestSprite"); // deprecated

	lua_pop(_state->state, 1);
	lua_newtable(_state->state);
	lua_setfield(_state->state, -2, "Collisions");
	lua_getfield(_state->state, -1, "Collisions");

	// Push collision functions
	lua_pushcfunction(_state->state, LuaMMF::BackdropTestPoint);
	lua_setfield(_state->state, -2, "BackdropTestPoint");
	lua_pushcfunction(_state->state, LuaMMF::BackdropTestRect);
	lua_setfield(_state->state, -2, "BackdropTestRect");
	lua_pushcfunction(_state->state, LuaMMF::BackdropTestObject);
	lua_setfield(_state->state, -2, "BackdropTestObject");

	lua_pushcfunction(_state->state, LuaMMF::ObjectTestPoint);
	lua_setfield(_state->state, -2, "ObjectTestPoint");
	lua_pushcfunction(_state->state, LuaMMF::ObjectTestRect);
	lua_setfield(_state->state, -2, "ObjectTestRect");
	lua_pushcfunction(_state->state, LuaMMF::ObjectTestObject);
	lua_setfield(_state->state, -2, "ObjectTestObject");

	lua_pushcfunction(_state->state, LuaMMF::ObjectClassTestPoint);
	lua_setfield(_state->state, -2, "ObjectClassTestPoint");
	lua_pushcfunction(_state->state, LuaMMF::ObjectClassTestRect);
	lua_setfield(_state->state, -2, "ObjectClassTestRect");
	lua_pushcfunction(_state->state, LuaMMF::ObjectClassTestObject);
	lua_setfield(_state->state, -2, "ObjectClassTestObject");

	lua_pushcfunction(_state->state, LuaMMF::GetObjectsOverPoint);
	lua_setfield(_state->state, -2, "GetObjectsOverPoint");
	lua_pushcfunction(_state->state, LuaMMF::GetObjectsOverRect);
	lua_setfield(_state->state, -2, "GetObjectsOverRect");
	lua_pushcfunction(_state->state, LuaMMF::GetObjectsOverObject);
	lua_setfield(_state->state, -2, "GetObjectsOverObject");

	// Create Object table for object functions
	lua_pop(_state->state, 1);
	lua_newtable(_state->state);
	lua_setfield(_state->state, -2, "Object");
	lua_getfield(_state->state, -1, "Object");

	// Object collision / overlap functions
	lua_pushcfunction(_state->state, LuaMMF::BackdropTestObject);
	lua_setfield(_state->state, -2, "OverlapsBackground"); // deprecated
	lua_pushcfunction(_state->state, LuaMMF::ObjectTestObject);
	lua_setfield(_state->state, -2, "OverlapsObject"); // deprecated
	lua_pushcfunction(_state->state, LuaMMF::ObjectClassTestObject);
	lua_setfield(_state->state, -2, "OverlapsObjectClass"); // deprecated
	lua_pushcfunction(_state->state, LuaMMF::GetObjectsOverObject);
	lua_setfield(_state->state, -2, "GetOverlapList"); // deprecated

	// Push general object management functions
	lua_pushcfunction(_state->state, LuaMMF::ObjectUpdate);
	lua_setfield(_state->state, -2, "Update");
	lua_pushcfunction(_state->state, LuaMMF::ObjectDestroy);
	lua_setfield(_state->state, -2, "Destroy");

	// Push general object info functions
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetFixed);
	lua_setfield(_state->state, -2, "GetFixed");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetName);
	lua_setfield(_state->state, -2, "GetName");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetIdentifier);
	lua_setfield(_state->state, -2, "GetIdentifier");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetClass);
	lua_setfield(_state->state, -2, "GetClass");
	lua_pushcfunction(_state->state, LuaMMF::ObjectExport);
	lua_setfield(_state->state, -2, "Export");

	// Push positional access functions
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetX);
	lua_setfield(_state->state, -2, "GetX");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetY);
	lua_setfield(_state->state, -2, "GetY");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetPosition);
	lua_setfield(_state->state, -2, "GetPosition");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetXLeft);
	lua_setfield(_state->state, -2, "GetXLeft");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetXRight);
	lua_setfield(_state->state, -2, "GetXRight");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetYTop);
	lua_setfield(_state->state, -2, "GetYTop");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetYBottom);
	lua_setfield(_state->state, -2, "GetYBottom");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetX);
	lua_setfield(_state->state, -2, "SetX");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetY);
	lua_setfield(_state->state, -2, "SetY");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetPosition);
	lua_setfield(_state->state, -2, "SetPosition");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetWidth);
	lua_setfield(_state->state, -2, "GetWidth");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetHeight);
	lua_setfield(_state->state, -2, "GetHeight");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetLayer);
	lua_setfield(_state->state, -2, "GetLayer");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetLayer);
	lua_setfield(_state->state, -2, "SetLayer");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetZOrder);
	lua_setfield(_state->state, -2, "GetZOrder");
	lua_pushcfunction(_state->state, LuaMMF::ObjectMoveToFront);
	lua_setfield(_state->state, -2, "MoveToFront");
	lua_pushcfunction(_state->state, LuaMMF::ObjectMoveToBack);
	lua_setfield(_state->state, -2, "MoveToBack");
	lua_pushcfunction(_state->state, LuaMMF::ObjectMoveAbove);
	lua_setfield(_state->state, -2, "MoveAbove");
	lua_pushcfunction(_state->state, LuaMMF::ObjectMoveBelow);
	lua_setfield(_state->state, -2, "MoveBelow");
	lua_pushcfunction(_state->state, LuaMMF::ObjectIsAbove);
	lua_setfield(_state->state, -2, "IsAbove");
	lua_pushcfunction(_state->state, LuaMMF::ObjectIsBelow);
	lua_setfield(_state->state, -2, "IsBelow");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSwapOrder);
	lua_setfield(_state->state, -2, "SwapOrder");

	// Push Hotspot access functions
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetSpotX);
	lua_setfield(_state->state, -2, "GetHotspotX");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetSpotY);
	lua_setfield(_state->state, -2, "GetHotspotY");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetHotspot);
	lua_setfield(_state->state, -2, "GetHotspot");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetActionX);
	lua_setfield(_state->state, -2, "GetActionX");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetActionY);
	lua_setfield(_state->state, -2, "GetActionY");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetActionPoint);
	lua_setfield(_state->state, -2, "GetActionPoint");

	// Push Animation & Direction functions
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetDirection);
	lua_setfield(_state->state, -2, "GetDirection");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetDirection);
	lua_setfield(_state->state, -2, "SetDirection");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetAnimation);
	lua_setfield(_state->state, -2, "GetAnimation");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetAnimationFrame);
	lua_setfield(_state->state, -2, "GetAnimationFrame");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetAnimation);
	lua_setfield(_state->state, -2, "SetAnimation");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetAnimationFrame);
	lua_setfield(_state->state, -2, "SetAnimationFrame");

	// Push Scale & Rotation functions
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetAngle);
	lua_setfield(_state->state, -2, "GetAngle");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetAngle);
	lua_setfield(_state->state, -2, "SetAngle");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetScaleX);
	lua_setfield(_state->state, -2, "GetScaleX");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetScaleY);
	lua_setfield(_state->state, -2, "GetScaleY");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetScale);
	lua_setfield(_state->state, -2, "GetScale");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetScaleX);
	lua_setfield(_state->state, -2, "SetScaleX");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetScaleY);
	lua_setfield(_state->state, -2, "SetScaleY");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetScale);
	lua_setfield(_state->state, -2, "SetScale");

	// Push Visibility & Display functions
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetVisibility);
	lua_setfield(_state->state, -2, "GetVisibility");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetVisibility);
	lua_setfield(_state->state, -2, "SetVisibility");	

	// Push Alterable Value access functions
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetValue);
	lua_setfield(_state->state, -2, "GetValue");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetString);
	lua_setfield(_state->state, -2, "GetString");
	lua_pushcfunction(_state->state, LuaMMF::ObjectGetFlag);
	lua_setfield(_state->state, -2, "GetFlag");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetValue);
	lua_setfield(_state->state, -2, "SetValue");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetString);
	lua_setfield(_state->state, -2, "SetString");
	lua_pushcfunction(_state->state, LuaMMF::ObjectSetFlag);
	lua_setfield(_state->state, -2, "SetFlag");

	// Create Frame table for frame functions
	lua_pop(_state->state, 1);
	lua_newtable(_state->state);
	lua_setfield(_state->state, -2, "Frame");
	lua_getfield(_state->state, -1, "Frame");

	// Frame
	lua_pushcfunction(_state->state, LuaMMF::FrameGetXLeft);
	lua_setfield(_state->state, -2, "GetXLeft");
	lua_pushcfunction(_state->state, LuaMMF::FrameGetXRight);
	lua_setfield(_state->state, -2, "GetXRight");
	lua_pushcfunction(_state->state, LuaMMF::FrameGetYTop);
	lua_setfield(_state->state, -2, "GetYTop");
	lua_pushcfunction(_state->state, LuaMMF::FrameGetYBottom);
	lua_setfield(_state->state, -2, "GetYBottom");
	lua_pushcfunction(_state->state, LuaMMF::FrameGetWidth);
	lua_setfield(_state->state, -2, "GetWidth");
	lua_pushcfunction(_state->state, LuaMMF::FrameGetHeight);
	lua_setfield(_state->state, -2, "GetHeight");
	lua_pushcfunction(_state->state, LuaMMF::FrameGetVWidth);
	lua_setfield(_state->state, -2, "GetVWidth");
	lua_pushcfunction(_state->state, LuaMMF::FrameGetVHeight);
	lua_setfield(_state->state, -2, "GetVHeight");

	lua_pushcfunction(_state->state, LuaMMF::GetCurrentEvent);
	lua_setfield(_state->state, -2, "GetCurrentEvent");

	// Create Window table for window functions
	lua_pop(_state->state, 1);
	lua_newtable(_state->state);
	lua_setfield(_state->state, -2, "Window");
	lua_getfield(_state->state, -1, "Window");

	// Window
	lua_pushcfunction(_state->state, LuaMMF::WindowGetWidth);
	lua_setfield(_state->state, -2, "GetWidth");
	lua_pushcfunction(_state->state, LuaMMF::WindowGetHeight);
	lua_setfield(_state->state, -2, "GetHeight");
	lua_pushcfunction(_state->state, LuaMMF::WindowGetClientWidth);
	lua_setfield(_state->state, -2, "GetCWidth");
	lua_pushcfunction(_state->state, LuaMMF::WindowGetClientHeight);
	lua_setfield(_state->state, -2, "GetCHeight");
	lua_pushcfunction(_state->state, LuaMMF::WindowGetFrameWidth);
	lua_setfield(_state->state, -2, "GetFrameWidth");
	lua_pushcfunction(_state->state, LuaMMF::WindowGetFrameHeight);
	lua_setfield(_state->state, -2, "GetFrameHeight");
	lua_pushcfunction(_state->state, LuaMMF::WindowGetMainHandle);
	lua_setfield(_state->state, -2, "GetMainHandle");
	lua_pushcfunction(_state->state, LuaMMF::WindowGetEditHandle);
	lua_setfield(_state->state, -2, "GetEditHandle");

	// Create Mouse table for mouse functions
	lua_pop(_state->state, 1);
	lua_newtable(_state->state);
	lua_setfield(_state->state, -2, "Mouse");
	lua_getfield(_state->state, -1, "Mouse");

	// Mouse
	lua_pushcfunction(_state->state, LuaMMF::MouseGetX);
	lua_setfield(_state->state, -2, "GetX");
	lua_pushcfunction(_state->state, LuaMMF::MouseGetY);
	lua_setfield(_state->state, -2, "GetY");
	lua_pushcfunction(_state->state, LuaMMF::MouseGetWindowX);
	lua_setfield(_state->state, -2, "GetClientX");
	lua_pushcfunction(_state->state, LuaMMF::MouseGetWindowY);
	lua_setfield(_state->state, -2, "GetClientY");
	lua_pushcfunction(_state->state, LuaMMF::MouseIsButtonDown);
	lua_setfield(_state->state, -2, "ButtonDown");
	lua_pushcfunction(_state->state, LuaMMF::MouseIsButtonUp);
	lua_setfield(_state->state, -2, "ButtonUp");

	// Create Keyboard table for keyboard functions
	lua_pop(_state->state, 1);
	lua_newtable(_state->state);
	lua_setfield(_state->state, -2, "Keyboard");
	lua_getfield(_state->state, -1, "Keyboard");

	// Keyboard
	lua_pushcfunction(_state->state, LuaMMF::KeyboardIsKeyDown);
	lua_setfield(_state->state, -2, "KeyDown");
	lua_pushcfunction(_state->state, LuaMMF::KeyboardIsKeyUp);
	lua_setfield(_state->state, -2, "KeyUp");

	_state->mmfiReady = true;
}

LPHO GetHO (LPRO obj) {
	if (!obj) return 0;

	return (LPHO)obj;
}

tagRCOM* GetCOM (LPRO obj) {
	if (!obj) return 0;

	LPHO ho = GetHO(obj);
	DWORD flags = ho->hoOEFlags;

	if (flags & OEFLAG_MOVEMENTS || flags & OEFLAG_SPRITES || flags & OEFLAG_ANIMATIONS) {
		return (tagRCOM*)((char*)obj + sizeof(headerObject));
	}

	return 0;
}

tagRM* GetRM (LPRO obj) {
	if (!obj) return 0;

	LPHO ho = GetHO(obj);
	DWORD flags = ho->hoOEFlags;
	
	if (flags & OEFLAG_MOVEMENTS) {
		return (tagRM*)((char*)obj + sizeof(headerObject) + sizeof(tagRCOM));
	}

	return 0;
}

tagRA* GetRA (LPRO obj) {
	if (!obj) return 0;

	LPHO ho = GetHO(obj);
	DWORD flags = ho->hoOEFlags;
	
	if (flags & OEFLAG_MOVEMENTS && flags & OEFLAG_ANIMATIONS) {
		return (tagRA*)((char*)obj + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRM));
	}
	else if (flags & OEFLAG_ANIMATIONS) {
		return (tagRA*)((char*)obj + sizeof(headerObject) + sizeof(tagRCOM));
	}

	return 0;
}

tagRSPR* GetRSPR (LPRO obj) {
	if (!obj) return 0;

	LPHO ho = GetHO(obj);
	DWORD flags = ho->hoOEFlags;
	
	if (flags & OEFLAG_MOVEMENTS) {
		if (flags & OEFLAG_SPRITES) {
			if (flags & OEFLAG_ANIMATIONS) {
				return (tagRSPR*)((char*)obj + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRM) + sizeof(tagRA));
			}
			else {
				return (tagRSPR*)((char*)obj + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRM));
			}
		}
	}
	else if (flags & OEFLAG_SPRITES) {
		if (flags & OEFLAG_ANIMATIONS) { // This should never be valid
			return (tagRSPR*)((char*)obj + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRA));
		}
		else {
			return (tagRSPR*)((char*)obj + sizeof(headerObject) + sizeof(tagRCOM));
		}
	}

	return 0;
}

tagRV* GetRV (LPRO obj) {
	if (!obj) return 0;

	LPHO ho = GetHO(obj);
	DWORD flags = ho->hoOEFlags;

	if ((flags & OEFLAG_VALUES) == 0) {
		return 0;
	}
	
	if (flags & OEFLAG_MOVEMENTS) {
		const size_t base = sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRM);
		if (flags & OEFLAG_ANIMATIONS && flags & OEFLAG_SPRITES) {
			return (tagRV*)((char*)obj + base + sizeof(tagRA) + sizeof(tagRSPR));
		}
		else if (flags & OEFLAG_SPRITES) {
			return (tagRV*)((char*)obj + base + sizeof(tagRSPR));
		}
		else {
			return (tagRV*)((char*)obj + base);
		}
	}
	else if (flags & OEFLAG_ANIMATIONS && flags & OEFLAG_SPRITES) {
		return (tagRV*)((char*)obj + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRA) + sizeof(tagRSPR)); // should never be valid
	}
	else if (flags & OEFLAG_ANIMATIONS) {
		return (tagRV*)((char*)obj + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRA));
	}
	else if (flags & OEFLAG_SPRITES) {
		return (tagRV*)((char*)obj + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRSPR));
	}
	else {
		return (tagRV*)((char*)obj + sizeof(headerObject));
	}
}

// =====================================================================================
//
// General Object Management
//
// =====================================================================================

int LuaMMF::ObjectUpdate (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	obj->roc.rcChanged = 1;

	return 0;
}

int LuaMMF::ObjectGetFixed (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, (obj->roHo.hoCreationId<<16)+obj->roHo.hoNumber);

	return 1;
}

int LuaMMF::ObjectGetName (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushstring(L, (LPSTR) &(obj->roHo.hoOiList->oilName));

	return 1;
}

int LuaMMF::ObjectGetIdentifier (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoIdentifier);

	return 1;
}

int LuaMMF::ObjectGetClass (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoOi);

	return 1;
}

int LuaMMF::ObjectDestroy (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	obj->roHo.hoAdRunHeader->rh4.rh4KpxFunctions[RFUNCTION_DESTROY].routine((LPHO)obj, 0, 0);

	return 1;
}

int LuaMMF::ObjectExport (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	if (!ParamCheck(L, 2)) return 0;
	int fixed = lua_tointeger(L, 1);
	int objid = lua_tointeger(L, 2);

	LPRDATA rdPtr = state->rdList.front()->rdPtr;
	LPRO obj = (LPRO) rdPtr->rHo.hoAdRunHeader->rhObjectList[0x0000FFFF&fixed].oblOffset;

	if ((headerObject*)obj != 0) {
		state->ExportObject(objid, obj);
	}
	else {
		state->RaiseError("Object.Export: Invalid Fixed ID");
	}

	return 0;
}

int LuaMMF::GetCurrentEvent (lua_State *L) {

	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushinteger(L, rh->rhEventGroup->evgIdentifier);

	return 1;
}


// =====================================================================================
//
// Local Storage Management
//
// =====================================================================================

int LuaMMF::LocalStoreGetValue (lua_State *L) {
	// Do a parameter check first
	int paramCount = lua_gettop(L);
	if (paramCount != 1) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	int id = lua_tointeger(L, 1);

	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	lua_pushnumber(L, state->GetLocalValue(id));

	return 1;
}

int LuaMMF::LocalStoreGetString (lua_State *L) {
	// Do a parameter check first
	int paramCount = lua_gettop(L);
	if (paramCount != 1) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	int id = lua_tointeger(L, 1);

	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	lua_pushstring(L, state->GetLocalString(id).c_str());

	return 1;
}

int LuaMMF::LocalStoreSetValue (lua_State *L) {
	// Do a parameter check first
	int paramCount = lua_gettop(L);
	if (paramCount != 2) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	int id = lua_tointeger(L, 1);
	double v = lua_tonumber(L, 2);

	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	state->SetLocalValue(id, v);

	return 0;
}

int LuaMMF::LocalStoreSetString (lua_State *L) {
	// Do a parameter check first
	int paramCount = lua_gettop(L);
	if (paramCount != 2) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	int id = lua_tointeger(L, 1);
	const char* str = lua_tostring(L, 2);

	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	state->SetLocalString(id, str);

	return 0;
}

// =====================================================================================
//
// Positional Access
//
// =====================================================================================

int LuaMMF::ObjectGetX (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoX);
	return 1;
}

int LuaMMF::ObjectGetY (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoY);
	return 1;
}

int LuaMMF::ObjectGetPosition (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoX);
	lua_pushnumber(L, obj->roHo.hoY);
	return 2;
}

int LuaMMF::ObjectGetXLeft (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoX - obj->roHo.hoImgXSpot);
	return 1;
}

int LuaMMF::ObjectGetXRight (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoX + (obj->roHo.hoImgWidth - obj->roHo.hoImgXSpot));
	return 1;
}

int LuaMMF::ObjectGetYTop (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoY - obj->roHo.hoImgYSpot);
	return 1;
}

int LuaMMF::ObjectGetYBottom (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoY + (obj->roHo.hoImgHeight - obj->roHo.hoImgYSpot));
	return 1;
}

int LuaMMF::ObjectSetX (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	if (obj == NULL) return 0;

	obj->roHo.hoAdRunHeader->rh4.rh4KpxFunctions[RFUNCTION_SETPOSITION].routine(&(obj->roHo), lua_tointeger(L, 2), obj->roHo.hoY);

	return 0;
}

int LuaMMF::ObjectSetY (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	if (obj == NULL) return 0;

	obj->roHo.hoAdRunHeader->rh4.rh4KpxFunctions[RFUNCTION_SETPOSITION].routine(&(obj->roHo), obj->roHo.hoX, lua_tointeger(L, 2));

	return 0;
}

int LuaMMF::ObjectSetPosition (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 3);
	if (obj == NULL) return 0;

	obj->roHo.hoAdRunHeader->rh4.rh4KpxFunctions[RFUNCTION_SETPOSITION].routine(&(obj->roHo), lua_tointeger(L, 2), lua_tointeger(L, 3));

	return 0;
}

int LuaMMF::ObjectGetWidth (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoImgWidth);
	return 1;
}

int LuaMMF::ObjectGetHeight (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoImgHeight);
	return 1;
}

int LuaMMF::ObjectGetLayer (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	if (com->rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rhIdMainWin;
		lua_pushnumber(L, GetSpriteLayer(win, com->rcSprite) + 1);
		return 1;
	}

	//lua_pushnumber(L, obj->roHo.hoLayer);
	return 0;
}

int LuaMMF::ObjectSetLayer (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	int layer = lua_tointeger(L, 2);
	if (com->rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		SetSpriteLayer(win, com->rcSprite, layer - 1);
		obj->roHo.hoLayer = layer - 1;
	}

	return 0;
}

int LuaMMF::ObjectGetZOrder (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	if (com->rcSprite) {
		lua_pushnumber(L, com->rcSprite->sprZOrder);
	}
	else {
		lua_pushnil(L);
	}

	return 1;
}

int LuaMMF::ObjectMoveToFront (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	if (com->rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteToFront(win, com->rcSprite);
	}

	return 0;
}

int LuaMMF::ObjectMoveToBack (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	if (com->rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteToBack(win, com->rcSprite);
	}

	return 0;
}

int LuaMMF::ObjectMoveAbove (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRCOM* com = GetCOM(obj);
	LPRO obj2 = state->GetExport(lua_tointeger(L, 2));
	tagRCOM* com2 = GetCOM(obj2);

	if (obj == NULL || obj2 == NULL || com == NULL || com2 == NULL) return 0;

	if (com->rcSprite && com2->rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteAfter(win, com->rcSprite, com2->rcSprite);
		//obj->ros.rsZOrder = obj->roc.rcSprite->sprZOrder;
	}

	return 0;
}

int LuaMMF::ObjectMoveBelow (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRCOM* com = GetCOM(obj);
	LPRO obj2 = state->GetExport(lua_tointeger(L, 2));
	tagRCOM* com2 = GetCOM(obj2);

	if (obj == NULL || obj2 == NULL || com == NULL || com2 == NULL) return 0;

	if (com->rcSprite && com2->rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteBefore(win, com->rcSprite, com2->rcSprite);
	}

	return 0;
}

int LuaMMF::ObjectIsAbove (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRCOM* com = GetCOM(obj);
	LPRO obj2 = state->GetExport(lua_tointeger(L, 2));
	tagRCOM* com2 = GetCOM(obj2);

	if (obj == NULL || obj2 == NULL || com == NULL || com2 == NULL) return 0;

	if (com->rcSprite && com2->rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		lua_pushboolean(L, IsSpriteAfter(win, com->rcSprite, com2->rcSprite));
		return 1;
	}

	return 0;
}

int LuaMMF::ObjectIsBelow (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRCOM* com = GetCOM(obj);
	LPRO obj2 = state->GetExport(lua_tointeger(L, 2));
	tagRCOM* com2 = GetCOM(obj2);

	if (obj == NULL || obj2 == NULL || com == NULL || com2 == NULL) return 0;

	if (com->rcSprite && com2->rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		lua_pushboolean(L, IsSpriteBefore(win, com->rcSprite, com2->rcSprite));
		return 1;
	}

	return 0;
}

int LuaMMF::ObjectSwapOrder (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRCOM* com = GetCOM(obj);
	LPRO obj2 = state->GetExport(lua_tointeger(L, 2));
	tagRCOM* com2 = GetCOM(obj2);

	if (obj == NULL || obj2 == NULL || com == NULL || com2 == NULL) return 0;

	if (com->rcSprite && com2->rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		SwapSprites(win, com->rcSprite, com2->rcSprite);
	}

	return 0;
}

// =====================================================================================
//
// Hotspot Access
//
// =====================================================================================

int LuaMMF::ObjectGetSpotX (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoImgXSpot);
	return 1;
}

int LuaMMF::ObjectGetSpotY (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoImgYSpot);
	return 1;
}

int LuaMMF::ObjectGetHotspot (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	if (obj == NULL) return 0;

	lua_pushnumber(L, obj->roHo.hoImgXSpot);
	lua_pushnumber(L, obj->roHo.hoImgYSpot);

	return 2;
}

int LuaMMF::ObjectGetActionX (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	if (com->rcSprite) {
		Img info;
		npAppli app = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdAppli;
		GetImageInfoEx(app, com->rcSprite->sprImg, 0, 1, 1, &info);
		lua_pushnumber(L, info.imgXAction);
		return 1;
	}

	return 0;
}

int LuaMMF::ObjectGetActionY (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	if (com->rcSprite) {
		Img info;
		npAppli app = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdAppli;
		GetImageInfoEx(app, com->rcSprite->sprImg, 0, 1, 1, &info);
		lua_pushnumber(L, info.imgYAction);
		return 1;
	}

	return 0;
}

int LuaMMF::ObjectGetActionPoint (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	if (com->rcSprite) {
		Img info;
		npAppli app = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdAppli;
		GetImageInfoEx(app, com->rcSprite->sprImg, 0, 1, 1, &info);
		lua_pushnumber(L, info.imgXAction);
		lua_pushnumber(L, info.imgYAction);
		return 2;
	}

	return 0;
}

// =====================================================================================
//
// Animation & Direction
//
// =====================================================================================

int LuaMMF::ObjectGetDirection (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	lua_pushnumber(L, com->rcDir);

	return 1;
}

int LuaMMF::ObjectSetDirection (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	com->rcDir = lua_tointeger(L, 2);

	return 0;
}

int LuaMMF::ObjectGetAnimation (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	lua_pushnumber(L, com->rcAnim);

	return 1;
}

int LuaMMF::ObjectGetAnimationFrame (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRA* ra = GetRA(obj);
	if (obj == NULL || ra == NULL) return 0;

	//lua_pushnumber(L, obj->roc.rcImage);
	lua_pushnumber(L, ra->raAnimFrame - 1);

	return 1;
}

int LuaMMF::ObjectSetAnimation (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRCOM* com = GetCOM(obj);
	tagRA* ra = GetRA(obj);

	if (obj == NULL || com == NULL || ra == NULL) return 0;

	com->rcAnim = lua_tointeger(L, 2);
	//ra->raAnimOn = lua_tointeger(L, 2);
	//obj->roHo.hoAdRunHeader->rh4.rh4Animations(&(obj->roHo), lua_tointeger(L, 2));

	return 0;
}

int LuaMMF::ObjectSetAnimationFrame (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRA* ra = GetRA(obj);
	if (obj == NULL || ra == NULL) return 0;

	ra->raAnimFrame = lua_tointeger(L, 2) + 1;
	ra->raAnimFrameForced = ra->raAnimFrame;

	return 0;
}

// =====================================================================================
//
// Scale & Rotation
//
// =====================================================================================

int LuaMMF::ObjectGetAngle (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	int rtHWA = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvCallFunction(NULL, EF_ISHWA, 0, 0, 0);

	if (rtHWA) {
		float val = *(float*) &com->rcAngle;
		lua_pushnumber(L, val);
	}
	else {
		lua_pushinteger(L, com->rcAngle);
	}

	return 1;
}

int LuaMMF::ObjectSetAngle (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);

	// Do object lookup
	if (lua_gettop(L) < 2) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	LPRO obj = state->GetExport(lua_tointeger(L, 1));
	tagRCOM* com = GetCOM(obj);
	tagRSPR* spr = GetRSPR(obj);
	if (obj == NULL || com == NULL) return 0;

	int rtHWA = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvCallFunction(NULL, EF_ISHWA, 0, 0, 0);

	if (spr) {
		if (lua_toboolean(L, 3) == 1) {
			spr->rsFlags = spr->rsFlags | RSFLAG_ROTATE_ANTIA;
		}
		else {
			spr->rsFlags = spr->rsFlags & ~RSFLAG_ROTATE_ANTIA;
		}
	}

	if (rtHWA) {
		float val = (float) lua_tonumber(L, 2);
		com->rcAngle = *(long*) &val;
	}
	else {
		com->rcAngle = lua_tointeger(L, 2) % 360;
	}

	return 0;
}

int LuaMMF::ObjectGetScaleX (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	lua_pushnumber(L, com->rcScaleX);

	return 1;
}

int LuaMMF::ObjectGetScaleY (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	lua_pushnumber(L, com->rcScaleY);

	return 1;
}

int LuaMMF::ObjectGetScale (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	lua_pushnumber(L, com->rcScaleX);
	lua_pushnumber(L, com->rcScaleY);

	return 2;
}

int LuaMMF::ObjectSetScaleX (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	com->rcScaleX = (float)lua_tonumber(L, 2);

	return 0;
}

int LuaMMF::ObjectSetScaleY (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	com->rcScaleY = (float)lua_tonumber(L, 2);

	return 0;
}

int LuaMMF::ObjectSetScale (lua_State *L) {
	// Do object lookup
	//LPRO obj = ObjectLookup(L, 3);
	//if (obj == NULL) return 0;

	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);

	// Do object lookup
	if (lua_gettop(L) < 3) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	LPRO obj = state->GetExport(lua_tointeger(L, 1));
	tagRCOM* com = GetCOM(obj);
	tagRSPR* spr = GetRSPR(obj);
	if (obj == NULL || com == NULL) return 0;

	if (spr) {
		if (lua_toboolean(L, 4) == 1) {
			spr->rsFlags = spr->rsFlags | RSFLAG_SCALE_RESAMPLE;
		}
		else {
			spr->rsFlags = spr->rsFlags & ~RSFLAG_SCALE_RESAMPLE;
		}
	}

	com->rcScaleX = (float)lua_tonumber(L, 2);
	com->rcScaleY = (float)lua_tonumber(L, 3);

	return 0;
}

// =====================================================================================
//
// Visibility & Display
//
// =====================================================================================

int LuaMMF::ObjectGetVisibility (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 1);
	tagRCOM* com = GetCOM(obj);
	tagRSPR* spr = GetRSPR(ObjectLookup(L, 1));

	if (obj == NULL || com == NULL) return 0;

	//lua_pushboolean(L, (obj->ros.rsFlags & RSFLAG_VISIBLE) != 0);
	//lua_pushboolean(L, (spr->rsFlags & RSFLAG_HIDDEN) == 0);

	if (com->rcSprite) {
		lua_pushboolean(L, (com->rcSprite->sprFlags & SF_HIDDEN) == 0);
	}
	else if (spr) {
		lua_pushboolean(L, (spr->rsFlags & RSFLAG_HIDDEN) == 0);
	}

	return 1;
}

int LuaMMF::ObjectSetVisibility (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRCOM* com = GetCOM(obj);
	tagRSPR* spr = GetRSPR(obj);

	if (obj == NULL || com == NULL) return 0;

	int flag = lua_toboolean(L, 2);
	int nflag = (flag) ? 0 : 1;

	if (com->rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		ShowSprite(win, com->rcSprite, flag);
	}

	if (spr) {
		spr->rsFlags = (spr->rsFlags & ~RSFLAG_VISIBLE) | (flag << 5);
		spr->rsFlags = (spr->rsFlags & ~RSFLAG_HIDDEN) | (nflag << 0);
		spr->rsCreaFlags = (spr->rsCreaFlags & ~SF_HIDDEN) | (nflag << 7);
	}

	return 0;
}

// =====================================================================================
//
// Alterable Values
//
// =====================================================================================

int LuaMMF::ObjectGetValue (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRV* rv = GetRV(obj);
	if (obj == NULL || rv == NULL) return 0;

	unsigned altId = lua_tointeger(L, 2);
	if (altId >= VALUES_NUMBEROF_ALTERABLE) {
		lua_pushstring(L, "Alt value index out of range");
		lua_error(L);
	}

	switch (rv->rvpValues[altId].m_type) {
		case TYPE_LONG: lua_pushinteger(L, rv->rvpValues[altId].m_long); break;
		case TYPE_FLOAT: lua_pushnumber(L, rv->rvpValues[altId].m_double); break;
		case TYPE_STRING: lua_pushstring(L, rv->rvpValues[altId].m_pString); break;
	}

	return 1;
}

int LuaMMF::ObjectGetString (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRV* rv = GetRV(obj);
	if (obj == NULL || rv == NULL) return 0;

	unsigned altId = lua_tointeger(L, 2);
	if (altId >= STRINGS_NUMBEROF_ALTERABLE) {
		lua_pushstring(L, "Alt string index out of range");
		lua_error(L);
	}

	if (rv->rvStrings[altId] == NULL) {
		lua_pushstring(L, "");
	}
	else {
		lua_pushstring(L, rv->rvStrings[altId]);
	}

	return 1;
}

int LuaMMF::ObjectGetFlag (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 2);
	tagRV* rv = GetRV(obj);
	if (obj == NULL || rv == NULL) return 0;

	unsigned flagId = lua_tointeger(L, 2);
	if (flagId >= 32) {
		lua_pushstring(L, "Flag id out of range");
		lua_error(L);
	}

	lua_pushboolean(L, (rv->rvValueFlags & (1 << flagId)) != 0);

	return 1;
}

int LuaMMF::ObjectSetValue (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 3);
	tagRV* rv = GetRV(obj);
	if (obj == NULL || rv == NULL) return 0;

	unsigned altId = lua_tointeger(L, 2);
	if (altId >= VALUES_NUMBEROF_ALTERABLE) {
		lua_pushstring(L, "Alt value index out of range");
		lua_error(L);
	}

	int intval = lua_tointeger(L, 3);
	double dblval = lua_tonumber(L, 3);

	// Try to figure out if it's an int or a double.
	if ((double)intval == dblval) {
		rv->rvpValues[altId].m_type = TYPE_LONG;
		rv->rvpValues[altId].m_long = intval;
	}
	else {
		rv->rvpValues[altId].m_type = TYPE_DOUBLE;
		rv->rvpValues[altId].m_double = dblval;
	}

	return 0;
}

int LuaMMF::ObjectSetString (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 3);
	tagRV* rv = GetRV(obj);
	if (obj == NULL || rv == NULL) return 0;

	unsigned altId = lua_tointeger(L, 2);
	if (altId >= STRINGS_NUMBEROF_ALTERABLE) {
		lua_pushstring(L, "Alt string index out of range");
		lua_error(L);
	}

	unsigned strlen = lua_objlen(L, 3);
	rv->rvStrings[altId] = (LPSTR) mvReAlloc(obj->roHo.hoAdRunHeader->rh4.rh4Mv, rv->rvStrings[altId], strlen + 1);
	strncpy(rv->rvStrings[altId], lua_tostring(L, 3), strlen + 1);

	return 0;
}

int LuaMMF::ObjectSetFlag (lua_State *L) {
	// Do object lookup
	LPRO obj = ObjectLookup(L, 3);
	tagRV* rv = GetRV(obj);
	if (obj == NULL || rv == NULL) return 0;

	unsigned flagId = lua_tointeger(L, 2);
	if (flagId >= 32) {
		lua_pushstring(L, "Flag id out of range");
		lua_error(L);
	}

	int bit = lua_toboolean(L, 3);
	rv->rvValueFlags = (rv->rvValueFlags & (0xFFFFFFFF ^ (!bit << flagId))) | (bit << flagId);

	return 0;
}

// =====================================================================================
//
// Collision Testing - Experimental
//
// =====================================================================================

// Layer params in terms of object/bg, not sprite
bool LayerCheck (lua_State* L, int index, int layer1 = 0, int layer2 = 0, int layer = LuaMMF::MF_ANY_LAYER) {
	//int layer = LuaMMF::MF_SAME_LAYER;
	if (lua_isnumber(L, index))
		layer = lua_tointeger(L, index);
	if (layer > 0)
		layer -= 1;

	if (layer == LuaMMF::MF_ANY_LAYER)
		return true;
	if (layer == LuaMMF::MF_SAME_LAYER && layer1 == layer2)
		return true;
	if (layer == LuaMMF::MF_DIFF_LAYER && layer1 != layer2)
		return true;
	if (layer == layer1)
		return true;

	return false;
}

bool LayerCheckBackdrop (lua_State* L, int index, int layer1 = 0, int layer2 = 0) {
	return LayerCheck(L, index, layer1, layer2, LuaMMF::MF_ANY_LAYER);
}

int LayerParam (lua_State* L, int index, int layer1 = -1, int layer = LuaMMF::MF_ANY_LAYER) {
	//int layer = LuaMMF::MF_SAME_LAYER;
	if (lua_isnumber(L, index))
		layer = lua_tointeger(L, index);
	if (layer > 0)
		layer -= 1;

	if (layer == LuaMMF::MF_ANY_LAYER || layer == LuaMMF::MF_DIFF_LAYER || (layer == LuaMMF::MF_SAME_LAYER && layer1 == -1))
		return -1;
	if (layer == LuaMMF::MF_SAME_LAYER)
		return layer1;

	return layer;
}

int LayerParamBackdrop (lua_State* L, int index, int layer1 = -1) {
	return LayerParam(L, index, layer1, LuaMMF::MF_ANY_LAYER);
}

int ClassParam (lua_State* L, int index) {
	int classid = LuaMMF::MF_ANY_CLASS;
	if (lua_isnumber(L, index))
		classid = lua_tointeger(L, index);

	return classid;
}

int SpriteExtraInfo (npSpr spr, int rtHWA) {
	if (!rtHWA)
		return spr->sprExtraInfo;

	LPARAM extra = *(int*)((char*)spr + 180);
	return extra;
}

int LuaMMF::BackdropTestPoint (lua_State *L) {
	if (lua_gettop(L) < 2) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	// Get Vars
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int layer = LayerParamBackdrop(L, 3);

	// Get Run Header
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;

	x = x - rh->rh3.rh3DisplayX;
	y = y - rh->rh3.rh3DisplayY;

	//if (!rh->rh4.rh4ColMaskTestPoint(&(rdPtr->rHo), x, y, layer, 0)) {
	if (ColMask_TestPoint(win, x, y, 0)) {
		if (!LayerCheckBackdrop(L, 3, 0))
			lua_pushboolean(L, false);
		else
			lua_pushboolean(L, true);
	}
	else {
		npSpr spr = 0;
		while ((spr = SpriteCol_TestPoint(win, spr, layer, x, y, SCF_BACKGROUND | SCF_OBSTACLE)) != 0) {
			if (!((spr->sprFlags & SF_BACKGROUND) && (spr->sprFlags & SF_OBSTACLE)))
				continue;
			lua_pushboolean(L, true);
			return 1;
		}

		lua_pushboolean(L, false);
	}

	return 1;
}

int LuaMMF::BackdropTestRect (lua_State *L) {
	if (lua_gettop(L) < 4) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	// Get Vars
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);
	int layer = LayerParamBackdrop(L, 5);

	// Get Run Header
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;

	x = x - rh->rh3.rh3DisplayX;
	y = y - rh->rh3.rh3DisplayY;

	//if (!rh->rh4.rh4ColMaskTestRect(&(rdPtr->rHo), x, y, w, h, layer, 0)) {
	if (ColMask_TestRect(win, x, y, w, h, 0)) {
		if (!LayerCheckBackdrop(L, 5, 0))
			lua_pushboolean(L, false);
		else
			lua_pushboolean(L, true);
	}
	else {
		npSpr spr = 0;
		while ((spr = SpriteCol_TestRect(win, spr, layer, x, y, w, h, SCF_BACKGROUND)) != 0) {
			if (!((spr->sprFlags & SF_BACKGROUND) && (spr->sprFlags & SF_OBSTACLE)))
				continue;

			lua_pushboolean(L, true);
			return 1;
		}

		lua_pushboolean(L, false);
	}

	return 1;
}

int LuaMMF::BackdropTestObject (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 1) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	LPRO obj = state->GetExport(lua_tointeger(L, 1));
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL || com->rcSprite == NULL) return 0;

	int layer = LayerParamBackdrop(L, 2, obj->roHo.hoLayer);

	if (com->rcSprite) {
		LPRH rh = obj->roHo.hoAdRunHeader;
		npWin win = rh->rh4.rh4Mv->mvIdEditWin;

		RECT rc;
		GetSpriteRect(win, com->rcSprite, &rc);

		int x = rc.left - rh->rh3.rh3DisplayX; //obj->roHo.hoX - obj->roc.rcOldX;
		int y = rc.top - rh->rh3.rh3DisplayY; //obj->roHo.hoY - obj->roc.rcOldY;

		if (lua_isnumber(L, 3)) {
			x += lua_tointeger(L, 3);
		}
		if (lua_isnumber(L, 4)) {
			y += lua_tointeger(L, 4);
		}

		if (ColMask_TestSprite(win, com->rcSprite, com->rcSprite->sprImg, 
			x, y, com->rcSprite->sprAngle,
			com->rcSprite->sprScaleX, com->rcSprite->sprScaleY, 0, 0)) {

			if (!LayerCheckBackdrop(L, 2, 0, obj->roHo.hoLayer))
				lua_pushboolean(L, false);
			else
				lua_pushboolean(L, true);
		}
		else {
			// We need to check for backdrops on other layers (~!!)
			npSpr spr = 0;
			sMask* mask1 = GetSpriteMask(win, com->rcSprite, com->rcSprite->sprImg, 0);
			LPBYTE maskptr1 = (LPBYTE)mask1 + sizeof(sMask);

			while ((spr = SpriteCol_TestRect(win, spr, layer, x, y,
				mask1->mkWidth, mask1->mkHeight, SCF_BACKGROUND)) != 0) {

				if (!((spr->sprFlags & SF_BACKGROUND) && (spr->sprFlags & SF_OBSTACLE)))
					continue;

				LO* lo = (LO*)GetSpriteExtra(win, spr); //SpriteExtraInfo(spr, rtHWA); //spr->sprExtraInfo;
				if (!LayerCheckBackdrop(L, 2, lo->loLayer, obj->roHo.hoLayer))
					continue;

				sMask* mask2 = GetSpriteMask(win, spr, spr->sprImg, 0);
				LPBYTE maskptr2 = (LPBYTE)mask2 + sizeof(sMask);

				int x2 = lo->loX - rh->rh3.rh3DisplayX;
				int y2 = lo->loY - rh->rh3.rh3DisplayY;

				if (Mask_TestMask(maskptr1, maskptr2, 
					x, y, mask1->mkWidth, mask1->mkHeight,
					x2, y2, mask2->mkWidth, mask2->mkHeight)) {
					lua_pushboolean(L, true);
					return 1;
				}
			}

			lua_pushboolean(L, false);
		}
	}

	return 1;
}

int LuaMMF::ObjectTestPoint (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 3) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	LPRO obj = state->GetExport(lua_tointeger(L, 1));
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	int layer = LayerParam(L, 4, obj->roHo.hoLayer);

	if (com->rcSprite) {
		LPRH rh = obj->roHo.hoAdRunHeader;
		npWin win = rh->rh4.rh4Mv->mvIdEditWin;
		npSpr spr = 0;

		x = x - rh->rh3.rh3DisplayX;
		y = y - rh->rh3.rh3DisplayY;

		while ((spr = SpriteCol_TestPoint(win, spr, layer, x, y, SCF_EVENNOCOL)) != 0) {
			if (com->rcSprite == spr) {
				lua_pushboolean(L, 1);
				return 1;
			}
		}
		
		lua_pushboolean(L, 0);
		return 1;
	}

	return 0;
}

int LuaMMF::ObjectTestRect (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 5) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	LPRO obj = state->GetExport(lua_tointeger(L, 1));
	tagRCOM* com = GetCOM(obj);
	if (obj == NULL || com == NULL) return 0;

	int x1 = lua_tointeger(L, 2);
	int y1 = lua_tointeger(L, 3);
	int x2 = lua_tointeger(L, 4);
	int y2 = lua_tointeger(L, 5);
	int layer = LayerParam(L, 6, obj->roHo.hoLayer);

	if (com->rcSprite) {
		LPRH rh = obj->roHo.hoAdRunHeader;
		npWin win = rh->rh4.rh4Mv->mvIdEditWin;
		npSpr spr = 0;

		x1 = x1 - rh->rh3.rh3DisplayX;
		y1 = y1 - rh->rh3.rh3DisplayY;

		while ((spr = SpriteCol_TestRect(win, spr, layer, x1, y1, x2, y2, SCF_EVENNOCOL)) != 0) {
			if (com->rcSprite == spr) {
				lua_pushboolean(L, 1);
				return 1;
			}
		}
		
		lua_pushboolean(L, 0);
		return 1;
	}

	return 0;
}

int LuaMMF::ObjectTestObject (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	LPRO obj = state->GetExport(lua_tointeger(L, 1));
	tagRCOM* com = GetCOM(obj);
	LPRO obj2 = state->GetExport(lua_tointeger(L, 2));
	tagRCOM* com2 = GetCOM(obj2);

	if (obj == NULL || obj2 == NULL || com == NULL || com2 == NULL) return 0;

	if (com2->rcSprite && com2->rcSprite) {
		LPRH rh = obj->roHo.hoAdRunHeader;
		npWin win = rh->rh4.rh4Mv->mvIdEditWin;

		// Layer Test
		if (!LayerCheck(L, 3, obj2->roHo.hoLayer, obj->roHo.hoLayer)) {
			lua_pushboolean(L, 0);
			return 1;
		}

		//while ((spr = SpriteCol_TestSprite(win, spr, com2->rcSprite->sprImg,
		//	com2->rcSprite->sprX, com2->rcSprite->sprY, com2->rcSprite->sprAngle,
		//	com2->rcSprite->sprScaleX, com2->rcSprite->sprScaleY, -1, SCF_EVENNOCOL)) != 0) {

		sMask* mask1 = GetSpriteMask(win, com->rcSprite, com->rcSprite->sprImg, 0);
		sMask* mask2 = GetSpriteMask(win, com2->rcSprite, com2->rcSprite->sprImg, 0);
		LPBYTE maskptr1 = (LPBYTE)mask1 + sizeof(sMask);
		LPBYTE maskptr2 = (LPBYTE)mask2 + sizeof(sMask);

		RECT rc1;
		RECT rc2;
		GetSpriteRect(win, com->rcSprite, &rc1);
		GetSpriteRect(win, com2->rcSprite, &rc2);

		int x1 = rc1.left - rh->rh3.rh3DisplayX;
		int y1 = rc1.top - rh->rh3.rh3DisplayY;
		int x2 = rc2.left - rh->rh3.rh3DisplayX; //obj2->roHo.hoX - com2->rcOldX;
		int y2 = rc2.top - rh->rh3.rh3DisplayY; //obj2->roHo.hoY - com2->rcOldY;

		if (lua_isnumber(L, 4)) {
			x1 += lua_tointeger(L, 4);
		}
		if (lua_isnumber(L, 5)) {
			y1 += lua_tointeger(L, 5);
		}

		if (Mask_TestMask(maskptr1, maskptr2, 
			x1, y1, mask1->mkWidth, mask1->mkHeight,
			x2, y2, mask2->mkWidth, mask2->mkHeight)) {
			lua_pushboolean(L, true);
			return 1;
		}

		lua_pushboolean(L, false);
		return 1;
	}

	return 0;
}

int LuaMMF::ObjectClassTestPoint (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 3) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	int objclass = lua_tointeger(L, 1);
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	int layer = LayerParam(L, 4);

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;
	npSpr spr = 0;

	x = x - rh->rh3.rh3DisplayX;
	y = y - rh->rh3.rh3DisplayY;

	while ((spr = SpriteCol_TestPoint(win, spr, layer, x, y, SCF_EVENNOCOL)) != 0) {
		if (spr->sprFlags & SF_BACKGROUND)
			continue;

		LPHO oh = (LPHO)GetSpriteExtra(win, spr); //spr->sprExtraInfo;
		if (oh->hoOi == objclass) {
			lua_pushboolean(L, 1);
			return 1;
		}
	}
	
	lua_pushboolean(L, 0);
	return 1;
}

int LuaMMF::ObjectClassTestRect (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 5) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	int objclass = lua_tointeger(L, 1);
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	int w = lua_tointeger(L, 4);
	int h = lua_tointeger(L, 5);
	int layer = LayerParam(L, 6);

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;
	npSpr spr = 0;

	x = x - rh->rh3.rh3DisplayX;
	y = y - rh->rh3.rh3DisplayY;

	while ((spr = SpriteCol_TestRect(win, spr, layer, x, y, w, h, SCF_EVENNOCOL)) != 0) {
		if (spr->sprFlags & SF_BACKGROUND)
			continue;

		LPHO oh = (LPHO)GetSpriteExtra(win, spr); //spr->sprExtraInfo;
		if (oh->hoOi == objclass) {
			lua_pushboolean(L, 1);
			return 1;
		}
	}
	
	lua_pushboolean(L, 0);
	return 1;
}

int LuaMMF::ObjectClassTestObject (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 2) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	int objclass = lua_tointeger(L, 1);
	LPRO obj = state->GetExport(lua_tointeger(L, 2));
	tagRCOM* com = GetCOM(obj);

	if (obj == NULL || com == NULL) return 0;

	if (com->rcSprite) {
		LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
		npWin win = rh->rh4.rh4Mv->mvIdEditWin;
		npSpr spr = 0;

		int layer = LayerParam(L, 3, obj->roHo.hoLayer);

		sMask* mask1 = GetSpriteMask(win, com->rcSprite, com->rcSprite->sprImg, 0);
		LPBYTE maskptr1 = (LPBYTE)mask1 + sizeof(sMask);

		RECT rc1;
		GetSpriteRect(win, com->rcSprite, &rc1);

		int x1 = rc1.left - rh->rh3.rh3DisplayX;
		int y1 = rc1.top - rh->rh3.rh3DisplayY;

		if (lua_isnumber(L, 4)) {
			x1 += lua_tointeger(L, 4);
		}
		if (lua_isnumber(L, 5)) {
			y1 += lua_tointeger(L, 5);
		}

		while ((spr = SpriteCol_TestRect(win, spr, layer, x1, y1,
			mask1->mkWidth, mask1->mkHeight, SCF_EVENNOCOL)) != 0) {
			if (spr->sprFlags & SF_BACKGROUND)
				continue;

			LPRO obj2 = (LPRO)GetSpriteExtra(win, spr); //spr->sprExtraInfo;
			tagRCOM* com2 = GetCOM(obj2);

			if (obj2 == NULL || com2 == NULL) 
				continue;

			if (com->rcSprite == spr)
				continue;

			if (obj2->roHo.hoOi == objclass) {
				if (!LayerCheck(L, 3, obj2->roHo.hoLayer, obj->roHo.hoLayer))
					continue;

				sMask* mask2 = GetSpriteMask(win, spr, spr->sprImg, 0);
				LPBYTE maskptr2 = (LPBYTE)mask2 + sizeof(sMask);

				RECT rc2;
				GetSpriteRect(win, com2->rcSprite, &rc2);

				int x2 = rc2.left - rh->rh3.rh3DisplayX; //obj2->roHo.hoX - com2->rcOldX;
				int y2 = rc2.top - rh->rh3.rh3DisplayY; //obj2->roHo.hoY - com2->rcOldY;

				if (Mask_TestMask(maskptr1, maskptr2, 
					x1, y1, mask1->mkWidth, mask1->mkHeight,
					x2, y2, mask2->mkWidth, mask2->mkHeight)) {
					lua_pushboolean(L, true);
					return 1;
				}
			}
		}
	}
	
	lua_pushboolean(L, 0);
	return 1;
}

int LuaMMF::GetObjectsOverPoint (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 2) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int classid = ClassParam(L, 3);
	int layer = LayerParam(L, 4);

	int matchCount = 0;
	lua_newtable(L);

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;
	npSpr spr = 0;

	x = x - rh->rh3.rh3DisplayX;
	y = y - rh->rh3.rh3DisplayY;

	while ((spr = SpriteCol_TestPoint(win, spr, layer, x, y, SCF_EVENNOCOL)) != 0) {
		if (spr->sprFlags & SF_BACKGROUND)
			continue;

		LPRO obj = (LPRO)GetSpriteExtra(win, spr); //spr->sprExtraInfo;
		tagRCOM* com2 = GetCOM(obj);

		if (obj == NULL || com2 == NULL) continue;

		if (classid != LuaMMF::MF_ANY_CLASS && classid != obj->roHo.hoOi)
			continue;

		matchCount++;
		lua_pushnumber(L, (obj->roHo.hoCreationId<<16)+obj->roHo.hoNumber);
		lua_rawseti(L, -2, matchCount);
	}

	return 1;
}

int LuaMMF::GetObjectsOverRect (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 4) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);
	int classid = ClassParam(L, 5);
	int layer = LayerParam(L, 6);

	int matchCount = 0;
	lua_newtable(L);

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;
	npSpr spr = 0;

	x = x - rh->rh3.rh3DisplayX;
	y = y - rh->rh3.rh3DisplayY;

	while ((spr = SpriteCol_TestRect(win, spr, layer, x, y, w, h, SCF_EVENNOCOL)) != 0) {
		if (spr->sprFlags & SF_BACKGROUND)
			continue;

		LPRO obj = (LPRO)GetSpriteExtra(win, spr); //spr->sprExtraInfo;
		tagRCOM* com2 = GetCOM(obj);

		if (obj == NULL || com2 == NULL) continue;

		if (classid != LuaMMF::MF_ANY_CLASS && classid != obj->roHo.hoOi)
			continue;

		matchCount++;
		lua_pushnumber(L, (obj->roHo.hoCreationId<<16)+obj->roHo.hoNumber);
		lua_rawseti(L, -2, matchCount);
	}

	return 1;
}

int* GetSpriteColList(npSpr spr, int rtHWA) {
	if (!rtHWA)
		return &spr->sprCollisList[0];

	return (int*)((char*)spr + spr->sprObj.objSize - 8);
}

int LuaMMF::GetObjectsOverObject (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 1) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	LPRO obj = state->GetExport(lua_tointeger(L, 1));
	tagRCOM* com = GetCOM(obj);

	if (obj == NULL || com == NULL || com->rcSprite == NULL) return 0;

	int classid = ClassParam(L, 2);
	int layer = LayerParam(L, 3);

	int xoff = 0;
	int yoff = 0;

	if (lua_isnumber(L, 4))
		xoff = lua_tointeger(L, 4);
	if (lua_isnumber(L, 5))
		yoff = lua_tointeger(L, 5);

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;
	int rtHWA = rh->rh4.rh4Mv->mvCallFunction(NULL, EF_ISHWA, 0, 0, 0);
	int matchCount = 0;

	lua_newtable(L);
	
	sMask* mask1 = GetSpriteMask(win, com->rcSprite, com->rcSprite->sprImg, 0);
	LPBYTE maskptr1 = (LPBYTE)mask1 + sizeof(sMask);

	// If cache is absent or we're offsetting, do a manual lookup
	//int* colList1 = GetSpriteColList(com->rcSprite, rtHWA);
	//int nSprites = colList1[0]; //com->rcSprite->sprCollisList[0];
	//if (nSprites < 0 || nSprites == SF_UPDATECOLLIST || xoff != 0 || yoff != 0) {
		
		npSpr spr = 0;

		RECT rc1;
		GetSpriteRect(win, com->rcSprite, &rc1);

		int x1 = rc1.left - rh->rh3.rh3DisplayX + xoff;
		int y1 = rc1.top - rh->rh3.rh3DisplayY + yoff;

		while ((spr = SpriteCol_TestRect(win, spr, layer, x1, y1,
			mask1->mkWidth, mask1->mkHeight, SCF_EVENNOCOL)) != 0) {

			if (spr->sprFlags & SF_BACKGROUND)
				continue;

			LPRO obj2 = (LPRO)GetSpriteExtra(win, spr); //spr->sprExtraInfo;
			tagRCOM* com2 = GetCOM(obj2);

			if (obj2 == NULL || com2 == NULL || com2->rcSprite == NULL)
				continue;

			if (com->rcSprite == spr)
				continue;

			if (classid != LuaMMF::MF_ANY_CLASS && classid != obj->roHo.hoOi)
				continue;

			if (!LayerCheck(L, 3, obj2->roHo.hoLayer, obj->roHo.hoLayer))
				continue;

			// Figure out what to do with this case
			if (spr->sprFlags & SF_OWNERDRAW)
				continue;

			sMask* mask2 = GetSpriteMask(win, spr, spr->sprImg, 0);
			LPBYTE maskptr2 = (LPBYTE)mask2 + sizeof(sMask);

			RECT rc2;
			GetSpriteRect(win, com2->rcSprite, &rc2);

			int x2 = rc2.left - rh->rh3.rh3DisplayX; //obj2->roHo.hoX - com2->rcOldX;
			int y2 = rc2.top - rh->rh3.rh3DisplayY; //obj2->roHo.hoY - com2->rcOldY;

			if (Mask_TestMask(maskptr1, maskptr2, 
				x1, y1, mask1->mkWidth, mask1->mkHeight,
				x2, y2, mask2->mkWidth, mask2->mkHeight)) {
				
				matchCount++;
				lua_pushnumber(L, (obj->roHo.hoCreationId<<16)+obj->roHo.hoNumber);
				lua_rawseti(L, -2, matchCount);
			}
		}
	//}

	// Otherwise use the available cache, it's faster
	/*else {
		Spr** sprlist = (Spr**)(com->rcSprite->sprCollisList[1]);
		for (int i = 0; i < nSprites; i++) {
			Spr* sprite = (Spr*)(sprlist[i]);

			if (sprite->sprFlags & SF_BACKGROUND)
				continue;

			LPRO obj2 = (LPRO)(sprite->sprExtraInfo);

			tagRCOM* com2 = GetCOM(obj2);
			if (com2 == NULL || com2->rcSprite == NULL)
				continue;

			if (classid != LuaMMF::MF_ANY_CLASS && classid != obj->roHo.hoOi)
				continue;

			if (!LayerCheck(L, 3, obj2->roHo.hoLayer, obj->roHo.hoLayer))
				continue;

			sMask* mask2 = GetSpriteMask(win, com2->rcSprite, com2->rcSprite->sprImg, 0);
			LPBYTE maskptr2 = (LPBYTE)mask2 + sizeof(sMask);

			int x1off = obj->roHo.hoX - com->rcOldX;
			int y1off = obj->roHo.hoY - com->rcOldY;
			int x2off = obj2->roHo.hoX - com2->rcOldX;
			int y2off = obj2->roHo.hoY - com2->rcOldY;

			if (Mask_TestMask(maskptr1, maskptr2,
				com->rcSprite->sprX1 + x1off, com->rcSprite->sprY1 + y1off, mask1->mkWidth, mask1->mkHeight,
				com2->rcSprite->sprX1 + x2off, com2->rcSprite->sprY1 + y2off, mask2->mkWidth, mask2->mkHeight)) {

				matchCount++;
				lua_pushnumber(L, (obj2->roHo.hoCreationId<<16)+obj2->roHo.hoNumber);
				lua_rawseti(L, -2, matchCount);
			}
		}
	}*/

	return 1;
}

/*int LuaMMF::TestObjectOverlap (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 2) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	LPRO obj = state->GetExport(lua_tointeger(L, 1));
	tagRCOM* com = GetCOM(obj);
	LPRO obj2 = state->GetExport(lua_tointeger(L, 2));
	tagRCOM* com2 = GetCOM(obj2);

	// Optional parameter
	int e_layer = MF_SAME_LAYER;
	if (lua_isnumber(L, 3) && lua_tointeger(L, 3) == MF_ANY_LAYER) {
		e_layer = MF_ANY_LAYER;
	}

	if (obj == NULL || obj2 == NULL || com == NULL || com2 == NULL) return 0;

	if (com->rcSprite && com2->rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;

		// If objects not on the same layer, and layer enforced, return false
		if (e_layer == MF_SAME_LAYER && com->rcSprite->sprLayer != com2->rcSprite->sprLayer) {
			lua_pushboolean(L, false);
			return 1;
		}

		sMask* mask1 = GetSpriteMask(win, com->rcSprite, com->rcSprite->sprImg, 0);
		sMask* mask2 = GetSpriteMask(win, com2->rcSprite, com2->rcSprite->sprImg, 0);

		LPBYTE maskptr1 = (LPBYTE)mask1 + sizeof(sMask);
		LPBYTE maskptr2 = (LPBYTE)mask2 + sizeof(sMask);

		int x1off = obj->roHo.hoX - com->rcOldX;
		int y1off = obj->roHo.hoY - com->rcOldY;
		int x2off = obj2->roHo.hoX - com2->rcOldX;
		int y2off = obj2->roHo.hoY - com2->rcOldY;

		if (lua_isnumber(L, 4)) {
			x1off += lua_tointeger(L, 4);
		}
		if (lua_isnumber(L, 5)) {
			y1off += lua_tointeger(L, 5);
		}

		if (Mask_TestMask(maskptr1, maskptr2,
			com->rcSprite->sprX1 + x1off,com->rcSprite->sprY1 + y1off, mask1->mkWidth, mask1->mkHeight,
			com2->rcSprite->sprX1 + x2off, com2->rcSprite->sprY1 + y2off, mask2->mkWidth, mask2->mkHeight)) {
			lua_pushboolean(L, true);
		}
		else {
			lua_pushboolean(L, false);
		}

		return 1;
	}

	return 0;
}*/

/*int LuaMMF::TestObjectClassOverlap (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 2) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	LPRO obj = state->GetExport(lua_tointeger(L, 1));
	tagRCOM* com = GetCOM(obj);
	int objclass = lua_tointeger(L, 2);

	// Optional parameter
	int e_layer = MF_SAME_LAYER;
	if (lua_isnumber(L, 3) && lua_tointeger(L, 3) == MF_ANY_LAYER) {
		e_layer = MF_ANY_LAYER;
	}

	if (obj == NULL || com == NULL || com->rcSprite == NULL) return 0;

	npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;

	int nSprites = com->rcSprite->sprCollisList[0];
	if (nSprites <= 0 || nSprites == SF_UPDATECOLLIST) {
		lua_pushboolean(L, false);
		return 1;
	}

	sMask* mask1 = GetSpriteMask(win, com->rcSprite, com->rcSprite->sprImg, 0);
	LPBYTE maskptr1 = (LPBYTE)mask1 + sizeof(sMask);

	Spr** sprlist = (Spr**)(com->rcSprite->sprCollisList[1]);
	for (int i = 0; i < nSprites; i++) {
		Spr* sprite = (Spr*)(sprlist[i]);
		LPRO target = (LPRO)(sprite->sprExtraInfo);

		tagRCOM* tcom = GetCOM(target);
		if (tcom == NULL)
			continue;
		
		if (target->roHo.hoOi == objclass) {
			if (e_layer == MF_ANY_LAYER || (e_layer == MF_SAME_LAYER && com->rcSprite->sprLayer == sprite->sprLayer)) {
				sMask* mask2 = GetSpriteMask(win, tcom->rcSprite, tcom->rcSprite->sprImg, 0);
				LPBYTE maskptr2 = (LPBYTE)mask2 + sizeof(sMask);

				int x1off = obj->roHo.hoX - com->rcOldX;
				int y1off = obj->roHo.hoY - com->rcOldY;
				int x2off = target->roHo.hoX - tcom->rcOldX;
				int y2off = target->roHo.hoY - tcom->rcOldY;

				if (Mask_TestMask(maskptr1, maskptr2,
					com->rcSprite->sprX1 + x1off, com->rcSprite->sprY1 + y1off, mask1->mkWidth, mask1->mkHeight,
					tcom->rcSprite->sprX1 + x2off, tcom->rcSprite->sprY1 + y2off, mask2->mkWidth, mask2->mkHeight)) {
					lua_pushboolean(L, true);
					return 1;
				}
			}
		}
	}

	lua_pushboolean(L, false);
	return 1;
}

int LuaMMF::GetObjectOverlap (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;

	// Do object lookup
	if (lua_gettop(L) < 1) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return 0;
	}

	LPRO obj = state->GetExport(lua_tointeger(L, 1));
	tagRCOM* com = GetCOM(obj);

	// Optional parameters
	int e_class = MF_ANY_CLASS;
	int e_layer = MF_SAME_LAYER;

	if (lua_isnumber(L, 2)) {
		e_class = lua_tointeger(L, 2);
	}
	if (lua_isnumber(L, 3) && lua_tointeger(L, 3) == MF_ANY_LAYER) {
		e_layer = MF_ANY_LAYER;
	}

	if (obj == NULL || com == NULL || com->rcSprite == NULL) return 0;

	npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;

	int matchCount = 0;
	lua_newtable(L);

	int nSprites = com->rcSprite->sprCollisList[0];
	if (nSprites <= 0 || nSprites == SF_UPDATECOLLIST) {
		return 1;
	}

	sMask* mask1 = GetSpriteMask(win, com->rcSprite, com->rcSprite->sprImg, 0);
	LPBYTE maskptr1 = (LPBYTE)mask1 + sizeof(sMask);

	Spr** sprlist = (Spr**)(com->rcSprite->sprCollisList[1]);
	for (int i = 0; i < nSprites; i++) {
		Spr* sprite = (Spr*)(sprlist[i]);
		LPRO target = (LPRO)(sprite->sprExtraInfo);

		tagRCOM* tcom = GetCOM(target);
		if (tcom == NULL)
			continue;
		
		if (e_class == MF_ANY_CLASS || target->roHo.hoOi == e_class) {
			if (e_layer == MF_ANY_LAYER || (e_layer == MF_SAME_LAYER && com->rcSprite->sprLayer == sprite->sprLayer)) {
				sMask* mask2 = GetSpriteMask(win, tcom->rcSprite, tcom->rcSprite->sprImg, 0);
				LPBYTE maskptr2 = (LPBYTE)mask2 + sizeof(sMask);

				int x1off = obj->roHo.hoX - com->rcOldX;
				int y1off = obj->roHo.hoY - com->rcOldY;
				int x2off = target->roHo.hoX - tcom->rcOldX;
				int y2off = target->roHo.hoY - tcom->rcOldY;

				if (Mask_TestMask(maskptr1, maskptr2,
					com->rcSprite->sprX1 + x1off, com->rcSprite->sprY1 + y1off, mask1->mkWidth, mask1->mkHeight,
					tcom->rcSprite->sprX1 + x2off, tcom->rcSprite->sprY1 + y2off, mask2->mkWidth, mask2->mkHeight)) {
					matchCount++;
					lua_pushnumber(L, (target->roHo.hoCreationId<<16)+target->roHo.hoNumber);
					lua_rawseti(L, -2, matchCount);
				}
			}
		}
	}

	return 1;
}*/

// =====================================================================================
//
// Global Values
//
// =====================================================================================

int LuaMMF::GetGlobalValue (lua_State *L) {
	if (!ParamCheck(L, 1)) return 0;
	unsigned valId = lua_tointeger(L, 1);

	// Get Application object
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	fprh rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
	fpRunApp ra = rh->rhApp;
	
	// If this is a sub-app, and globals are shared, use parent app instead
	while (ra->m_pParentApp && ra->m_bShareGlobalValues) {
		ra = ra->m_pParentApp;
	}

	// Check if request is within range
	if (valId >= (unsigned)ra->m_nGlobalValues) {
		lua_pushstring(L, "Global value id out of range");
		lua_error(L);
	}

	switch (ra->m_pGlobalValues[valId].m_type) {
		case TYPE_LONG: lua_pushinteger(L, ra->m_pGlobalValues[valId].m_long); break;
		case TYPE_DOUBLE: lua_pushnumber(L, ra->m_pGlobalValues[valId].m_double); break;
		case TYPE_STRING: lua_pushstring(L, ra->m_pGlobalValues[valId].m_pString); break;
	}

	return 1;
}

int LuaMMF::GetGlobalString (lua_State *L) {
	if (!ParamCheck(L, 1)) return 0;
	unsigned strId = lua_tointeger(L, 1);

	// Get Application object
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	fprh rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
	fpRunApp ra = rh->rhApp;
	
	// If this is a sub-app, and globals are shared, use parent app instead
	while (ra->m_pParentApp && ra->m_bShareGlobalStrings) {
		ra = ra->m_pParentApp;
	}

	// Check if request is within range
	if (strId >= (unsigned)ra->m_nGlobalStrings) {
		lua_pushstring(L, "Global string id out of range");
		lua_error(L);
	}

	lua_pushstring(L, ra->m_pGlobalString[strId]);

	return 1;
}

int LuaMMF::SetGlobalValue (lua_State *L) {
	if (!ParamCheck(L, 2)) return 0;
	unsigned valId = lua_tointeger(L, 1);
	double dblval = lua_tonumber(L, 2);
	int intval = lua_tointeger(L, 2);

	// Get Application object
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	fprh rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
	fpRunApp ra = rh->rhApp;
	
	// If this is a sub-app, and globals are shared, use parent app instead
	while (ra->m_pParentApp && ra->m_bShareGlobalValues) {
		ra = ra->m_pParentApp;
	}

	// Check if request is within range
	if (valId >= (unsigned)ra->m_nGlobalValues) {
		lua_pushstring(L, "Global value id out of range");
		lua_error(L);
	}

	// Try to figure out if it's an int or a double.
	if ((double)intval == dblval) {
		ra->m_pGlobalValues[valId].m_type = TYPE_LONG;
		ra->m_pGlobalValues[valId].m_long = intval;
	}
	else {
		ra->m_pGlobalValues[valId].m_type = TYPE_DOUBLE;
		ra->m_pGlobalValues[valId].m_double = dblval;
	}

	return 0;
}

int LuaMMF::SetGlobalString (lua_State *L) {
	if (!ParamCheck(L, 2)) return 0;
	unsigned strId = lua_tointeger(L, 1);

	// Get Application object
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	fprh rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;
	fpRunApp ra = rh->rhApp;
	
	// If this is a sub-app, and globals are shared, use parent app instead
	while (ra->m_pParentApp && ra->m_bShareGlobalStrings) {
		ra = ra->m_pParentApp;
	}

	// Check if request is within range
	if (strId >= (unsigned)ra->m_nGlobalStrings) {
		lua_pushstring(L, "Global string id out of range");
		lua_error(L);
	}

	unsigned strlen = lua_objlen(L, 2);
	ra->m_pGlobalString[strId] = (LPSTR) mvReAlloc(rh->rh4.rh4Mv, ra->m_pGlobalString[strId], strlen + 1);
	strncpy(ra->m_pGlobalString[strId], lua_tostring(L, 2), strlen + 1);

	return 0;
}

// =====================================================================================
//
// Frame
//
// =====================================================================================

int LuaMMF::FrameGetXLeft (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rh3.rh3DisplayX);
	return 1;
}

int LuaMMF::FrameGetXRight (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rh3.rh3DisplayX + rh->rh3.rh3WindowSx);
	return 1;
}

int LuaMMF::FrameGetYTop (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rh3.rh3DisplayY);
	return 1;
}

int LuaMMF::FrameGetYBottom (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rh3.rh3DisplayY + rh->rh3.rh3WindowSy);
	return 1;
}

int LuaMMF::FrameGetWidth (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rhFrame->m_hdr.leWidth);
	return 1;
}

int LuaMMF::FrameGetHeight (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rhFrame->m_hdr.leHeight);
	return 1;
}

int LuaMMF::FrameGetVWidth (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rhLevelSx);
	return 1;
}

int LuaMMF::FrameGetVHeight (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rhLevelSy);
	return 1;
}

// =====================================================================================
//
// Window
//
// =====================================================================================

int LuaMMF::WindowGetWidth (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	RECT rc;
	GetWindowRect(rh->rhHMainWin, &rc);

	lua_pushnumber(L, rc.right - rc.left);
	return 1;
}

int LuaMMF::WindowGetHeight (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	RECT rc;
	GetWindowRect(rh->rhHMainWin, &rc);

	lua_pushnumber(L, rc.bottom - rc.top);
	return 1;
}

int LuaMMF::WindowGetClientWidth (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	RECT rc;
	GetClientRect(rh->rhHMainWin, &rc);

	lua_pushnumber(L, rc.right - rc.left);
	return 1;
}

int LuaMMF::WindowGetClientHeight (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	RECT rc;
	GetClientRect(rh->rhHMainWin, &rc);

	lua_pushnumber(L, rc.bottom - rc.top);
	return 1;
}

int LuaMMF::WindowGetFrameWidth (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	RECT rc;
	GetWindowRect(rh->rhHEditWin, &rc);

	lua_pushnumber(L, rc.right - rc.left);
	return 1;
}

int LuaMMF::WindowGetFrameHeight (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	RECT rc;
	GetWindowRect(rh->rhHEditWin, &rc);

	lua_pushnumber(L, rc.bottom - rc.top);
	return 1;
}

int LuaMMF::WindowGetMainHandle (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushlightuserdata(L, (void*)rh->rhHMainWin);
	return 1;
}

int LuaMMF::WindowGetEditHandle (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushlightuserdata(L, (void*)rh->rhHEditWin);
	return 1;
}

// =====================================================================================
//
// Mouse
//
// =====================================================================================

int LuaMMF::MouseGetX (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rh2.rh2Mouse.x);
	return 1;
}

int LuaMMF::MouseGetY (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rh2.rh2Mouse.y);
	return 1;
}

int LuaMMF::MouseGetWindowX (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rh2.rh2MouseClient.x);
	return 1;
}

int LuaMMF::MouseGetWindowY (lua_State *L) {
	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return 0;
	if (state->rdList.size() == 0 || state->rdList.front() == NULL) return 0;

	LPRH rh = state->rdList.front()->rdPtr->rHo.hoAdRunHeader;

	lua_pushnumber(L, rh->rh2.rh2MouseClient.y);
	return 1;
}

int LuaMMF::MouseIsButtonDown (lua_State *L) {
	if (!LuaMMF::ParamCheck(L, 1)) return 0;

	short state = 0;
	switch (lua_tointeger(L, 1)) {
		case 1: state = GetAsyncKeyState(VK_LBUTTON); break;
		case 2: state = GetAsyncKeyState(VK_MBUTTON); break;
		case 3: state = GetAsyncKeyState(VK_RBUTTON); break;
		case 4: state = GetAsyncKeyState(VK_XBUTTON1); break;
		case 5: state = GetAsyncKeyState(VK_XBUTTON2); break;
	}

	if (state & 0x8000) {
		lua_pushboolean(L, true);
	} else {
		lua_pushboolean(L, false);
	}

	return 1;
}

int LuaMMF::MouseIsButtonUp (lua_State *L) {
	if (!LuaMMF::ParamCheck(L, 1)) return 0;

	short state = 0;
	switch (lua_tointeger(L, 1)) {
		case 1: state = GetAsyncKeyState(VK_LBUTTON); break;
		case 2: state = GetAsyncKeyState(VK_MBUTTON); break;
		case 3: state = GetAsyncKeyState(VK_RBUTTON); break;
		case 4: state = GetAsyncKeyState(VK_XBUTTON1); break;
		case 5: state = GetAsyncKeyState(VK_XBUTTON2); break;
	}

	if (state & 0x8000) {
		lua_pushboolean(L, false);
	} else {
		lua_pushboolean(L, true);
	}

	return 1;
}

int LuaMMF::KeyboardIsKeyDown (lua_State *L) {
	if (!LuaMMF::ParamCheck(L, 1)) return 0;

	int key = lua_tointeger(L, 1);
	short state = GetAsyncKeyState(key);

	if (state & 0x8000) {
		lua_pushboolean(L, true);
	} else {
		lua_pushboolean(L, false);
	}

	return 1;
}

int LuaMMF::KeyboardIsKeyUp (lua_State *L) {
	if (!LuaMMF::ParamCheck(L, 1)) return 0;

	int key = lua_tointeger(L, 1);
	short state = GetAsyncKeyState(key);

	if (state & 0x8000) {
		lua_pushboolean(L, false);
	} else {
		lua_pushboolean(L, true);
	}

	return 1;
}

LPRO LuaMMF::ObjectLookup (lua_State* L, int params) {
	// Do a parameter check first
	int paramCount = lua_gettop(L);
	if (paramCount != params) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return NULL;
	}

	// Get object ID (assumed first item on stack)
	int objId = lua_tointeger(L, 1);

	XLuaState* state = XLuaGlobal::Get().GetStateByState(L);
	if (state == NULL) return NULL;

	LPRO obj = state->GetExport(objId);

	return obj;
}

bool LuaMMF::ParamCheck (lua_State* L, int params) {
	int paramCount = lua_gettop(L);
	if (paramCount != params) {
		lua_pushstring(L, "Invalid parameter count");
		lua_error(L);
		return false;
	}
	return true;
}