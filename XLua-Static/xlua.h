#ifndef XLUA_H_
#define XLUA_H_

//---------------------------------------------------------
// This is a basic template for XLua Extension modules.
// Make sure you have rSDK and Lua 5.1.4 in your include
// path.  Make sure you have included xlua.lib and mmfs2.lib
// in your project.
//---------------------------------------------------------

#define COXSDK

#include	"ccxhdr.h"
#include	"cnpdll.h"
#include	"cncr.h"

#include	"lua.hpp"

#define MAKEID(a,b,c,d)				((#@a<<24)|(#@b<<16)|(#@c<<8)|(#@d)) 

class rRundata;
typedef rRundata* LPRRDATA;

LPRO xlua_get_object (lua_State* L, int objid);

#endif
