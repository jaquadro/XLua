#ifndef KCOBJECT_H_
#define KCOBJECT_H_

#include "common.h"
#include "kc.h"

class KCOArray {
public:

	// Getters

	static int	XDim (lua_State* L);
	static int	YDim (lua_State* L);
	static int	ZDim (lua_State* L);

	static int	XIndex (lua_State* L);
	static int	YIndex (lua_State* L);
	static int	ZIndex (lua_State* L);

	// Setters

	static int	SetXIndex (lua_State* L);
	static int	SetYIndex (lua_State* L);
	static int	SetZIndex (lua_State* L);

	// Queries

	static int	Get (lua_State* L);
	static int	GetFunc (lua_State* L);

	// Actions

	static int	Set (lua_State* L);
	static int	SetFunc (lua_State* L);

	static int	Clear (lua_State* L);
	static int	ClearFunc (lua_State* L);

public:

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);
};

const FunctionPair KCOArrayRead[] = {
	{ "clear",		KCOArray::Clear },
	{ "curX",		KCOArray::XIndex },
	{ "curY",		KCOArray::YIndex },
	{ "curZ",		KCOArray::ZIndex },
	{ "dimX",		KCOArray::XDim },
	{ "dimY",		KCOArray::YDim },
	{ "dimZ",		KCOArray::ZDim },
	{ "get",		KCOArray::Get },
	{ "set",		KCOArray::Set },
};

const FunctionPair KCOArrayWrite[] = {
	{ "curX",		KCOArray::SetXIndex },
	{ "curY",		KCOArray::SetYIndex },
	{ "curZ",		KCOArray::SetZIndex },
};

#endif