#ifndef INPUT_H_
#define INPUT_H_

#include "common.h"

class Mouse {
public:

	static int	NewMouse (lua_State* L);

	// Getters 

	static int	ClientX (lua_State* L);
	static int	ClientY (lua_State* L);

	static int	WheelDelta (lua_State* L);

	static int	X (lua_State* L);
	static int	Y (lua_State* L);

	// Queries

	static int	ButtonDown (lua_State* L);
	static int	ButtonDownFunc (lua_State* L);

	static int	ButtonUp (lua_State* L);
	static int	ButtonUpFunc (lua_State* L);

	// Actions

	static int SimulateButtonDown (lua_State* L);
	static int SimulateButtonUp (lua_State* L);

public:

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);

	static int	MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc);
};

const FunctionPair MouseRead[] = {
	{ "buttonDown",		Mouse::ButtonDown },
	{ "buttonUp",		Mouse::ButtonUp },
	{ "clientX",		Mouse::ClientX },
	{ "clientY",		Mouse::ClientY },
	{ "wheelDelta",		Mouse::WheelDelta },
	{ "x",				Mouse::X },
	{ "y",				Mouse::Y },
};

const FunctionPair MouseWrite[] = {
	{ 0, 0 },
};

class Keyboard {
public:

	static int	NewKeyboard (lua_State* L);

	// Queries 

	static int	KeyDown (lua_State* L);
	static int	KeyDownFunc (lua_State* L);

	static int	KeyUp (lua_State* L);
	static int	KeyUpFunc (lua_State* L);

	// Actions

	static int	SimulateKeyDown (lua_State* L);
	static int	SimulateKeyDownFunc (lua_State* L);

	static int	SimulateKeyUp (lua_State* L);
	static int	SimulateKeyUpFunc (lua_State* L);

public:

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);

	static int	MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc);
};

const FunctionPair KeyboardRead[] = {
	{ "keyDown",			Keyboard::KeyDown },
	{ "keyUp",				Keyboard::KeyUp },
	{ "simulateKeyDown",	Keyboard::SimulateKeyDown },
	{ "simulateKeyUp",		Keyboard::SimulateKeyUp },
};

const FunctionPair KeyboardWrite[] = {
	{ 0, 0 },
};

#endif
