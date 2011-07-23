#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "common.h"

class System {
public:

	// Getters

	static int	Value (lua_State* L);
	static int	MinValue (lua_State* L);
	static int	MaxValue (lua_State* L);

	static int	Text (lua_State* L);

	// Setters

	static int	SetValue (lua_State* L);
	static int	SetMinValue (lua_State* L);
	static int	SetMaxValue (lua_State* L);
};

class Counter {
public:

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);
};

const FunctionPair CounterRead[] = {
	{ "maxValue",	System::MaxValue },
	{ "minValue",	System::MinValue },
	{ "value",		System::Value },
};

const FunctionPair CounterWrite[] = {
	{ "maxValue",	System::MaxValue },
	{ "minValue",	System::MinValue },
	{ "value",		System::Value },
};

class Text {
public:

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);
};

const FunctionPair TextRead[] = {
	{ "text",		System::Text },
};

const FunctionPair TextWrite[] = {
	{ 0, 0 },
};

#endif