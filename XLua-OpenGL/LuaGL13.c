#include <string.h>
#include <malloc.h>

#include "lauxlib.h"
#include "LuaGL.h"

/* void glActiveTexture (GLenum texture)
 *
 * ActiveTexture (texture) -> none
 */
int gl_active_texture(lua_State *L)
{
	GLenum a;

	a = (GLenum)get_gl_enum(L, 1);

	if (a == ENUM_ERROR)
		luaL_error(L, "incorrect string argument to function 'gl.ActiveTexture'");

	glActiveTexture(a);

	return 0;
}

/* void glClientActiveTexture (GLenum texture)
 *
 * ClientActiveTexture (texture) -> none
 */
int gl_client_active_texture(lua_State *L)
{
	GLenum a;

	a = (GLenum)get_gl_enum(L, 1);

	if (a == ENUM_ERROR)
		luaL_error(L, "incorrect string argument to function 'gl.ClientActiveTexture'");

	glClientActiveTexture(a);

	return 0;
}

/* void glMultiTexCoord4f (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
 *
 * MultiTexCoord (target, s [, t [, r [, q]]]) -> none
 */
int gl_multi_tex_coord(lua_State *L)
{
	GLenum a;
	GLint size = 1;

	if (!(lua_isstring(L, 1) && lua_isnumber(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.MultiTexCoord'");

	a = (GLenum)get_gl_enum(L, 1);

	if (a == ENUM_ERROR)
		luaL_error(L, "incorrect string argument to function 'gl.MultiTexCoord'");

	if (lua_isnumber(L, 3))
		size = 2;
	if (lua_isnumber(L, 4))
		size = 3;
	if (lua_isnumber(L, 5))
		size = 4;

	switch (size) {
		case 1: glMultiTexCoord1f(a, (GLfloat)lua_tonumber(L, 2)); break;
		case 2: glMultiTexCoord2f(a, (GLfloat)lua_tonumber(L, 2), (GLfloat)lua_tonumber(L, 3)); break;
		case 3: glMultiTexCoord3f(a, (GLfloat)lua_tonumber(L, 2), (GLfloat)lua_tonumber(L, 3), 
					(GLfloat)lua_tonumber(L, 4)); break;
		case 4: glMultiTexCoord4f(a, (GLfloat)lua_tonumber(L, 2), (GLfloat)lua_tonumber(L, 3), 
					(GLfloat)lua_tonumber(L, 4), (GLfloat)lua_tonumber(L, 5)); break;
	}

	return 0;
}