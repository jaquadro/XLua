#ifndef FRAME_H_
#define FRAME_H_

#include "common.h"

class Frame {
public:

	static int	NewFrame (lua_State* L);

	// Getters 

	static int	XLeft (lua_State* L);
	static int	XRight (lua_State* L);
	static int	YTop (lua_State* L);
	static int	YBottom (lua_State* L);
	
	static int	Width (lua_State* L);
	static int	Height (lua_State* L);
	
	static int	VWidth (lua_State* L);
	static int	VHeight (lua_State* L);

	// Queries

	static int	TestPoint (lua_State* L);
	static int	TestPointFunc (lua_State* L);

	static int	TestRect (lua_State* L);
	static int	TestRectFunc (lua_State* L);

public:

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);

	static int	MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc);
};

const FunctionPair FrameRead[] = {
	{ "height",			Frame::Height },
	{ "testPoint",		Frame::TestPoint },
	{ "testRect",		Frame::TestRect },
	{ "virtualHeight",	Frame::VHeight },
	{ "virtualWidth",	Frame::VWidth },
	{ "width",			Frame::Width },
	{ "xLeft",			Frame::XLeft },
	{ "xRight",			Frame::XRight },
	{ "yBottom",		Frame::YBottom },
	{ "yTop",			Frame::YTop },
};

const FunctionPair FrameWrite[] = {
	{ 0, 0 },
};

#endif
