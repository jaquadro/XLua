#include <string.h>
#include <malloc.h>

#include "lauxlib.h"
#include "LuaGL.h"

/* void glBlendFuncSeparate (GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)
 *
 * BlendFuncSeparate (srcRGB, destRGB, srcAlpha, destAlpha) -> none
 */
int gl_blend_func_separate(lua_State *L)
{
	GLenum a, b, c, d;

	/* test arguments */
	if(!(lua_isstring(L, 1) && lua_isstring(L, 2) && lua_isstring(L, 3) && lua_isstring(L, 4)))
		luaL_error(L, "incorrect argument to function 'gl.BlendFuncSeparate'");

	/* get values */
	a = (GLenum)get_gl_enum(L, 1);
	b = (GLenum)get_gl_enum(L, 2);
	c = (GLenum)get_gl_enum(L, 3);
	d = (GLenum)get_gl_enum(L, 4);

	/* test arguments */
	if((a == ENUM_ERROR) || (b == ENUM_ERROR) || (c == ENUM_ERROR) || (d == ENUM_ERROR))
		luaL_error(L, "incorrect string argument to function 'gl.BlendFuncSeparate'");

	/* call opengl function */
	glBlendFuncSeparate(a, b, c, d);

	return 0;
}

/* void glFogCoordf (GLfloat coord)
 *
 * FogCoord (cArray) -> none
 */
int gl_fog_coord(lua_State *L)
{
	/* if have there's no arguments show an error message */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.FogCoord'");

	glFogCoordf((GLfloat) lua_tonumber(L, 1));

	return 0;
}

/* void glFogCoordPointer (GLenum type, GLsizei stride, const GLvoid* pointer)
 *
 * FogCoordPointer (colorArray [,stride]]) -> none
 * FogCoordPointer (bufferOffset [,stride]) -> none
 */
int gl_fog_coord_pointer(lua_State *L)
{
	GLint size, offset;
	GLsizei stride = 0;
	static GLfloat *array = 0;

	if(array)
		free(array);

	if (lua_isnumber(L, 1)) {
		offset = lua_tointeger(L, 1);
		
		if (lua_isnumber(L, 2)) {
			stride = lua_tointeger(L, 2);
		}

		glFogCoordPointer(GL_FLOAT, stride * sizeof(GLfloat), (GLubyte*) NULL + offset * sizeof(GLfloat));
	}
	else if (lua_istable(L, 1)) {
		if (get_array2f(L, 1, &array, &size) == -1)
			size = get_arrayf(L, 1, &array) / 3;

		if (lua_isnumber(L, 2)) {
			stride = lua_tointeger(L, 2);
		}

		glFogCoordPointer(GL_FLOAT, stride * sizeof(GLfloat), array);
	}
	else {
		luaL_error(L, "incorrect argument to function 'gl.FogCoordPointer'");
	}

	return 0;
}

/* void glPointParameterf (GLenum pname, GLfloat param)
 *
 * PointParameter (name, param) -> none
 */
int gl_point_parameter(lua_State *L)
{
	GLenum name, p;

	/* if have there's no arguments show an error message */
	if(!(lua_isstring(L, 1)))
		luaL_error(L, "incorrect argument to function 'gl.PointParameter'");

	name = get_gl_enum(L, 1);
	if (name == ENUM_ERROR)
		luaL_error(L, "incorrect string argument to function 'gl.PointParameter'");

	switch (name) {
		case GL_POINT_SIZE_MIN:
		case GL_POINT_SIZE_MAX:
		case GL_POINT_FADE_THRESHOLD_SIZE:
		case GL_POINT_DISTANCE_ATTENUATION:
			if (!lua_isnumber(L, 2))
				luaL_error(L, "incorrect argument to function 'gl.PointParameter'");
			glPointParameterf(name, (GLfloat) lua_tonumber(L, 2));
			break;

		case GL_POINT_SPRITE_COORD_ORIGIN:
			if (!lua_isstring(L, 2))
				luaL_error(L, "incorrect argument to function 'gl.PointParameter'");
			p = get_gl_enum(L, 2);
			if (p == ENUM_ERROR)
				luaL_error(L, "incorrect string argument to function 'gl.PointParameter'");
			glPointParameteri(name, p);
			break;
	}

	return 0;
}

/*SecondaryColor (red, green, blue) -> none
  SecondaryColor (color) -> none*/
int gl_secondary_color(lua_State *L)
{
	GLfloat* array = 0;

	int index;
	int num_args = 0;

	num_args = lua_gettop(L);

	/* test arguments type */
	if(lua_istable(L, 1))
	{
		num_args = get_arrayf(L, 1, &array);

		/* if more then 3 arguments, ignore the others */
		if(num_args > 3)
			num_args = 3;

		/* call openGL functions */
		switch(num_args)
		{
			case 3:  glSecondaryColor3fv(array); break;
		}

		if(array)
			free(array);

		return 0;
	}

	/* if more then 4 arguments, ignore the others */
	if(num_args > 3)
		num_args = 3;

	for(index = 0; index < num_args; index++)
	{
		if(!lua_isnumber(L, index + 1))
			luaL_error(L, "incorrect argument to function 'gl.SecondaryColor'");
	}

	/* call openGL functions */
	switch(num_args)
	{
		case 3:  glSecondaryColor3f((GLfloat)lua_tonumber(L, 1), (GLfloat)lua_tonumber(L, 2),
								 (GLfloat)lua_tonumber(L, 3));
					break;
	}
	return 0;
}

/* void glSecondaryColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
 *
 * SecondaryColorPointer (colorArray [,size [,stride]]) -> none
 * SecondaryColorPointer (bufferOffset, size [,stride]) -> none
 */
int gl_secondary_color_pointer(lua_State *L)
{
	GLint size, offset;
	GLsizei stride = 0;
	static GLfloat *array = 0;

	if(array)
		free(array);

	if (lua_isnumber(L, 1)) {
		offset = lua_tointeger(L, 1);
		if (!lua_isnumber(L, 2)) {
			luaL_error(L, "incorrect argument to function 'gl.SeconadryColorPointer'");
		}
		size = lua_tointeger(L, 2);
		
		if (lua_isnumber(L, 3)) {
			stride = lua_tointeger(L, 3);
		}

		glSecondaryColorPointer(size, GL_FLOAT, stride * sizeof(GLfloat), (GLubyte*) NULL + offset * sizeof(GLfloat));
	}
	else if (lua_istable(L, 1)) {
		if(lua_isnumber(L, 2))
		{
		  size = (GLint)lua_tonumber(L, 2);
		  get_arrayf(L, 1, &array);
		}
		else if(get_array2f(L, 1, &array, &size) == -1)
		{
			luaL_error(L, "incorrect argument to function 'gl.SecondaryColorPointer'");
			return 0;
		}

		if (lua_isnumber(L, 3)) {
			stride = lua_tointeger(L, 3);
		}

		glSecondaryColorPointer(size, GL_FLOAT, stride * sizeof(GLfloat), array);
	}
	else {
		luaL_error(L, "incorrect argument to function 'gl.SecondaryColorPointer'");
	}

	return 0;
}

/* void glWindowPos2f (GLfloat x, GLfloat y)
 *
 * WindowPos (x, y [, z]) -> none
 * WindowPos (pos) -> none
 */
int gl_window_pos(lua_State *L)
{
	GLfloat *array = 0;

	int index;
	int num_args = lua_gettop(L);

	/* test arguments type */
	if(lua_istable(L, 1))
	{
		num_args = get_arrayf(L, 1, &array);

		/* if more then 3 arguments, ignore the others */
		if(num_args > 3)
			num_args = 3;

		/* call openGL functions */
		switch(num_args)
		{
			case 2:  glWindowPos2fv(array); break;
			case 3:  glWindowPos3fv(array); break;
		}

		if(array)
			free(array);

		return 0;
	}

	/* if more then 3 arguments, ignore the others */
	if(num_args > 3)
		num_args = 3;

	for(index = 0; index < num_args; index++)
	{
		if(!lua_isnumber(L, index + 1))
			luaL_error(L, "incorrect argument to function 'gl.WindowPos'");
	}

	/* call openGL functions */
	switch(num_args)
	{
		case 2:  glWindowPos2f((GLfloat)lua_tonumber(L, 1), (GLfloat)lua_tonumber(L, 2));
					break;
		case 3:  glWindowPos3f((GLfloat)lua_tonumber(L, 1), (GLfloat)lua_tonumber(L, 2),
								 (GLfloat)lua_tonumber(L, 3));
					break;
	}
	return 0;
}
