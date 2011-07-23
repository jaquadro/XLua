#ifndef WINDOW_H_
#define WINDOW_H_

#include "common.h"

class Window {
public:

	static int	NewWindow (lua_State* L);

	// Getters 

	static int	ClientWidth (lua_State* L);
	static int	ClientHeight (lua_State* L);

	static int	FrameWidth (lua_State* L);
	static int	FrameHeight (lua_State* L);
	
	static int	Width (lua_State* L);
	static int	Height (lua_State* L);

public:

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);

	static int	MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc);
};

const FunctionPair WindowRead[] = {
	{ "clientHeight",	Window::ClientHeight },
	{ "clientWidth",	Window::ClientWidth },
	{ "frameWidth",		Window::FrameWidth },
	{ "frameHeight",	Window::FrameHeight },
	{ "height",			Window::Height },
	{ "width",			Window::Width },
};

const FunctionPair WindowWrite[] = {
	{ 0, 0 },
};

#endif
