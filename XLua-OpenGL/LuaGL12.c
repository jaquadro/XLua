#include <string.h>
#include <malloc.h>

#include "lauxlib.h"
#include "LuaGL.h"

/* void glBlendColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
 *
 * BlendColor (red, green, blue, alpha) -> none
 */
int gl_blend_color(lua_State *L)
{
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4)))
		luaL_error(L, "incorrect argument to function 'gl.BlendColor'");

	glBlendColor((GLclampf)lua_tonumber(L, 1), (GLclampf)lua_tonumber(L, 2),
			(GLclampf)lua_tonumber(L, 3), (GLclampf)lua_tonumber(L, 4));

	return 0;
}

/* void glBlendEquation (GLenum mode)
 *
 * BlendEquation (mode) -> none
 */
int gl_blend_equation(lua_State *L)
{
	GLenum a;

	a = (GLenum)get_gl_enum(L, 1);

	if (a == ENUM_ERROR)
		luaL_error(L, "incorrect string argument to function 'gl.BlendEquation'");

	glBlendEquation(a);

	return 0;
}