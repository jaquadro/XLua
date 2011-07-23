#ifndef EXTOBJECT_H_
#define EXTOBJECT_H_

#include "common.h"
#include "ext.h"

class ExtXLuaSurface {
public:

	// Getters

	// Setters

	// Queries

	// Actions

	static int	ReleaseSurface (lua_State* L);
	static int	ReleaseSurfaceFunc (lua_State* L);

	static int	SetSurface (lua_State* L);
	static int	SetSurfaceFunc (lua_State* L);

	static int	Update (lua_State* L);
	static int	UpdateFunc (lua_State* L);

public:

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);
};

const FunctionPair ExtXLuaSurfaceRead[] = {
	{ "releaseSurface",	ExtXLuaSurface::ReleaseSurface },
	{ "setSurface",		ExtXLuaSurface::SetSurface },
	{ "update",			ExtXLuaSurface::Update },
};

const FunctionPair ExtXLuaSurfaceWrite[] = {
	{ 0, 0 },
};


class ExtSurface {
public:

	// Getters

	// Setters

	// Queries

	static int	GetSurface (lua_State* L);
	static int	GetSurfaceFunc (lua_State* L);

	static int	GetSurfaceCount (lua_State* L);
	static int	GetSurfaceCountFunc (lua_State* L);

	static int	CloneSurface (lua_State* L);
	static int	CloneSurfaceFunc (lua_State* L);

	// Actions

	static int	Update (lua_State* L);
	static int	UpdateFunc (lua_State* L);

public:

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);
};

const FunctionPair ExtSurfaceRead[] = {
	{ "cloneSurface",		ExtSurface::CloneSurface },
	{ "getSurface",			ExtSurface::GetSurface },
	{ "getSurfaceCount",	ExtSurface::GetSurfaceCount },
	{ "update",				ExtSurface::Update },
};

const FunctionPair ExtSurfaceWrite[] = {
	{ 0, 0 },
};

#endif
