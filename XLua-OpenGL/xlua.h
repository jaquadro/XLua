#ifndef XLUA_H_
#define XLUA_H_

//---------------------------------------------------------
// This is a basic template for XLua Extension modules.
// You need to add the following to your include path:
// - includes directory of rSDK or the MMF2 SDK
// - includes directory of Lua 5.1.4
// Similarly you need to add to your lib path the locations
// of xlua.lib and mmfs2.lib, and add those libraries to
// your project.
//---------------------------------------------------------

#include	"ccxhdr.h"
#include	"cnpdll.h"
#include	"cncr.h"

#include	"lua.hpp"

#define MAKEID(a,b,c,d)				((#@a<<24)|(#@b<<16)|(#@c<<8)|(#@d)) 

class rRundata;
typedef rRundata* LPRRDATA;

LPRO xlua_get_object (lua_State* L, int objid);

#endif
