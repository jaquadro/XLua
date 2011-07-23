/*************************************************
*  LuaGL - an OpenGL binding for Lua
*  2003-2004(c) Fabio Guerra, Cleyde Marlyse
*  www.luagl.sourceforge.net
*-------------------------------------------------
*  Description: This file implements the OpenGL
*					binding for Lua 5.0.
*-------------------------------------------------
*  Last Update: 14/07/2004
*  Version: v1.01
*-------------------------------------------------
*  See Copyright Notice in LuaGL.h
*************************************************/

#include <string.h>
#include <malloc.h>

#include "lauxlib.h"

#define BUILDING_LUAGL_DLL

//#include "LuaGLExt.h"
#include "LuaGL.h"

/* set field of a lua table with a number */
void set_field(lua_State *L, unsigned int index, lua_Number value)
{
	lua_pushnumber(L, index);
	lua_pushnumber(L, value);
	lua_settable(L, -3);
}

GLenum get_enum(const char *str, int n)
{
	int i = 0;

	while(gl_str[i].str != 0)
	{
		if(strncmp(str, gl_str[i].str, n) == 0 && gl_str[i].str[n] == 0)
			return gl_str[i].value;

		i++;
	}
	return ENUM_ERROR;
}
GLenum get_gl_enum(lua_State *L, int index)
{
	unsigned int i;
	const char *str = lua_tostring(L, index);
	GLenum temp = 0, ret = 0;

	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] == ',')
		{
			temp = get_enum(str, i);
			if(temp != ENUM_ERROR)
				ret |= temp;

			str += i+1;
			i = 0;
		}
	}
	temp = get_enum(str, strlen(str));

	if(temp == ENUM_ERROR)
	{
		if(ret == 0)
			return ENUM_ERROR;
		return ret;
	}

	return ret | temp;
}

const char *get_str_gl_enum(GLenum num)
{
	unsigned int i = 0;

	while(gl_str[i].str != 0)
	{
		if(num == gl_str[i].value)
			return gl_str[i].str;

		i++;
	}
	return NULL;
}

/* Gets an array from a lua table, store it in 'array' and returns the no. of elems of the array
	index refers to where the table is in stack. */
int get_arrayb(lua_State *L, int index, GLboolean **array)
{
	int i;
	int n = luaL_getn(L, index);

	*array = (GLboolean *)malloc(n * sizeof(GLboolean));
	for(i = 0; i < n; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*array)[i] = (GLboolean)lua_toboolean(L, -1);
		lua_pop(L, 1);
	}

	return n; /* return the number of valid elements found.*/
}

int get_arrayd(lua_State *L, int index, GLdouble **array)
{
	int i;
	int n = luaL_getn(L, index);

	*array = (GLdouble *)malloc(n * sizeof(GLdouble));

	for(i = 0; i < n; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*array)[i] = (GLdouble)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	return n; /* return the number of valid elements found.*/
}

int get_arrayf(lua_State *L, int index, GLfloat **array)
{
	int i;
	int n = luaL_getn(L, index);

	*array = (GLfloat *)malloc(n * sizeof(GLfloat));

	for(i = 0; i < n; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*array)[i] = (GLfloat)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	return n; /* return the number of valid elements found.*/
}

int get_arrayui(lua_State *L, int index, GLuint **array)
{
	int i;
	int n = luaL_getn(L, index);

	*array = (GLuint *)malloc(n * sizeof(GLint));

	for(i = 0; i < n; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*array)[i] = (GLuint)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	return n; /* return the number of valid elements found.*/
}

int get_arrayubyte(lua_State *L, int index, GLubyte **array)
{
	int i;
	int n = luaL_getn(L, index);

	*array = (GLubyte *)malloc(n * sizeof(GLubyte));

	for(i = 0; i < n; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*array)[i] = (GLubyte)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	return n; /* return the number of valid elements found.*/
}

int get_array2ubyte(lua_State *L, int index, GLubyte **array, int *size)
{
	int i, j;
	int n = luaL_getn(L, index);

	lua_rawgeti(L, index, 1);

	if(!lua_istable(L, -1))
	{
		lua_remove(L, -1);
		return -1;
	}

	*size = luaL_getn(L, -1);

	*array = (GLubyte *)malloc(n * (*size) * sizeof(GLubyte));

	for(i = 0; i < n; i++)
	{
		lua_rawgeti(L, index, i+1);

		if(!lua_istable(L, -1))
			return -1;

		for(j = 0; j < *size; j++)
		{
			lua_rawgeti(L, -1, j + 1);

			(*array)[i*(*size) + j] = (GLubyte)lua_tonumber(L, -1);

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return n; /* return the number of valid elements found.*/
}

int get_array2i(lua_State *L, int index, GLint **array, int *size)
{
	int i, j;
	int n = luaL_getn(L, index);

	lua_rawgeti(L, index, 1);

	if(!lua_istable(L, -1))
	{
		lua_remove(L, -1);
		return -1;
	}

	*size = luaL_getn(L, -1);

	*array = (GLint *)malloc(n * (*size) * sizeof(GLint));

	for(i = 0; i < n; i++)
	{
		lua_rawgeti(L, index, i+1);

		if(!lua_istable(L, -1))
			return -1;

		for(j = 0; j < *size; j++)
		{
			lua_rawgeti(L, -1, j + 1);

			(*array)[i*(*size) + j] = (GLint)lua_tointeger(L, -1);

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return n; /* return the number of valid elements found.*/
}

int get_array2ui(lua_State *L, int index, GLuint **array, int *size)
{
	int i, j;
	int n = luaL_getn(L, index);

	lua_rawgeti(L, index, 1);

	if(!lua_istable(L, -1))
	{
		lua_remove(L, -1);
		return -1;
	}

	*size = luaL_getn(L, -1);

	*array = (GLuint *)malloc(n * (*size) * sizeof(GLuint));

	for(i = 0; i < n; i++)
	{
		lua_rawgeti(L, index, i+1);

		if(!lua_istable(L, -1))
			return -1;

		for(j = 0; j < *size; j++)
		{
			lua_rawgeti(L, -1, j + 1);

			(*array)[i*(*size) + j] = (GLuint)lua_tointeger(L, -1);

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return n; /* return the number of valid elements found.*/
}

int get_array2d(lua_State *L, int index, GLdouble **array, int *size)
{
	int i, j;
	int n = luaL_getn(L, index);

	lua_rawgeti(L, index, 1);

	if(!lua_istable(L, -1))
	{
		lua_remove(L, -1);
		return -1;
	}

	*size = luaL_getn(L, -1);

	*array = (GLdouble *)malloc(n * (*size) * sizeof(GLdouble));

	for(i = 0; i < n; i++)
	{
		lua_rawgeti(L, index, i+1);

		if(!lua_istable(L, -1))
			return -1;

		for(j = 0; j < *size; j++)
		{
			lua_rawgeti(L, -1, j + 1);

			(*array)[i*(*size) + j] = (GLdouble)lua_tonumber(L, -1);

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return n; /* return the number of valid elements found.*/
}

int get_array2f(lua_State *L, int index, GLfloat **array, int *size)
{
	int i, j;
	int n = luaL_getn(L, index);

	lua_rawgeti(L, index, 1);

	if(!lua_istable(L, -1))
	{
		lua_remove(L, -1);
		return -1;
	}

	*size = luaL_getn(L, -1);

	*array = (GLfloat *)malloc(n * (*size) * sizeof(GLfloat));

	for(i = 0; i < n; i++)
	{
		lua_rawgeti(L, index, i+1);

		if(!lua_istable(L, -1))
			return -1;

		for(j = 0; j < *size; j++)
		{
			lua_rawgeti(L, -1, j + 1);

			(*array)[i*(*size) + j] = (GLfloat)lua_tonumber(L, -1);

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return n; /* return the number of valid elements found.*/
}

int get_array3f(lua_State *L, int index, GLfloat **array, int *outerSize, int *innerSize)
{
	int i, j, k;
	int n = luaL_getn(L, index);

	lua_rawgeti(L, index, 1);

	if(!lua_istable(L, -1))
	{
		lua_remove(L, -1);
		return -1;
	}

	*outerSize = luaL_getn(L, -1);
	lua_rawgeti(L, -1, 1);

	if (!lua_istable(L, -1))
	{
		lua_remove(L, -1);
		return -1;
	}

	*innerSize = luaL_getn(L, -1);

	*array = (GLfloat *)malloc(n * (*outerSize) * (*innerSize) * sizeof(GLfloat));

	for(i = 0; i < n; i++)
	{
		lua_rawgeti(L, index, i + 1);

		if(!lua_istable(L, -1))
			return -1;

		for(j = 0; j < *outerSize; j++)
		{
			lua_rawgeti(L, -1, j + 1);

			if (!lua_istable(L, -1))
				return -1;

			for (k = 0; k < *innerSize; k++)
			{
				lua_rawgeti(L, -1, k + 1);

				(*array)[i*(*outerSize) + j*(*innerSize) + k] = (GLfloat)lua_tonumber(L, -1);

				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return n; /* return the number of valid elements found.*/
}

int str2mask(const char *str)
{
	int i, j;
	int mask = 0;
	int size = strlen(str);
	for(i = 0, j = 0; j < size; i++)
	{
		if(str[i] == '1')
		{
			mask |= (1 << (size-1-j));
			j++;
		}
		else if(str[i] == '0')
			j++;
			
	}
	return mask;
}

const char *mask2str(int mask)
{
	unsigned int i;
	static char str[17];
	for(i = 0; i < 16; i++)
	{
		if(mask & (1 << (15 - i)))
			str[i] = '1';
		else
			str[i] = '0';
	}
	str[i] = 0;
	return str;
}

/*Accum (op, value) -> none*/
static int gl_accum(lua_State *L)
{
	/* get string parameters */
	//GLenum e;

	/* test argument */
	if(!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		luaL_error(L, "incorrect argument to function 'gl.Accum'");

	//e = get_gl_enum(L, 1);

	/* test arguments */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.Accum'");

	//if(!lua_isnumber(L, 2))
	//	luaL_error(L, "incorrect argument to function 'gl.Accum'");

	/* call opengl function */
	glAccum((GLenum)lua_tointeger(L, 1), (GLfloat)lua_tonumber(L, 2));

	return 0;
}

/*AlphaFunc (func, ref) -> none*/
static int gl_alpha_func(lua_State *L)
{
	/* get string parameters */
	//GLenum e;

	/* test argument */
	if(!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		luaL_error(L, "incorrect argument to function 'gl.AlphaFunc'");

	//e = get_gl_enum(L, 1);

	/* test arguments */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.AlphaFunc'");

	/* call opengl function */
	glAlphaFunc((GLenum)lua_tointeger(L, 1), (GLclampf)lua_tonumber(L, 2));

	return 0;
}

/*AreTexturesResident (texturesArray) -> residences*/
static int gl_are_textures_resident(lua_State *L)
{
	GLboolean *residences;
	GLuint *textures;

	int i, n;

	/* test argument */
	if(!lua_istable(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.AreTexturesResident'");

	/* get textures array */
	n = get_arrayui(L, 1, &textures);

	residences = (GLboolean *)malloc(n * sizeof(GLboolean));

	/* call opengl function */
	glAreTexturesResident(n, (GLuint *)textures, residences);

	lua_newtable(L);

	/* return residences values */
	for(i = 0; i < n; i++)
		set_field(L, i+1, residences[i]);

	free(textures);
	free(residences);

	return 1;
}

/*ArrayElement (i) -> none*/
static int gl_array_element(lua_State *L)
{
	/* test argument */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.ArrayElement'");

	/* call opengl function */
	glArrayElement((GLint)lua_tonumber(L, 1));

	return 0;
}

/*Begin (mode) -> none*/
static int gl_begin(lua_State *L)
{
	//GLenum e;

	/* test argument */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.Begin'");

	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.Begin'");

	/* call opengl function */
	glBegin((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*BindTexture (target, texture) -> none*/
static int gl_bind_texture(lua_State *L)
{
	//GLenum e;

	/* test arguments */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.BindTexture'");

	/* get string value */
	//e = get_gl_enum(L, 1);

	/* test arguments */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.BindTexture'");

	/* call opengl function */
	glBindTexture((GLenum)lua_tonumber(L, 1), (GLuint)lua_tonumber(L, 2));

	return 0;
}

/*Bitmap (xorig, yorig, xmove, ymove, bitmap) -> none*/
static int gl_bitmap(lua_State *L)
{
	int width, height;

	GLubyte *bitmap;

	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
			lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_istable(L,5) ))
		luaL_error(L, "incorrect argument to function 'gl.Bitmap'");

	if((height = get_array2ubyte(L, 4, &bitmap, &width)) == -1)
		luaL_error(L, "incorrect argument to function 'gl.Bitmap'");

	glBitmap(width, height, (GLfloat)lua_tonumber(L, 1), (GLfloat)lua_tonumber(L, 2),
				(GLfloat)lua_tonumber(L, 3), (GLfloat)lua_tonumber(L, 4), bitmap);
	return 0;
}

/*BlendFunc (sfactor, dfactor) -> none*/
static int gl_blend_func(lua_State *L)
{
	//GLenum a, b;

	/* test arguments */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.BlendFunc'");

	/* get values */
	//a = (GLenum)get_gl_enum(L, 1);
	//b = (GLenum)get_gl_enum(L, 2);

	/* test arguments */
	//if((a == ENUM_ERROR) || (b == ENUM_ERROR))
	//	luaL_error(L, "incorrect string argument to function 'gl.BlendFunc'");

	/* call opengl function */
	glBlendFunc((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2));

	return 0;
}

/*CallList (list) -> none*/
static int gl_call_list(lua_State *L)
{
	/* test argument */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.CallList'");

	/* call opengl function */
	glCallList((GLuint)lua_tonumber(L, 1));

	return 0;
}

/*CallLists (listArray) -> none*/
static int gl_call_lists(lua_State *L)
{
	GLsizei n;
	GLfloat *lists;

	/* test argument */
	if(!lua_istable(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.CallLists'");

	/* get array of lists */
	n = get_arrayf(L, 1, &lists);

	/* call opengl function */
	glCallLists(n, GL_FLOAT, lists);

	free(lists);

	return 0;
}

/*Clear (mask) -> none*/
static int gl_clear(lua_State *L)
{
	//GLenum e;
	int i, paramCount;
	GLbitfield accum = 0;

	paramCount = lua_gettop(L);
	for (i = 0; i < paramCount; i++) {
		if (!lua_isnumber(L, i + 1))
			luaL_error(L, "incorrect argument to function 'gl.Clear'");
		accum |= lua_tointeger(L, i + 1);
	}

	/* test argument type */
	//if(!lua_isstring(L, 1))
	//	luaL_error(L, "incorrect argument to function 'gl.Clear'");

	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.Clear'");

	/* call opengl function */
	glClear(accum);

	return 0;
}

/*ClearAccum (red, green, blue, alpha) -> none*/
static int gl_clear_accum(lua_State *L)
{
	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) ))
		luaL_error(L, "incorrect argument to function 'gl.ClearAccum'");

	/* call opengl function */
	glClearAccum((GLfloat)lua_tonumber(L, 1), (GLfloat)lua_tonumber(L, 2),
					 (GLfloat)lua_tonumber(L, 3), (GLfloat)lua_tonumber(L, 4));

	return 0;
}

/*ClearColor (red, green, blue, alpha) -> none*/
static int gl_clear_color(lua_State *L)
{
	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) ))
		luaL_error(L, "incorrect argument to function 'gl.ClearColor'");

	/* call opengl function */
	glClearColor((GLclampf)lua_tonumber(L, 1), (GLclampf)lua_tonumber(L, 2),
					 (GLclampf)lua_tonumber(L, 3), (GLclampf)lua_tonumber(L, 4));

	return 0;
}

/*ClearDepth (depth) -> none*/
static int gl_clear_depth(lua_State *L)
{
	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.ClearDepth'");

	/* call opengl function */
	glClearDepth((GLclampd)lua_tonumber(L, 1));

	return 0;
}

/*ClearIndex (c) -> none*/
static int gl_clear_index(lua_State *L)
{
	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.ClearIndex'");

	/* call opengl function */
	glClearIndex((GLfloat)lua_tonumber(L, 1));

	return 0;
}

/*ClearStencil (s) -> none*/
static int gl_clear_stencil(lua_State *L)
{
	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.ClearStencil'");

	/* call opengl function */
	glClearStencil((GLint)lua_tonumber(L, 1));

	return 0;
}

/*ClipPlane (plane, equationArray) -> none*/
static int gl_clip_plane(lua_State *L)
{
	//GLenum plane;
	GLdouble *equation;

	/* test arguments */
	if(!lua_isnumber(L, 1) || !lua_istable(L, 2))
		luaL_error(L, "incorrect argument to function 'gl.ClipPlane'");

	//if(!lua_istable(L, 2))
	//	luaL_error(L, "incorrect argument to function 'gl.ClipPlane'");

	/* get values */
	//plane = get_gl_enum(L, 1);

	/* test argument */
	//if(plane == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.ClipPlane'");

	/* get array of equations */
	get_arrayd(L, 2, &equation);

	/* call opengl function */
	glClipPlane((GLenum)lua_tointeger(L, 1), equation);

	free(equation);

	return 0;
}

/*Color (red, green, blue [, alpha]) -> none
  Color (color) -> none*/
static int gl_color(lua_State *L)
{
	GLdouble *array = 0;

	int index;
	int num_args = lua_gettop(L);

	/* test arguments type */
	if(lua_istable(L, 1))
	{
		num_args = get_arrayd(L, 1, &array);

		/* if more then 4 arguments, ignore the others */
		if(num_args > 4)
			num_args = 4;

		/* call openGL functions */
		switch(num_args)
		{
			case 3:  glColor3dv(array); break;
			case 4:  glColor4dv(array); break;
		}

		if(array)
			free(array);

		return 0;
	}

	/* if more then 4 arguments, ignore the others */
	if(num_args > 4)
		num_args = 4;

	for(index = 0; index < num_args; index++)
	{
		if(!lua_isnumber(L, index + 1))
			luaL_error(L, "incorrect argument to function 'gl.Color'");
	}

	/* call openGL functions */
	switch(num_args)
	{
		case 3:  glColor3d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
								 (GLdouble)lua_tonumber(L, 3));
					break;
		case 4:  glColor4d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
								 (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4));
					break;
	}
	return 0;
}

/*ColorMask (red, green, blue, alpha) -> none*/
static int gl_color_mask(lua_State *L)
{
	/* test arguments type */
	if(!( lua_isboolean(L, 1) && lua_isboolean(L, 2) && lua_isboolean(L, 3) && lua_isboolean(L, 4) ))
		luaL_error(L, "incorrect argument to function 'gl.ColorMask'");

	glColorMask((GLboolean)lua_toboolean(L, 1), (GLboolean)lua_toboolean(L, 2),
					(GLboolean)lua_toboolean(L, 3), (GLboolean)lua_toboolean(L, 4));

	return 0;
}

/*ColorMaterial (face, mode) -> none*/
static int gl_color_material(lua_State *L)
{
	//GLenum e1, e2;

	/* test arguments */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.ColorMaterial'");

	/* get string parameters */
	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);

	/* test strings */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.ColorMaterial'");

	/* call opengl function */
	glColorMaterial((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2));

	return 0;
}

/* void glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
 *
 * ColorPointer (colorArray [,size [,stride]]) -> none
 * ColorPointer (bufferOffset, size [,stride]) -> none
 */
static int gl_color_pointer(lua_State *L)
{
	GLint size, offset;
	GLsizei stride = 0;
	static GLfloat *array = 0;

	if(array)
		free(array);

	if (lua_isnumber(L, 1)) {
		offset = lua_tointeger(L, 1);
		if (!lua_isnumber(L, 2)) {
			luaL_error(L, "incorrect argument to function 'gl.ColorPointer'");
		}
		size = lua_tointeger(L, 2);
		
		if (lua_isnumber(L, 3)) {
			stride = lua_tointeger(L, 3);
		}

		glColorPointer(size, GL_FLOAT, stride * sizeof(GLfloat), (GLubyte*) NULL + offset * sizeof(GLfloat));
	}
	else if (lua_istable(L, 1)) {
		if(lua_isnumber(L, 2))
		{
		  size = (GLint)lua_tonumber(L, 2);
		  get_arrayf(L, 1, &array);
		}
		else if(get_array2f(L, 1, &array, &size) == -1)
		{
			luaL_error(L, "incorrect argument to function 'gl.ColorPointer'");
			return 0;
		}

		if (lua_isnumber(L, 3)) {
			stride = lua_tointeger(L, 3);
		}

		glColorPointer(size, GL_FLOAT, stride * sizeof(GLfloat), array);
	}
	else {
		luaL_error(L, "incorrect argument to function 'gl.ColorPointer'");
	}

	return 0;
}

/*CopyPixels (x, y, width, height, type) -> none*/
static int gl_copy_pixels(lua_State *L)
{
	//GLenum e;

	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
			lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5) ))
		luaL_error(L, "incorrect argument to function 'gl.CopyPixels'");

	/* get string parameter */
	//e = get_gl_enum(L, 5);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.CopyPixels'");

	/* call opengl function */
	glCopyPixels((GLint)  lua_tonumber(L, 1), (GLint)  lua_tonumber(L, 2),
					 (GLsizei)lua_tonumber(L, 3), (GLsizei)lua_tonumber(L, 4), (GLenum)lua_tointeger(L, 5));

	return 0;
}

/*CullFace (mode) -> none*/
static int gl_cull_face(lua_State *L)
{
	//GLenum e;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.CullFace'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.CullFace'");

	/* call opengl function */
	glCullFace((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*DeleteLists (list, range) -> none*/
static int gl_delete_lists(lua_State *L)
{
	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.DeleteLists'");

	/* call opengl function */
	glDeleteLists((GLuint)lua_tonumber(L, 1), (GLsizei)lua_tonumber(L, 2));

	return 0;
}

/*DeleteTextures (texturesArray) -> none*/
static int gl_delete_textures(lua_State *L)
{
	int n;
	GLuint *textures;

	/* test argument type */
	if(!lua_istable(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.DeleteTextures'");

	/* get textures array */
	n = get_arrayui(L, 1, &textures);

	/* call opengl function */
	glDeleteTextures((GLsizei)n, (GLuint *)textures);

	free(textures);

	return 0;
}

/*DepthFunc (func) -> none*/
static int gl_depth_func(lua_State *L)
{
	//GLenum e;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.DepthFunc'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.DepthFunc'");

	/* call opengl function */
	glDepthFunc((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*DepthMask (flag) -> none*/
static int gl_depth_mask(lua_State *L)
{
	/* test argument type */
	if(!lua_isboolean(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.DepthMask'");

	/* call opengl function */
	glDepthMask((GLboolean)lua_toboolean(L, 1));

	return 0;
}

/*DepthRange (zNear, zFar) -> none*/
static int gl_depth_range(lua_State *L)
{
	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.DepthRange'");

	/* call opengl function */
	glDepthRange((GLclampd)lua_tonumber(L, 1), (GLclampd)lua_tonumber(L, 2));

	return 0;
}

/*Disable (cap) -> none*/
static int gl_disable(lua_State *L)
{
	//GLenum e;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.Disable'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.Disable'");

	/* call opengl function */
	glDisable((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*DisableClientState (array) -> none*/
static int gl_disable_client_state(lua_State *L)
{
	//GLenum e;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.DisableClientState'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.DisableClientState'");

	/* call opengl function */
	glDisableClientState((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*DrawArrays (mode, first, count) -> none*/
static int gl_draw_arrays(lua_State *L)
{
	//GLenum e;

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
		luaL_error(L, "incorrect argument to function 'gl.DrawArrays'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.DrawArrays'");

	/* call opengl function */
	glDrawArrays((GLenum)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLsizei)lua_tointeger(L, 3));

	return 0;
}

/*DrawBuffer (mode) -> none*/
static int gl_draw_buffer(lua_State *L)
{
	//GLenum e;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.DrawBuffer'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.DrawBuffer'");

	/* call opengl function */
	glDrawBuffer((GLenum)lua_tointeger(L, 1));

	return 0;
}

/* void glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
 *
 * DrawElements (mode, indicesArray [,count]) -> none 
 * DrawElements (mode, bufferOffset, count) -> none
 */
static int gl_draw_elements(lua_State *L)
{
	int n, offset, size = 1;
	GLuint *indices;
	//GLenum e;

	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.DrawElements'");

	/* get parameters */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.DrawElements'");

	if (lua_isnumber(L, 2)) {
		offset = lua_tointeger(L, 2);
		if (!lua_isnumber(L, 3))
			luaL_error(L, "incorrect string argument to function 'gl.DrawElements'");
		n = lua_tointeger(L, 3);

		glDrawElements((GLenum)lua_tointeger(L, 1), n, GL_UNSIGNED_INT, (GLubyte*) NULL + offset * sizeof(GLuint));
	}
	else if (lua_istable(L, 2)) {
		n = get_array2ui(L, 2, &indices, &size);
		if (n == -1) {
			n = get_arrayui(L, 2, &indices);
			if (n == -1)
				luaL_error(L, "incorrect argument to function 'gl.DrawElements'");
		}

		if (lua_isnumber(L, 3) && n > lua_tointeger(L, 3))
			n = lua_tointeger(L, 3);

		glDrawElements((GLenum)lua_tointeger(L, 1), n * size, GL_UNSIGNED_INT, indices);

		free(indices);
	}
	else {
		luaL_error(L, "incorrect argument to function 'gl.DrawElements'");
	}

	return 0;
}


/*EdgeFlag (flag) -> none*/
static int gl_edge_flag(lua_State *L)
{
	GLboolean *flag;

	if(lua_istable(L, 1))/* test argument type */
	{
		/* get argument */
		get_arrayb(L, 1, &flag);

		/* call opengl function */
		glEdgeFlagv((GLboolean *)flag);

		free(flag);
	}
	else if(lua_isboolean(L, 1))/* test argument type */
		/* call opengl function */
		glEdgeFlag((GLboolean)lua_toboolean(L, 1));

	else
		luaL_error(L, "incorrect argument to function 'gl.EdgeFlag'");

	return 0;
}

/* void glEdgeFlagPointer (GLsizei stride, const GLvoid* pointer)
 *
 * EdgeFlagPointer (flagsArray [,stride]]) -> none
 * EdgeFlagPointer (bufferOffset [,stride]) -> none
 */
static int gl_edge_flag_pointer(lua_State *L)
{
	GLint offset;
	GLsizei stride = 0;
	static GLboolean *flags = 0;

	if(flags)
		free(flags);

	if (lua_isnumber(L, 1)) {
		offset = lua_tointeger(L, 1);
		if (!lua_isnumber(L, 2)) {
			luaL_error(L, "incorrect argument to function 'gl.EdgeFlagPointer'");
		}
		
		if (lua_isnumber(L, 2)) {
			stride = lua_tointeger(L, 2);
		}

		glEdgeFlagPointer(stride * sizeof(GLfloat), (GLubyte*) NULL + offset * sizeof(GLbyte));
	}
	else if (lua_istable(L, 1)) {
		get_arrayb(L, 1, &flags);
		if (lua_isnumber(L, 2)) {
			stride = lua_tointeger(L, 2);
		}

		glEdgeFlagPointer(stride * sizeof(GLfloat), flags);
	}
	else {
		luaL_error(L, "incorrect argument to function 'gl.EdgeFlagPointer'");
	}

	return 0;
}

/*Enable (cap) -> none*/
static int gl_enable(lua_State *L)
{
	//GLenum e;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.Enable'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.Enable'");

	/* call opengl function */
	glEnable((GLenum)lua_tointeger(L, 1));
	return 0;
}

/*EnableClientState (array) -> none*/
static int gl_enable_client_state(lua_State *L)
{
	//GLenum e;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.EnableClientState'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.EnableClientState'");

	/* call opengl function */
	glEnableClientState((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*End () -> none*/
static int gl_end(lua_State *L)
{
	glEnd();
	return 0;
}

/*EndList () -> none*/
static int gl_end_list(lua_State *L)
{
	glEndList();
	return 0;
}

/*EvalCoord (u[, v]) -> none
  EvalCoord (coordArray) -> none*/
static int gl_eval_coord(lua_State *L)
{
	GLdouble *array;

	int index;
	int num_args = lua_gettop(L);

	/* test arguments type */
	if(lua_istable(L, 1))
	{
		/* get_array and return no of elements */
		if(get_arrayd(L, 1, &array) == 1)
		{
			glEvalCoord1dv(array);
			return 0;
		}
		else
			glEvalCoord2dv(array);

		free(array);

		return 0;
	}

	/* if more then 2 arguments, ignore the others */
	if(num_args > 2)
		num_args = 2;

	/* test arguments */
	for(index = 0; index < num_args; index++)
		if(!lua_isnumber(L, index + 1))
			luaL_error(L, "incorrect argument to function 'gl.EvalCoord'");

	/* call openGL functions */
	switch(num_args)
	{
		case 1:  glEvalCoord1d((GLdouble)lua_tonumber(L, 1));
					break;
		case 2:  glEvalCoord2d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2));
					break;
	}
	return 0;
}

/*EvalMesh (mode, i1, i2[,j1, j2]) -> none*/
static int gl_eval_mesh(lua_State *L)
{
	//GLenum e;

	int index;
	int num_args = lua_gettop(L);

	/* test arguments type */
	if(!( lua_isnumber(L, 1) && num_args > 2))
		luaL_error(L, "incorrect argument to function 'gl.EvalMesh'");

	for(index = 2; index < num_args; index++)
		if(!lua_isnumber(L, index + 1))
			luaL_error(L, "incorrect argument to function 'gl.EvalMesh'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.EvalMesh'");

	/* call opengl function */
	if(num_args < 5)
		glEvalMesh1((GLenum)lua_tointeger(L, 1), (GLint)lua_tonumber(L, 2), (GLint)lua_tonumber(L, 3));
	else
		glEvalMesh2((GLenum)lua_tointeger(L, 1), (GLint)lua_tonumber(L, 2), (GLint)lua_tonumber(L, 3),
							(GLint)lua_tonumber(L, 4), (GLint)lua_tonumber(L, 5));

	return 0;
}

/*EvalPoint (i[, j]) -> none*/
static int gl_eval_point(lua_State *L)
{
	int index;
	int num_args = lua_gettop(L);

	/* if more then 2 arguments, ignore the others */
	if(num_args > 2)
		num_args = 2;

	/* test arguments */
	for(index = 0; index < num_args; index++)
		if(!lua_isnumber(L, index + 1))
			luaL_error(L, "incorrect argument to function 'gl.EvalPoint'");

	/* call openGL functions */
	if(num_args == 1)
		glEvalPoint1((GLint)lua_tonumber(L, 1));
	else
		glEvalPoint2((GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 2));

	return 0;
}

/*FeedbackBuffer (size, type) -> dataArray*/
static int gl_feedback_buffer(lua_State *L)
{
	GLfloat *array;
	//GLenum e;
	GLsizei size;
	int i;

	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.FeedbackBuffer'");

	/* get parameters */
	size = (GLsizei)lua_tonumber(L, 1);
	//e = get_gl_enum(L, 2);

	array = (GLfloat *)malloc(size * sizeof(GLfloat));

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.FeedbackBuffer'");

	/* call opengl function */
	glFeedbackBuffer (size, (GLenum)lua_tointeger(L, 2), array);

	lua_newtable(L);

	for(i = 0; i < size; i++)
		set_field(L, i+1, array[i]);

	free(array);

	return 0;
}

/*Finish () -> none*/
static int gl_finish(lua_State *L)
{
	glFinish();
	return 0;
}

/*Flush () -> none*/
static int gl_flush(lua_State *L)
{
	glFlush();
	return 0;
}

/*Fog (pname, param) -> none
  Fog (pname, paramsArray) -> none*/
static int gl_fog(lua_State *L)
{
	GLenum e;
	GLfloat *param;

	/* test first argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.Fog'");

	/* get string parameter */
	e = lua_tointeger(L, 1); //get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.Fog'");

	switch (e)
	{
		case GL_FOG_MODE:
		case GL_FOG_INDEX:
		case GL_FOG_COORD_SRC:
			if (!lua_isnumber(L, 2))
				luaL_error(L, "incorrect argument to function 'gl.Fog'");

			glFogi(e, lua_tointeger(L, 2));
		break;

		case GL_FOG_COLOR:
			if (!lua_istable(L, 2))
				luaL_error(L, "incorrect argument to function 'gl.Fog'");

			get_arrayf(L, 2, &param);
			glFogfv(e, (GLfloat*)param);
			free(param);
		break;

		default:
			if (!lua_isnumber(L, 2))
				luaL_error(L, "incorrect argument to function 'gl.Fog'");

			glFogf(e, (GLfloat)lua_tonumber(L, 2));
		break;
	}

	return 0;
}

/*FrontFace (mode) -> none*/
static int gl_front_face(lua_State *L)
{
	//GLenum e;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.FrontFace'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.FrontFace'");

	/* call opengl function */
	glFrontFace((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*Frustum (left, right, bottom, top, zNear, zFar) -> none*/
static int gl_frustum(lua_State *L)
{
	int index;

	/* test arguments type */
	for(index = 0; index < 6; index++)
		if(!lua_isnumber(L, index + 1))
			luaL_error(L, "incorrect argument to function 'gl.Frustum'");

	/* call opengl function */
	glFrustum((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
				 (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4),
				 (GLdouble)lua_tonumber(L, 5), (GLdouble)lua_tonumber(L, 6));

	return 0;
}

/*GenLists (range) -> num*/
static int gl_gen_lists(lua_State *L)
{
	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GenLists'");

	/* call opengl function and push the return value on the stack */
	lua_pushnumber(L, glGenLists((GLsizei)lua_tonumber(L, 1)) );

	return 1;
}

/*GenTextures (n) -> texturesArray*/
static int gl_gen_textures(lua_State *L)
{
	int i;
	GLsizei n;
	GLuint *textures;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GenTextures'");

	n = (GLsizei)lua_tonumber(L, 1);
	textures = (GLuint *)malloc(n * sizeof(GLuint));

	/* call opengl function */
	glGenTextures(n, (GLuint *)textures);

	lua_newtable(L);

	for(i = 0; i < n; i++)
		set_field(L, i+1, textures[i]);

	free(textures);

	return 1;
}

/*Get (pname) -> params*/
static int gl_get(lua_State *L)
{
	int i, size=1;
	GLenum e;
	GLdouble *params;
	int mask;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.Get'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);
	e = lua_tointeger(L, 1);

	switch(e)
	{
		case GL_STENCIL_VALUE_MASK:
		case GL_LINE_STIPPLE_PATTERN:
		case GL_STENCIL_WRITEMASK:
		case GL_INDEX_WRITEMASK:
			/* call opengl function */
			mask = 0;
			glGetIntegerv(e, &mask);
			lua_pushstring(L, mask2str(mask));
			return 1;

		case GL_DEPTH_RANGE:
		case GL_MAP1_GRID_DOMAIN:
		case GL_MAP2_GRID_SEGMENTS:
		case GL_MAX_VIEWPORT_DIMS:
		case GL_POINT_SIZE_RANGE:
		case GL_POLYGON_MODE:
			size = 2;
			break;

		case GL_CURRENT_NORMAL:
			size = 3;
			break;

		case GL_ACCUM_CLEAR_VALUE:
		case GL_COLOR_CLEAR_VALUE:
		case GL_COLOR_WRITEMASK:
		case GL_CURRENT_COLOR:
		case GL_CURRENT_RASTER_COLOR:
		case GL_CURRENT_RASTER_POSITION:
		case GL_CURRENT_RASTER_TEXTURE_COORDS:
		case GL_CURRENT_TEXTURE_COORDS:
		case GL_FOG_COLOR:
		case GL_LIGHT_MODEL_AMBIENT:
		case GL_MAP2_GRID_DOMAIN:
		case GL_SCISSOR_BOX:
		case GL_TEXTURE_ENV_COLOR:
		case GL_VIEWPORT:
			size = 4;
			break;

		case GL_MODELVIEW_MATRIX:
		case GL_PROJECTION_MATRIX:
		case GL_TEXTURE_MATRIX:
			size = 16;
			break;

		//case ENUM_ERROR:
		//	luaL_error(L, "incorrect string argument to function 'gl.Get'");
		//	break;
	}
	params = (GLdouble *)malloc(size * sizeof(GLdouble));

	/* call opengl function */
	glGetDoublev(e, params);

	for(i = 0; i < size; i++)
		lua_pushnumber(L, params[i]);

	free(params);

	return size;
}

/*GetConst (pname) -> constant string*/
static int gl_get_const(lua_State *L)
{
	int i, size=1;
	GLenum e;
	GLenum *params;
	const char *str;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GetConst'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);
	e = lua_tointeger(L, 1);

	switch(e)
	{
		case GL_DEPTH_RANGE:
		case GL_MAP1_GRID_DOMAIN:
		case GL_MAP2_GRID_SEGMENTS:
		case GL_MAX_VIEWPORT_DIMS:
		case GL_POINT_SIZE_RANGE:
		case GL_POLYGON_MODE:
			size = 2;
			break;

		case GL_CURRENT_NORMAL:
			size = 3;
			break;

		case GL_ACCUM_CLEAR_VALUE:
		case GL_COLOR_CLEAR_VALUE:
		case GL_COLOR_WRITEMASK:
		case GL_CURRENT_COLOR:
		case GL_CURRENT_RASTER_COLOR:
		case GL_CURRENT_RASTER_POSITION:
		case GL_CURRENT_RASTER_TEXTURE_COORDS:
		case GL_CURRENT_TEXTURE_COORDS:
		case GL_FOG_COLOR:
		case GL_LIGHT_MODEL_AMBIENT:
		case GL_MAP2_GRID_DOMAIN:
		case GL_SCISSOR_BOX:
		case GL_TEXTURE_ENV_COLOR:
		case GL_VIEWPORT:
			size = 4;
			break;

		case GL_MODELVIEW_MATRIX:
		case GL_PROJECTION_MATRIX:
		case GL_TEXTURE_MATRIX:
			size = 16;
			break;
	}
	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetConst'");

	params = (GLenum *)malloc(size * sizeof(GLenum));

	/* call opengl function */
	glGetIntegerv(e, (GLint*)params);

	for(i = 0; i < size; i++)
	{
		str = get_str_gl_enum(params[i]);
		lua_pushstring(L, str);
	}

	free(params);

	return size;
}

/*GetArray (pname) -> paramsArray*/
static int gl_get_array(lua_State *L)
{
	int i, size = 1;
	GLenum e;
	GLdouble *params;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GetArray'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);
	e = lua_tointeger(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetArray'");

	switch(e)
	{
		case GL_DEPTH_RANGE:
		case GL_MAP1_GRID_DOMAIN:
		case GL_MAP2_GRID_SEGMENTS:
		case GL_MAX_VIEWPORT_DIMS:
		case GL_POINT_SIZE_RANGE:
		case GL_POLYGON_MODE:
			size = 2;
			break;

		case GL_CURRENT_NORMAL:
			size = 3;
			break;

		case GL_ACCUM_CLEAR_VALUE:
		case GL_COLOR_CLEAR_VALUE:
		case GL_COLOR_WRITEMASK:
		case GL_CURRENT_COLOR:
		case GL_CURRENT_RASTER_COLOR:
		case GL_CURRENT_RASTER_POSITION:
		case GL_CURRENT_RASTER_TEXTURE_COORDS:
		case GL_CURRENT_TEXTURE_COORDS:
		case GL_FOG_COLOR:
		case GL_LIGHT_MODEL_AMBIENT:
		case GL_MAP2_GRID_DOMAIN:
		case GL_SCISSOR_BOX:
		case GL_TEXTURE_ENV_COLOR:
		case GL_VIEWPORT:
			size = 4;
			break;

		case GL_MODELVIEW_MATRIX:
		case GL_PROJECTION_MATRIX:
		case GL_TEXTURE_MATRIX:
			size = 16;
			break;
	}

	params = (GLdouble *)malloc(size * sizeof(GLdouble));

	/* call opengl function */
	glGetDoublev(e, params);

	lua_newtable(L);

	for(i = 0; i < size; i++)
		set_field(L, i+1, params[i]);

	free(params);

	return 1;
}

/*GetClipPlane (plane) -> equationArray*/
static int gl_get_clip_plane(lua_State *L)
{
	int i;
	//GLenum e;
	GLdouble *equation;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GetClipPlane'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetClipPlane'");

	equation = (GLdouble *)malloc(4 * sizeof(GLdouble));

	/* call opengl function */
	glGetClipPlane((GLenum)lua_tointeger(L, 1), equation);

	lua_newtable(L);

	for(i = 0; i < 4; i++)
		set_field(L, i+1, equation[i]);

	free(equation);

	return 1;
}

/*GetError () -> error flag*/
static int gl_get_error(lua_State *L)
{
	/* call glGetError function,
		convert returned number to string,
		and push the string on the stack. */
	GLenum error = glGetError();

	if(error == GL_NO_ERROR)
		lua_pushstring(L, "NO_ERROR");
	else
		lua_pushstring(L, get_str_gl_enum(error));

	return 1;
}

/*GetLight (light, pname) -> paramsArray*/
static int gl_get_light(lua_State *L)
{
	int i, size = 1;
	//GLenum e1, e2;
	GLenum e2;
	GLfloat *params;

	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.GetLight'");

	/* get string parameters */
	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);
	e2 = lua_tointeger(L, 2);

	/* test argument */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetLight'");

	switch(e2)
	{
		case GL_AMBIENT:
		case GL_DIFFUSE:
		case GL_SPECULAR:
		case GL_POSITION:
			size = 4;
			break;
		case GL_SPOT_DIRECTION :
			size = 3;
			break;
		case GL_SPOT_EXPONENT:
		case GL_SPOT_CUTOFF:
		case GL_CONSTANT_ATTENUATION:
		case GL_LINEAR_ATTENUATION:
		case GL_QUADRATIC_ATTENUATION:
			size = 1;
			break;
	}

	params = (GLfloat *)malloc(size * sizeof(GLfloat));

	/* call opengl function */
	glGetLightfv((GLenum)lua_tointeger(L, 1), e2, params);

	lua_newtable(L);

	for(i = 0; i < size; i++)
		set_field(L, i+1, params[i]);

	free(params);

	return 1;
}

/*GetMap (target, query) -> vArray*/
static int gl_get_map(lua_State *L)
{
	int i, size = 1;
	//GLenum e1, e2;
	GLenum e1;
	GLdouble *params;
	GLint *order;

	order = (GLint *)malloc(2 * sizeof(GLint));
	order[0] = order[1] = 1;

	/* test arguments type */
	if( !(lua_isnumber(L, 1) && lua_isnumber(L, 2)) )
		luaL_error(L, "incorrect argument to function 'gl.GetMap'");

	/* get string parameters */
	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);
	e1 = lua_tointeger(L, 1);

	/* test argument */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetMap'");

	switch(e1)
	{
		case GL_MAP1_INDEX:
		case GL_MAP2_INDEX:
		case GL_MAP1_TEXTURE_COORD_1:
		case GL_MAP2_TEXTURE_COORD_1:
			size = 1;
			break;
		case GL_MAP1_TEXTURE_COORD_2:
		case GL_MAP2_TEXTURE_COORD_2:
			size = 2;
			break;
		case GL_MAP1_VERTEX_3:
		case GL_MAP2_VERTEX_3:
		case GL_MAP1_NORMAL:
		case GL_MAP2_NORMAL:
		case GL_MAP1_TEXTURE_COORD_3:
		case GL_MAP2_TEXTURE_COORD_3:
			size = 3;
			break;
		case GL_MAP1_VERTEX_4:
		case GL_MAP2_VERTEX_4:
		case GL_MAP1_COLOR_4:
		case GL_MAP2_COLOR_4:
		case GL_MAP1_TEXTURE_COORD_4:
		case GL_MAP2_TEXTURE_COORD_4:
			size = 4;
			break;
	}

	glGetMapiv(e1, GL_ORDER, order);

	size *= order[0] * order[1];

	params = (GLdouble *)malloc(size * sizeof(GLdouble));

	/* call opengl function */
	glGetMapdv(e1, (GLenum)lua_tointeger(L, 2), params);

	lua_newtable(L);

	for(i = 0; i < size; i++)
		set_field(L, i+1, params[i]);

	free(params);

	return 1;
}

/*GetMaterial (face, pname) -> paramsArray*/
static int gl_get_material(lua_State *L)
{
	int i, size = 1;
	//GLenum e1, e2;
	GLenum e2;
	GLfloat *params;

	/* test arguments type */
	if( !(lua_isnumber(L, 1) && lua_isnumber(L, 2)) )
		luaL_error(L, "incorrect argument to function 'gl.GetMaterial'");

	/* get string parameters */
	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);
	e2 = lua_tointeger(L, 2);

	switch(e2)
	{
		case GL_AMBIENT:
		case GL_DIFFUSE:
		case GL_SPECULAR:
		case GL_EMISSION:
			size = 4;
			break;
		case GL_COLOR_INDEXES:
			size = 3;
			break;
		case GL_SHININESS:
			size = 1;
			break;
	}

	/* test argument */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetMaterial'");

	params = (GLfloat *)malloc(size * sizeof(GLfloat));

	/* call opengl function */
	glGetMaterialfv((GLenum)lua_tointeger(L, 1), e2, params);

	lua_newtable(L);

	for(i = 0; i < size; i++)
		set_field(L, i+1, params[i]);

	free(params);

	return 1;
}

/*GetPixelMap (map) -> valuesArray*/
static int gl_get_pixel_map(lua_State *L)
{
	int size;
	int i, s = GL_PIXEL_MAP_R_TO_R_SIZE;
	GLenum e;
	GLfloat *values;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GetPixelMap'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);
	e = lua_tointeger(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetPixelMap'");

	switch(e)
	{
		case GL_PIXEL_MAP_I_TO_I: s = GL_PIXEL_MAP_I_TO_I_SIZE; break;
		case GL_PIXEL_MAP_S_TO_S: s = GL_PIXEL_MAP_S_TO_S_SIZE; break;
		case GL_PIXEL_MAP_I_TO_R: s = GL_PIXEL_MAP_I_TO_R_SIZE; break;
		case GL_PIXEL_MAP_I_TO_G: s = GL_PIXEL_MAP_I_TO_G_SIZE; break;
		case GL_PIXEL_MAP_I_TO_B: s = GL_PIXEL_MAP_I_TO_B_SIZE; break;
		case GL_PIXEL_MAP_I_TO_A: s = GL_PIXEL_MAP_I_TO_A_SIZE; break;
		case GL_PIXEL_MAP_R_TO_R: s = GL_PIXEL_MAP_R_TO_R_SIZE; break;
		case GL_PIXEL_MAP_G_TO_G: s = GL_PIXEL_MAP_G_TO_G_SIZE; break;
		case GL_PIXEL_MAP_B_TO_B: s = GL_PIXEL_MAP_B_TO_B_SIZE; break;
		case GL_PIXEL_MAP_A_TO_A: s = GL_PIXEL_MAP_A_TO_A_SIZE; break;
	}
	glGetIntegerv(s, &size);

	values = (GLfloat *)malloc(size * sizeof(GLfloat));

	/* call opengl function */
	glGetPixelMapfv(e, values);

	lua_newtable(L);

	for(i = 0; i < size; i++)
		set_field(L, i+1, values[i]);

	free(values);

	return 1;
}

/*GetPointer (pname, n) -> valuesArray*/
static int gl_get_pointer(lua_State *L)
{
	int i, n;
	GLenum e;
	GLboolean *flags;
	GLdouble *params;

	/* test argument type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.GetPointer'");

	//e = get_gl_enum(L, 1);
	e = lua_tointeger(L, 1);
	n = (int)lua_tonumber(L, 2);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetPointer'");

	if(e == GL_EDGE_FLAG_ARRAY_POINTER)
	{
		flags = (GLboolean *)malloc(n * sizeof(GLboolean));

		/* call opengl function */
		glGetPointerv(e, (void **)&flags);

		if(flags == 0)
			return 0;

		lua_newtable(L);

		for(i = 0; i < n ; i++)
			set_field(L, i+1, flags[i]);
	}
	else
	{
		params = (GLdouble *)malloc(n * sizeof(GLdouble));

		/* call opengl function */
		glGetPointerv(e, (void **)&params);

		if(params == 0)
			return 0;

		lua_newtable(L);

		for(i = 0; i < n ; i++)
			set_field(L, i+1, params[i]);
	}

	return 1;
}

/*GetPolygonStipple () -> maskArray*/
static int gl_get_polygon_stipple(lua_State *L)
{
	int index;
	GLubyte *mask = (GLubyte*)malloc(32*32 * sizeof(GLubyte));

	glGetPolygonStipple(mask);

	lua_newtable(L);

	for(index = 0; index < 1024; index++)
		set_field(L, index+1, mask[index]);

	return 1;
}

/*GetString (name) -> string*/
static int gl_get_string(lua_State *L)
{
	//GLenum e;
	const GLubyte *str;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GetString'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetString'");

	/* call opengl function */
	str = glGetString((GLenum)lua_tointeger(L, 1));

	lua_pushstring(L, (const char*)str);

	return 1;
}

/*GetTexEnv (pname) -> paramsArray*/
static int gl_get_tex_env(lua_State *L)
{
	int i;
	GLenum e1;
	GLfloat *params;
	int e2;

	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GetTexEnv'");

	/* get string parameters */
	//e1 = get_gl_enum(L, 1);
	e1 = lua_tointeger(L, 1);

	/* test argument */
	//if(e1 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetTexEnv'");

	if(e1 == GL_TEXTURE_ENV_MODE)
	{
		glGetTexEnviv(GL_TEXTURE_ENV, e1, &e2);

		lua_pushinteger(L, e2);
	}
	else if(e1 == GL_TEXTURE_ENV_COLOR)
	{
		params = (GLfloat *)malloc(4 * sizeof(GLfloat));

		/* call opengl function */
		glGetTexEnvfv(GL_TEXTURE_ENV, e1, params);

		lua_newtable(L);

		for(i = 0; i < 4; i++)
			set_field(L, i+1, params[i]);

		free(params);
	}
	else
	{
		luaL_error(L, "incorrect string argument to function 'gl.GetTexEnv'");
	}
	return 1;
}

/*GetTexGen (coord, pname) -> paramsArray*/
static int gl_get_tex_gen(lua_State *L)
{
	int i;
	//GLenum e1, e2;
	GLenum e2;
	GLdouble *params;
	int e3;

	/* test arguments type */
	if( !(lua_isnumber(L, 1) && lua_isnumber(L, 2)) )
		luaL_error(L, "incorrect argument to function 'gl.GetTexGen'");

	/* get string parameters */
	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);
	e2 = lua_tointeger(L, 2);

	/* test argument */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetTexGen'");
	if(e2 == GL_TEXTURE_GEN_MODE)
	{
		/* call opengl function */
		glGetTexGeniv((GLenum)lua_tointeger(L, 1), e2, &e3);

		lua_pushinteger(L, e3);
	}
	else
	{
		params = (GLdouble *)malloc(4 * sizeof(GLdouble));

		/* call opengl function */
		glGetTexGendv((GLenum)lua_tointeger(L, 1), e2, params);

		lua_newtable(L);

		for(i = 0; i < 4; i++)
			set_field(L, i+1, params[i]);

		free(params);
	}
	return 1;
}

/*GetTexImage (target, level, format) -> pixelsArray*/
static int gl_get_tex_image(lua_State *L)
{
	int i, n=1;
	int width, height, level;
	GLenum target, format;
	GLfloat *pixels;

	/* test arguments type */
	if( !(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)) )
		luaL_error(L, "incorrect argument to function 'gl.GetTexImage'");

	/* get string parameters */
	target = lua_tointeger(L, 1); //get_gl_enum(L, 1);
	level = (int)lua_tonumber(L, 2);
	format = lua_tointeger(L, 3); //get_gl_enum(L, 3);

	/* get width and height of image */
	glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &height);

	switch(format)
	{
		case GL_RED:  case GL_GREEN:  case GL_BLUE:
		case GL_ALPHA: case GL_LUMINANCE: n = 1; break;
		case GL_LUMINANCE_ALPHA:			 n = 2; break;
		case GL_RGB:  case GL_BGR_EXT:	 n = 3; break;
		case GL_RGBA: case GL_BGRA_EXT:	n = 4; break;
		default:
			luaL_error(L, "incorrect string argument to function 'gl.GetTexImage'");
	}

	pixels = (GLfloat *)malloc(n * width * height * sizeof(GLfloat));

	/* call opengl function */
	glGetTexImage(target, level, format, GL_FLOAT, pixels);

	lua_newtable(L);

	for(i = 0; i < n * width * height; i++)
		set_field(L, i+1, pixels[i]);

	free(pixels);

	return 1;
}

/*GetTexLevelParameter (target, level, pname) -> param*/
static int gl_get_tex_level_parameter(lua_State *L)
{
	//int level;
	//GLenum target, pname;
	GLfloat param;

	/* test arguments type */
	if( !(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)) )
		luaL_error(L, "incorrect argument to function 'gl.GetTexLevelParameter'");

	/* get parameters */
	//target = get_gl_enum(L, 1);
	//level = (int)lua_tonumber(L, 2);
	//pname = get_gl_enum(L, 3);

	/* call opengl function */
	glGetTexLevelParameterfv((GLenum)lua_tointeger(L, 1), lua_tointeger(L, 2), (GLenum)lua_tointeger(L, 3), &param);

	/* return parameter */
	lua_pushnumber(L, param);

	return 1;
}

/*GetTexParameter (target, pname) -> paramsArray*/
static int gl_get_tex_parameter(lua_State *L)
{
	int i;
	//GLenum target, pname;
	GLenum pname;
	GLfloat *params;
	GLfloat param;
	int e;

	/* test arguments type */
	if(! (lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.GetTexParameter'");

	/* get string parameters */
	//target = get_gl_enum(L, 1);
	pname = get_gl_enum(L, 2);

	if(pname == GL_TEXTURE_BORDER_COLOR)
	{
		params = (GLfloat *)malloc(4 * sizeof(float));

		/* call opengl function */
		glGetTexParameterfv((GLenum)lua_tointeger(L, 1), pname, params);

		/* return parameters */
		lua_newtable(L);

		for(i = 0; i < 4; i++)
			set_field(L, i+1, params[i]);
	}
	else if(pname == GL_TEXTURE_PRIORITY)
	{
		/* call opengl function */
		glGetTexParameterfv((GLenum)lua_tointeger(L, 1), pname, &param);

		lua_pushnumber(L, param);
	}
	else
	{
		/* call opengl function */
		glGetTexParameteriv((GLenum)lua_tointeger(L, 1), pname, &e);

		lua_pushstring(L, get_str_gl_enum(e));
	}
	return 1;
}

/*Hint (target, mode) -> none*/
static int gl_hint(lua_State *L)
{
	//GLenum e1, e2;

	/* test arguments type */
	if( !(lua_isnumber(L, 1) && lua_isnumber(L, 2)) )
		luaL_error(L, "incorrect argument to function 'gl.Hint'");

	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);

	/* test argument */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.Hint'");

	/* call opengl function */
	glHint((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2));

	return 0;
}

/*Index (c) -> none*/
static int gl_index(lua_State *L)
{
	GLdouble *c;

	if(lua_istable(L, 1))/* test argument type */
	{
		/* get argument */
		get_arrayd(L, 1, &c);

		/* call opengl function */
		glIndexdv((GLdouble *)c);

		free(c);
	}
	else if(lua_isnumber(L, 1))/* test argument type */
		/* call opengl function */
		glIndexd((GLdouble)lua_tonumber(L, 1));

	else
		luaL_error(L, "incorrect argument to function 'gl.Index'");

	return 0;
}

/*IndexMask (mask) -> none*/
static int gl_index_mask(lua_State *L)
{
	if (!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.IndexMask'");
	//if(lua_type(L,1) == LUA_TSTRING)
		/* call opengl function */
	//	glIndexMask(str2mask(lua_tostring(L, 1)));

	//else if(lua_type(L,1) == LUA_TNUMBER)
		/* call opengl function */
		glIndexMask((GLuint)lua_tointeger(L, 1));

	//else
	//	luaL_error(L, "incorrect argument to function 'gl.IndexMask'");

	return 0;
}

/* void glIndexPointer (GLenum type, GLsizei stride, const GLvoid* pointer)
 *
 * IndexPointer (indexArray [,stride]]) -> none
 * IndexPointer (bufferOffset [,stride]) -> none
 */
static int gl_index_pointer(lua_State *L)
{
	GLint size, offset;
	GLsizei stride = 0;
	static GLfloat *array = 0;

	if(array)
		free(array);

	if (lua_isnumber(L, 1)) {
		offset = lua_tointeger(L, 1);
		if (!lua_isnumber(L, 2)) {
			luaL_error(L, "incorrect argument to function 'gl.IndexPointer'");
		}
		
		if (lua_isnumber(L, 2)) {
			stride = lua_tointeger(L, 2);
		}

		glIndexPointer(GL_FLOAT, stride * sizeof(GLfloat), (GLubyte*) NULL + offset * sizeof(GLfloat));
	}
	else if (lua_istable(L, 1)) {
		if (get_array2f(L, 1, &array, &size) == -1)
			size = get_arrayf(L, 1, &array) / 3;

		if (lua_isnumber(L, 2)) {
			stride = lua_tointeger(L, 2);
		}

		glIndexPointer(GL_FLOAT, stride * sizeof(GLfloat), array);
	}
	else {
		luaL_error(L, "incorrect argument to function 'gl.IndexPointer'");
	}

	return 0;
}

/*InitNames () -> none*/
static int gl_init_names(lua_State *L)
{
	glInitNames();
	return 0;
}

/*IsEnabled (cap) -> true/false*/
static int gl_is_enabled(lua_State *L)
{
	//GLenum e;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.IsEnabled'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.IsEnabled'");

	/* call opengl function */
	lua_pushboolean(L, glIsEnabled((GLenum)lua_tointeger(L, 1)));

	return 1;
}

/*IsList (list) -> true/false*/
static int gl_is_list(lua_State *L)
{
	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.IsList'");

	/* call opengl function and push return value in the lua stack */
	lua_pushboolean(L, glIsList((GLuint)lua_tonumber(L, 1)));

	return 1;
}

/*IsTexture (texture) -> true/false*/
static int gl_is_texture(lua_State *L)
{
	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.IsTexture'");

	/* call opengl function and push return value in the lua stack */
	lua_pushboolean(L, glIsTexture((GLuint)lua_tonumber(L, 1)));

	return 1;
}

/*Light (light, pname, param) -> none
  Light (light, pname, paramsArray) -> none*/
static int gl_light(lua_State *L)
{
	//GLenum e1, e2;
	GLfloat *params;

	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.Light'");

	/* get string parameters */
	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);

	/* test argument */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.Light'");

	/* test argument type */
	if(lua_istable(L, 3))
	{
		/* get argument */
		get_arrayf(L, 3, &params);

		/* call opengl function */
		glLightfv((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2), (GLfloat *)params);

		free(params);
	}
	/* test argument type */
	else if(lua_isnumber(L, 3))
	{
		/* call opengl function */
		glLightf((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2), (GLfloat)lua_tonumber(L, 3));
	}
	else
		luaL_error(L, "incorrect argument to function 'gl.Light'");

	return 0;
}

/*LightModel (pname, param) -> none
  LightModel (pname, paramsArray) -> none*/
static int gl_light_model(lua_State *L)
{
	//GLenum e;
	GLfloat *params;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.LightModel'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.LightModel'");

	/* test argument type */
	if(lua_istable(L, 2))
	{
		/* get argument */
		get_arrayf(L, 2, &params);

		/* call opengl function */
		glLightModelfv((GLenum)lua_tointeger(L, 1), (GLfloat *)params);

		free(params);
	}
	/* test argument type */
	else if(lua_isnumber(L, 2))
		/* call opengl function */
		glLightModelf((GLenum)lua_tointeger(L, 1), (GLfloat)lua_tonumber(L, 2));

	else
		luaL_error(L, "incorrect argument to function 'gl.LightModel'");

	return 0;
}

/*LineStipple (factor, pattern) -> none*/
static int gl_line_stipple(lua_State *L)
{
	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.LineStipple'");

	if(lua_type(L,2) == LUA_TSTRING)
		/* call opengl function */
		glLineStipple((GLint)lua_tonumber(L, 1), (GLushort)str2mask(lua_tostring(L, 2)));

	else if(lua_type(L,2) == LUA_TNUMBER)
		/* call opengl function */
		glLineStipple((GLint)lua_tonumber(L, 1), (GLushort)lua_tonumber(L, 2));

	else
		luaL_error(L, "incorrect argument to function 'gl.LineStipple'");

	return 0;
}

/*LineWidth (width) -> none*/
static int gl_line_width(lua_State *L)
{
	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.LineWidth'");

	/* call opengl function */
	glLineWidth((GLfloat)lua_tonumber(L, 1));

	return 0;
}

/*ListBase (base) -> none*/
static int gl_list_base(lua_State *L)
{
	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.ListBase'");

	/* call opengl function */
	glListBase((GLuint)lua_tonumber(L, 1));

	return 0;
}

/*LoadIdentity () -> none*/
static int gl_load_identity(lua_State *L)
{
	glLoadIdentity();
	return 0;
}

/*LoadMatrix (mArray) -> none*/
static int gl_load_matrix(lua_State *L)
{
	GLdouble *m;

	/* test argument type and the number of arguments in the array, must be 16 values */
	if(!lua_istable(L, 1) || luaL_getn(L, 1) < 16)
		luaL_error(L, "incorrect argument to function 'gl.LoadMatrix'");

	/* get argument */
	get_arrayd(L, 1, &m);

	/* call opengl function */
	glLoadMatrixd(m);

	free(m);

	return 0;
}

/*LoadName (name) -> none*/
static int gl_load_name(lua_State *L)
{
	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.LoadName'");

	/* call opengl function */
	glLoadName((GLuint)lua_tonumber(L, 1));

	return 0;
}

/*LogicOp (opcode) -> none*/
static int gl_logic_op(lua_State *L)
{
	//GLenum opcode;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.LogicOp'");

	/* get string parameter */
	//opcode = get_gl_enum(L, 1);

	/* test argument */
	//if(opcode == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.LogicOp'");

	/* call opengl function */
	glLogicOp((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*Map (target, u1, u2, pointsArray) -> none
  Map (target, u1, u2, v1, v2, pointsArray) -> none*/
static int gl_map(lua_State *L)
{
	int size=1;
	GLenum target;
	GLdouble *points;
	GLint uorder, vorder;

	/* test argument */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
		luaL_error(L, "incorrect argument to function 'gl.Map'");

	//target = get_gl_enum(L, 1);
	target = lua_tointeger(L, 1);

	switch(target)
	{
		case GL_MAP1_INDEX:
		case GL_MAP2_INDEX:
		case GL_MAP1_TEXTURE_COORD_1:
		case GL_MAP2_TEXTURE_COORD_1:
			size = 1;
			break;
		case GL_MAP1_TEXTURE_COORD_2:
		case GL_MAP2_TEXTURE_COORD_2:
			size = 2;
			break;
		case GL_MAP1_VERTEX_3:
		case GL_MAP2_VERTEX_3:
		case GL_MAP1_NORMAL:
		case GL_MAP2_NORMAL:
		case GL_MAP1_TEXTURE_COORD_3:
		case GL_MAP2_TEXTURE_COORD_3:
			size = 3;
			break;
		case GL_MAP1_VERTEX_4:
		case GL_MAP2_VERTEX_4:
		case GL_MAP1_COLOR_4:
		case GL_MAP2_COLOR_4:
		case GL_MAP1_TEXTURE_COORD_4:
		case GL_MAP2_TEXTURE_COORD_4:
			size = 4;
			break;
	}

	/* test argument */
	//if(target == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.Map'");

	/* test number of argument in the array */
	if(lua_gettop(L) < 6)
	{
		if(!lua_istable(L, 4))
			luaL_error(L, "incorrect argument to function 'gl.Map'");

		/* get argument */
		uorder = get_arrayd(L, 4, &points) / size;

		/* call opengl function */
		glMap1d(target, (GLdouble)lua_tonumber(L, 2),
							 (GLdouble)lua_tonumber(L, 3),
							 size, uorder, points);

		free(points);
	}
	else
	{
		if(!( lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_istable(L, 6) ))
			luaL_error(L, "incorrect argument to function 'gl.Map'");

		/* get argument */
		vorder = get_array2d(L, 6, &points, &uorder);
		uorder /= size;

		/* call opengl function */
		glMap2d(target, (GLdouble)lua_tonumber(L, 2),
							 (GLdouble)lua_tonumber(L, 3),
							 size, uorder,
							 (GLdouble)lua_tonumber(L, 4),
							 (GLdouble)lua_tonumber(L, 5),
							 size * uorder, vorder,
							 points);

		free(points);
	}
	return 0;
}

/*MapGrid (un, u1, u2[, vn, v1, v2]) -> none*/
static int gl_map_grid(lua_State *L)
{
	/* test arguments */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
		luaL_error(L, "incorrect argument to function 'gl.MapGrid'");

	/* test number of arguments */
	if(lua_gettop(L) < 6)
	{
		/* call opengl function */
		glMapGrid1d((GLint)lua_tonumber(L, 1),
						(GLdouble)lua_tonumber(L, 2),
						(GLdouble)lua_tonumber(L, 3));
	}
	else
	{
		/* test arguments type */
		if(!( lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_isnumber(L, 6) ))
			luaL_error(L, "incorrect argument to function 'gl.MapGrid'");

		/* call opengl function */
		glMapGrid2d((GLint)lua_tonumber(L, 1),
						(GLdouble)lua_tonumber(L, 2),
						(GLdouble)lua_tonumber(L, 3),
						(GLint)lua_tonumber(L, 4),
						(GLdouble)lua_tonumber(L, 5),
						(GLdouble)lua_tonumber(L, 6));
	}
	return 0;
}

/*Material (face, pname, param) -> none*/
static int gl_material(lua_State *L)
{
	//GLenum e1, e2;
	GLfloat *params;

	/* test arguments type */
	if( !(lua_isnumber(L, 1) && lua_isnumber(L, 2)) )
		luaL_error(L, "incorrect argument to function 'gl.Material'");

	/* get string parameters */
	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);

	/* test argument */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.Material'");

	/* test argument type */
	if(lua_istable(L, 3))
	{
		/* get argument */
		get_arrayf(L, 3, &params);

		/* call opengl function */
		glMaterialfv((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2), (GLfloat *)params);

		free(params);
	}
	/* test argument type */
	else if(lua_isnumber(L, 3))
	{
		/* call opengl function */
		glMaterialf((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2), (GLfloat)lua_tonumber(L, 3));
	}
	else
		luaL_error(L, "incorrect argument to function 'gl.Material'");

	return 0;
}

/*MatrixMode (mode) -> none*/
static int gl_matrix_mode(lua_State *L)
{
	//GLenum mode;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.MatrixMode'");

	/* get string parameter */
	//mode = get_gl_enum(L, 1);

	/* test argument */
	//if(mode == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.MatrixMode'");

	/* call opengl function */
	glMatrixMode((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*MultMatrix (mArray) -> none*/
static int gl_mult_matrix(lua_State *L)
{
	GLdouble *m;

	/* test argument type and the number of arguments in the array, must be 16 values */
	if(!lua_istable(L, 1) || luaL_getn(L, 1) < 16)
		luaL_error(L, "incorrect argument to function 'gl.MultMatrix'");

	/* get argument */
	get_arrayd(L, 1, &m);

	/* call opengl function */
	glMultMatrixd((GLdouble *)m);

	free(m);

	return 0;
}

/*NewList (list, mode) -> none*/
static int gl_new_list(lua_State *L)
{
	//GLenum e;

	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.NewList'");

	/* get string parameter */
	//e = get_gl_enum(L, 2);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.NewList'");

	/* call opengl function */
	glNewList((GLint)lua_tonumber(L, 1), (GLenum)lua_tointeger(L, 2));

	return 0;
}

/*Normal (nx, ny, nz) -> none
  Normal (nArray) -> none*/
static int gl_normal(lua_State *L)
{
	GLdouble *array;

	int num_args;

	/* test arguments type */
	if(lua_istable(L, 1))
	{
		num_args = get_arrayd(L, 1, &array);

		if(num_args < 3)
			luaL_error(L, "incorrect argument to function 'gl.Normal'");

		/* call openGL function */
		glNormal3dv(array);

		free(array);

		return 0;
	}

	/* test arguments */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
		luaL_error(L, "incorrect argument to function 'gl.Normal'");

	/* call openGL functions */
	glNormal3d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
				  (GLdouble)lua_tonumber(L, 3));

	return 0;
}

/* void glNormalPointer (GLenum type, GLsizei stride, const GLvoid* pointer)
 *
 * NormalPointer (colorArray [,stride]]) -> none
 * NormalPointer (bufferOffset [,stride]) -> none
 */
static int gl_normal_pointer(lua_State *L)
{
	GLint size, offset;
	GLsizei stride = 0;
	static GLfloat *array = 0;

	if(array)
		free(array);

	if (lua_isnumber(L, 1)) {
		offset = lua_tointeger(L, 1);
		if (!lua_isnumber(L, 2)) {
			luaL_error(L, "incorrect argument to function 'gl.NormalPointer'");
		}
		
		if (lua_isnumber(L, 2)) {
			stride = lua_tointeger(L, 2);
		}

		glNormalPointer(GL_FLOAT, stride * sizeof(GLfloat), (GLubyte*) NULL + offset * sizeof(GLfloat));
	}
	else if (lua_istable(L, 1)) {
		if (get_array2f(L, 1, &array, &size) == -1)
			size = get_arrayf(L, 1, &array) / 3;

		if (lua_isnumber(L, 2)) {
			stride = lua_tointeger(L, 2);
		}

		glNormalPointer(GL_FLOAT, stride * sizeof(GLfloat), array);
	}
	else {
		luaL_error(L, "incorrect argument to function 'gl.NormalPointer'");
	}

	return 0;
}

/*Ortho (left, right, bottom, top, zNear, zFar) -> none*/
static int gl_ortho(lua_State *L)
{
	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) &&
			lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_isnumber(L, 6)))
		luaL_error(L, "incorrect string argument to function 'gl.Ortho'");

	/* call opengl function */
	glOrtho((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
			  (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4),
			  (GLdouble)lua_tonumber(L, 5), (GLdouble)lua_tonumber(L, 6));

	return 0;
}

/*PassThrough (token) -> none*/
static int gl_pass_through(lua_State *L)
{
	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect string argument to function 'gl.PassThrough'");

	/* call opengl function */
	glPassThrough((GLfloat)lua_tonumber(L, 1));

	return 0;
}

/*PixelMap (map, valuesArray) -> none*/
static int gl_pixel_map(lua_State *L)
{
	//GLenum map;
	GLfloat *values;
	int mapsize;

	/* test arguments */
	if(!( lua_isnumber(L, 1) && lua_istable(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.PixelMap'");

	/* get values */
	//map = get_gl_enum(L, 1);

	/* test argument */
	//if(map == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.PixelMap'");

	/* get array of equations */
	mapsize = get_arrayf(L, 2, &values);

	/* call opengl function */
	glPixelMapfv((GLenum)lua_tointeger(L, 1), mapsize, values);

	free(values);

	return 0;
}

/*PixelStore (pname, param) -> none*/
static int gl_pixel_store(lua_State *L)
{
	/* get string parameters */
	//GLenum e;
	
	/* test argument */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.PixelStore'");

	//e = get_gl_enum(L, 1);

	/* test arguments */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.PixelStore'");

	if(lua_isnumber(L, 2))
		/* call opengl function */
		glPixelStoref((GLenum)lua_tointeger(L, 1), (GLfloat)lua_tonumber(L, 2));

	else if(lua_isboolean(L,2))
		/* call opengl function */
		glPixelStoref((GLenum)lua_tointeger(L, 1), (GLfloat)lua_toboolean(L, 2));

	else
		luaL_error(L, "incorrect argument to function 'gl.PixelStore'");

	return 0;
}

/*PixelTransfer (pname, param) -> none*/
static int gl_pixel_transfer(lua_State *L)
{
	/* get string parameters */
	//GLenum e;
	
	/* test argument */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.PixelTransfer'");

	//e = get_gl_enum(L, 1);

	/* test arguments */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.PixelTransfer'");

	if(lua_isnumber(L, 2))
		/* call opengl function */
		glPixelTransferf((GLenum)lua_tointeger(L, 1), (GLfloat)lua_tonumber(L, 2));

	else if(lua_isboolean(L,2))
		/* call opengl function */
		glPixelTransferf((GLenum)lua_tointeger(L, 1), (GLfloat)lua_toboolean(L, 2));

	else
		luaL_error(L, "incorrect argument to function 'gl.PixelTransfer'");

	return 0;
}

/*PixelZoom (xfactor, yfactor) -> none*/
static int gl_pixel_zoom(lua_State *L)
{
	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect string argument to function 'gl.PixelZoom'");

	/* call opengl function */
	glPixelZoom((GLfloat)lua_tonumber(L, 1), (GLfloat)lua_tonumber(L, 2));

	return 0;
}

/*PointSize (size) -> none*/
static int gl_point_size(lua_State *L)
{
	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect string argument to function 'gl.PointSize'");

	/* call opengl function */
	glPointSize((GLfloat)lua_tonumber(L, 1));

	return 0;
}

/*PolygonMode (face, mode) -> none*/
static int gl_polygon_mode(lua_State *L)
{
	//GLenum e1, e2;

	/* test arguments type */
	if( !(lua_isnumber(L, 1) && lua_isnumber(L, 2)) )
		luaL_error(L, "incorrect argument to function 'gl.PolygonMode'");

	/* get string parameters */
	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);

	/* test argument */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.PolygonMode'");

	/* call opengl function */
	glPolygonMode((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2));

	return 0;
}

/*PolygonOffset (factor, units) -> none*/
static int gl_polygon_offset(lua_State *L)
{
	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect string argument to function 'gl.PolygonOffset'");

	/* call opengl function */
	glPolygonOffset((GLfloat)lua_tonumber(L, 1), (GLfloat)lua_tonumber(L, 2));

	return 0;
}

/*PolygonStipple (maskArray) -> none*/
static int gl_polygon_stipple(lua_State *L)
{
	GLubyte *array;
	int width, height = 32;

	/* test arguments type */
	if(!lua_istable(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.PolygonStipple'");

	if((height = get_array2ubyte(L, 1, &array, &width)) == -1)
		width = get_arrayubyte(L, 4, &array);

	if(width != 32 && height != 32)
	{
		free(array);
		luaL_error(L, "incorrect argument to function 'gl.PolygonStipple'");
	}

	/* call opengl function */
	glPolygonStipple(array);

	return 0;
}

/*PopAttrib () -> none*/
static int gl_pop_attrib(lua_State *L)
{
	glPopAttrib();
	return 0;
}

/*PopClientAttrib () -> none*/
static int gl_pop_client_attrib(lua_State *L)
{
	glPopClientAttrib();
	return 0;
}

/*PopMatrix () -> none*/
static int gl_pop_matrix(lua_State *L)
{
	glPopMatrix();
	return 0;
}

/*PopName () -> none*/
static int gl_pop_name(lua_State *L)
{
	glPopName();
	return 0;
}

/*PrioritizeTextures (texturesArray, prioritiesArray) -> none*/
static int gl_prioritize_textures(lua_State *L)
{
	GLsizei n1, n2;
	GLuint *array1;
	GLclampf *array2;

	/* test arguments type */
	if(!( lua_istable(L, 1) && lua_istable(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.PrioritizeTextures'");

	/* get arguments */
	n1 = get_arrayui(L, 1, &array1);
	n2 = get_arrayf(L, 2, &array2);

	/* call opengl function */
	if(n1 > n2) n1 =  n2;

	glPrioritizeTextures(n1, array1, array2);

	free(array1);
	free(array2);

	return 0;
}

/*PushAttrib (mask) -> none*/
static int gl_push_attrib(lua_State *L)
{
	//GLbitfield e;

	int i, paramCount;
	GLenum accum;

	paramCount = lua_gettop(L);
	for (i = 0; i < paramCount; i++) {
		if (!lua_isnumber(L, i + 1))
			luaL_error(L, "incorrect argument to function 'gl.PushAttrib'");
		accum |= lua_tointeger(L, i + 1);
	}

	/* test arguments type */
	//if(!lua_isstring(L, 1))
	//	luaL_error(L, "incorrect argument to function 'gl.PushAttrib'");

	//e = get_gl_enum(L, 1);

	/* test arguments */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.PushAttrib'");

	/* call opengl function */
	glPushAttrib(accum);

	return 0;
}

/*PushClientAttrib (mask) -> none*/
static int gl_push_client_attrib(lua_State *L)
{
	//GLbitfield e;

	int i, paramCount;
	GLenum accum;

	paramCount = lua_gettop(L);
	for (i = 0; i < paramCount; i++) {
		if (!lua_isnumber(L, i + 1))
			luaL_error(L, "incorrect argument to function 'gl.PushClientAttrib'");
		accum |= lua_tointeger(L, i + 1);
	}

	/* test arguments type */
	//if(!lua_isstring(L, 1))
	//	luaL_error(L, "incorrect argument to function 'gl.PushClientAttrib'");

	//e = get_gl_enum(L, 1);

	/* test arguments */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.PushClientAttrib'");

	/* call opengl function */
	glPushClientAttrib(accum);

	return 0;
}

/*PushMatrix () -> none*/
static int gl_push_matrix(lua_State *L)
{
	glPushMatrix();
	return 0;
}

/*PushName (GLuint name) -> none*/
static int gl_push_name(lua_State *L)
{
	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.PushName'");

	/* call opengl function */
	glPushName((GLuint)lua_tonumber(L, 1));

	return 0;
}

/*RasterPos (x, y[, z, w]) -> none
  RasterPos (vArray) -> none*/
static int gl_raster_pos(lua_State *L)
{
	GLdouble *array;

	int index;
	int num_args = lua_gettop(L);

	/* test arguments type */
	if(lua_istable(L, 1))
	{
		num_args = get_arrayd(L, 1, &array);

		/* if more then 4 arguments, ignore the others */
		if(num_args > 4)
			num_args = 4;

		/* call openGL functions */
		switch(num_args)
		{
			case 2:  glRasterPos2dv(array); break;
			case 3:  glRasterPos3dv(array); break;
			case 4:  glRasterPos4dv(array); break;
		}

		free(array);

		return 0;
	}

	/* if more then 4 arguments, ignore the others */
	if(num_args > 4)
		num_args = 4;

	for(index = 0; index < num_args; index++)
	{
		if(!lua_isnumber(L, index + 1))
			luaL_error(L, "incorrect argument to function 'gl.RasterPos'");
	}

	/* call openGL functions */
	switch(num_args)
	{
		case 2:  glRasterPos2d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2));
					break;
		case 3:  glRasterPos3d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
									  (GLdouble)lua_tonumber(L, 3));
					break;
		case 4:  glRasterPos4d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
									  (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4));
					break;
	}
	return 0;
}

/*ReadBuffer (mode) -> none*/
static int gl_read_buffer(lua_State *L)
{
	//GLenum mode;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.ReadBuffer'");

	/* get string parameter */
	//mode = get_gl_enum(L, 1);

	/* test argument */
	//if(mode == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.ReadBuffer'");

	/* call opengl function */
	glReadBuffer((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*ReadPixels (x, y, width, height, format, pixelsArray) -> none*/
static int gl_read_pixels(lua_State *L)
{
	GLenum e;
	GLfloat *pixels;
	int width;
	int height;
	int n, i, j;
	int size = 1;

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
		  lua_isnumber(L, 3) && lua_isnumber(L, 4) &&
		  lua_isnumber(L, 5) && lua_istable (L, 6)) )
		luaL_error(L, "incorrect argument to function 'gl.ReadPixels'");

	/* get parameters */
	width = lua_tointeger(L, 3);
	height = lua_tointeger(L, 4);
	//e = get_gl_enum(L, 5);
	e = lua_tointeger(L, 5);
	//get_arrayf(L, 6, &pixels);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.ReadPixels'");

	switch (e) {
		case GL_LUMINANCE_ALPHA:
			size = 2;
		case GL_RGB:
		case GL_BGR:
			size = 3; break;
		case GL_RGBA:
		case GL_BGRA:
			size = 4; break;
	}

	n = width * height;
	pixels = (GLfloat*)malloc(n * size * sizeof(GLfloat));

	/* call opengl function */
	glReadPixels((GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 2),
					 (GLsizei)width, (GLsizei)height,
					 e, GL_FLOAT, pixels);

	lua_settop(L, 6);

	for (i = 0; i < n; i++) {
		for (j = 0; j < size; j++) {
			set_field(L, i * size + j + 1, pixels[i * size + j]);
		}
	}

	free(pixels);

	return 0;
}

/*Rect (x1, y1, x2, y2) -> none
  Rect (v1, v2) -> none*/
static int gl_rect(lua_State *L)
{
	GLdouble *v1, *v2;

	/* test argument type */
	if(lua_istable(L, 1) && lua_istable(L, 2))
	{
		/* get parameters */
		get_arrayd(L, 1, &v1);
		get_arrayd(L, 2, &v2);

		/* call opengl function */
		glRectdv(v1, v2);

		free(v1);
		free(v2);
	}
	/* test argument type */
	else if(lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
			  lua_isnumber(L, 3) && lua_isnumber(L, 4))
		/* call openGL functions */
		glRectd((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
				  (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4));

	else
		luaL_error(L, "incorrect argument to function 'gl.Rect'");

	return 0;
}

/*RenderMode (mode) -> none*/
static int gl_render_mode(lua_State *L)
{
	//GLenum mode;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.RenderMode'");

	/* get string parameter */
	//mode = get_gl_enum(L, 1);

	/* test argument */
	//if(mode == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.RenderMode'");

	/* call opengl function */
	glRenderMode((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*Rotate (angle, x, y, z) -> none*/
static int gl_rotate(lua_State *L)
{
	/* test argument type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
			lua_isnumber(L, 3) && lua_isnumber(L, 4) ))
		luaL_error(L, "incorrect argument to function 'gl.Rotate'");

	/* call opengl function */
	glRotated((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
				 (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4));

	return 0;
}

/*Scale (x, y, z) -> none*/
static int gl_scale(lua_State *L)
{
	/* test argument type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
		luaL_error(L, "incorrect argument to function 'gl.Scale'");

	/* call opengl function */
	glScaled((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
				(GLdouble)lua_tonumber(L, 3));

	return 0;
}

/*Scissor (x, y, width, height) -> none*/
static int gl_scissor(lua_State *L)
{
	/* test argument type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) ))
		luaL_error(L, "incorrect argument to function 'gl.Scissor'");

	/* call opengl function */
	glScissor((GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 2),
				 (GLsizei)lua_tonumber(L, 3), (GLsizei)lua_tonumber(L, 4));

	return 0;
}

/*SelectBuffer (size) -> SelectArray*/
static int gl_select_buffer(lua_State *L)
{
	int size, i;
	GLuint *buffer;

	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.SelectBuffer'");

	size = (int)lua_tonumber(L, 1);

	buffer = (GLuint *)malloc(size * sizeof(GLuint));

	/* call opengl function */
	glSelectBuffer (size, buffer);

	/* return parameters */
	lua_newtable(L);

	for(i = 0; i < size; i++)
		set_field(L, i+1, buffer[i]);

	free(buffer);

	return 1;
}

/*ShadeModel (mode) -> none*/
static int gl_shade_model(lua_State *L)
{
	//GLenum mode;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.ShadeModel'");

	/* get string parameter */
	//mode = get_gl_enum(L, 1);

	/* test argument */
	//if(mode == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.ShadeModel'");

	/* call opengl function */
	glShadeModel((GLenum)lua_tointeger(L, 1));

	return 0;
}

/*StencilFunc (func, ref, mask) -> none*/
static int gl_stencil_func(lua_State *L)
{
	//GLenum func;

	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.StencilFunc'");

	/* get string parameter */
	//func = get_gl_enum(L, 1);

	/* test argument */
	//if(func == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.StencilFunc'");

	if(lua_type(L,3) == LUA_TSTRING)
		/* call opengl function */
		glStencilFunc((GLenum)lua_tointeger(L, 1), (GLint)lua_tonumber(L, 2), str2mask(lua_tostring(L, 3)));

	else if(lua_type(L,3) == LUA_TNUMBER)
		/* call opengl function */
		glStencilFunc((GLenum)lua_tointeger(L, 1), (GLint)lua_tonumber(L, 2), (GLuint)lua_tonumber(L, 3));

	else
		luaL_error(L, "incorrect argument to function 'gl.StencilFunc'");

	return 0;
}

/*StencilMask (mask) -> none*/
static int gl_stencil_mask(lua_State *L)
{
	if(lua_type(L,1) == LUA_TSTRING)
		/* call opengl function */
		glStencilMask(str2mask(lua_tostring(L, 1)));

	else if(lua_type(L,1) == LUA_TNUMBER)
		/* call opengl function */
		glStencilMask((GLuint)lua_tonumber(L, 1));

	else
		luaL_error(L, "incorrect argument to function 'gl.StencilMask'");

	return 0;
}

/*StencilOp (fail, zfail, zpass) -> none*/
static int gl_stencil_op(lua_State *L)
{
	//GLenum e1, e2, e3;

	/* test arguments type */
	if( !(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
		luaL_error(L, "incorrect argument to function 'gl.StencilOp'");

	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);
	//e3 = get_gl_enum(L, 3);

	/* test argument */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR || e3 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.StencilOp'");

	/* call opengl function */
	glStencilOp((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2), (GLenum)lua_tointeger(L, 3));

	return 0;
}

/*TexCoord (s[, t, r, q]) -> none
  TexCoord (vArray) -> none*/
static int gl_tex_coord(lua_State *L)
{
	int index;
	int num_args = lua_gettop(L);

	GLdouble *v = 0;

	/* if more then 4 arguments, ignore the others */
	if(num_args > 4)
		num_args = 4;

	/* if have there's no arguments show an error message */
	if(num_args == 0)
		luaL_error(L, "incorrect argument to function 'gl.TexCoord'");

	/* test argument type */
	if(lua_istable(L, 1))
		num_args = get_arrayd(L, 1, &v);

	else
	{
		v = (GLdouble *)malloc(num_args * sizeof(GLdouble));

		/* get arguments */
		for(index = 0; index < num_args; index++)
		{
			/* test arguments type */
			if(!lua_isnumber(L, index + 1))
				luaL_error(L, "incorrect argument to function 'gl.TexCoord'");

			/* get argument */
			v[index] = lua_tonumber(L, index + 1);
		}
	}

	/* call openGL functions */
	switch(num_args)
	{
		case 1:  glTexCoord1dv((GLdouble *)v);  break;
		case 2:  glTexCoord2dv((GLdouble *)v);  break;
		case 3:  glTexCoord3dv((GLdouble *)v);  break;
		case 4:  glTexCoord4dv((GLdouble *)v);  break;
		default: break;
	}

	free(v);

	return 0;
}

/* void glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
 *
 * TexCoordPointer (colorArray [,size [,stride]]) -> none
 * TexCoordPointer (bufferOffset, size [,stride]) -> none
 */
static int gl_tex_coord_pointer(lua_State *L)
{
	GLint size, offset;
	GLsizei stride = 0;
	static GLfloat *array = 0;

	if(array)
		free(array);

	if (lua_isnumber(L, 1)) {
		offset = lua_tointeger(L, 1);
		if (!lua_isnumber(L, 2)) {
			luaL_error(L, "incorrect argument to function 'gl.TexCoordPointer'");
		}
		size = lua_tointeger(L, 2);
		
		if (lua_isnumber(L, 3)) {
			stride = lua_tointeger(L, 3);
		}

		glTexCoordPointer(size, GL_FLOAT, stride * sizeof(GLfloat), (GLubyte*) NULL + offset * sizeof(GLfloat));
	}
	else if (lua_istable(L, 1)) {
		if(lua_isnumber(L, 2))
		{
		  size = (GLint)lua_tonumber(L, 2);
		  get_arrayf(L, 1, &array);
		}
		else if(get_array2f(L, 1, &array, &size) == -1)
		{
			luaL_error(L, "incorrect argument to function 'gl.TexCoordPointer'");
			return 0;
		}

		if (lua_isnumber(L, 3)) {
			stride = lua_tointeger(L, 3);
		}

		glTexCoordPointer(size, GL_FLOAT, stride * sizeof(GLfloat), array);
	}
	else {
		luaL_error(L, "incorrect argument to function 'gl.TexCoordPointer'");
	}

	return 0;
}

/*TexEnv (pname, param, [target]) -> none
  TexEnv (pname, paramsArray, [target]) -> none*/
int static gl_tex_env(lua_State *L)
{
	GLfloat *param;
	//GLenum e, et;
	GLenum et;

	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.TexEnv'");

	/* get string parameters */
	//e = get_gl_enum(L, 1);

	if (lua_isstring(L, 3))
		et = lua_tointeger(L, 3); //get_gl_enum(L, 3);
	else
		et = GL_TEXTURE_ENV;

	/* test argument */
	//if(e == ENUM_ERROR || et == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.TexEnv'");

	if(lua_istable(L, 2))
	{
		get_arrayf(L, 2, &param);

		/* call opengl function */
		glTexEnvfv(et, (GLenum)lua_tointeger(L, 1), (GLfloat *)param);

		free(param);
	}
	else if(lua_isnumber(L, 2))
		/* call opengl function */
		glTexEnvf(et, (GLenum)lua_tointeger(L, 1), (GLfloat)lua_tonumber(L, 2));

	else if(lua_isstring(L, 2))
		/* call opengl function */
		glTexEnvi(et, (GLenum)lua_tointeger(L, 1), get_gl_enum(L, 2));

	else
		luaL_error(L, "incorrect argument to function 'gl.TexEnv'");

	return 0;
}

/*TexGen (coord, pname, param) -> none
  TexGen (coord, pname, paramsArray) -> none*/
int static gl_tex_gen(lua_State *L)
{
	//GLenum e1, e2;
	GLdouble *param;

	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.TexGen'");

	/* get string parameters */
	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);

	/* test argument */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.TexGen'");

	if(lua_istable(L, 3))
	{
		get_arrayd(L, 3, &param);

		/* call opengl function */
		glTexGendv((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2), (GLdouble *)param);

		free(param);
	}
	else if(lua_isstring(L, 3))
		/* call opengl function */
		glTexGeni((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2), get_gl_enum(L, 3));

	else
		luaL_error(L, "incorrect argument to function 'gl.TexGen'");
	return 0;
}


/*TexParameter (target, pname, param) -> none
  TexParameter (target, pname, paramsArray) -> none*/
static int gl_tex_parameter(lua_State *L)
{
	//GLenum e1, e2;
	GLenum e2;
	GLfloat *param;

	/* test arguments type */
	if(! (lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
		luaL_error(L, "incorrect argument to function 'gl.TexParameter'");

	/* get string parameters */
	//e1 = get_gl_enum(L, 1);
	//e2 = get_gl_enum(L, 2);
	e2 = lua_tointeger(L, 2);

	/* test argument */
	//if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.TexParameter'");

	switch (e2)
	{
		case GL_TEXTURE_MIN_LOD:
		case GL_TEXTURE_MAX_LOD:
		case GL_TEXTURE_PRIORITY:
			if (!lua_isnumber(L, 3))
				luaL_error(L, "incorrect argument to function 'gl.TexParameter'");

			glTexParameterf((GLenum)lua_tointeger(L, 1), e2, (GLfloat)lua_tonumber(L, 3));
		break;

		case GL_TEXTURE_BORDER_COLOR:
			if (!lua_istable(L, 3))
				luaL_error(L, "incorrect argument to function 'gl.TexParameter'");
			
			get_arrayf(L, 3, &param);
			glTexParameterfv((GLenum)lua_tointeger(L, 1), e2, (GLfloat *)param);
			free(param);
		break;

		default:
			if (!lua_isnumber(L, 3))
				luaL_error(L, "incorrect argument to function 'gl.TexParameter'");

			glTexParameteri((GLenum)lua_tointeger(L, 1), e2, (GLint)lua_tointeger(L, 3));
		break;
	}

	return 0;
}

/*Translate (x, y, z) -> none*/
static int gl_translate(lua_State *L)
{
	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
		luaL_error(L, "incorrect argument to function 'gl.Translate'");

	/* call opengl function */
	glTranslated((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
					 (GLdouble)lua_tonumber(L, 3));

	return 0;
}

/*Vertex (x, y, [z, w]) -> none
  Vertex (v) -> none*/
static int gl_vertex(lua_State *L)
{
	int index;
	int num_args = lua_gettop(L);

	GLdouble *v;

	/* if have there's no arguments show an error message */
	if(num_args == 0)
		luaL_error(L, "incorrect argument to function 'gl.Vertex'");

	/* test argument type */
	if(lua_istable(L, 1))
		num_args = get_arrayd(L, 1, &v);

	else
	{
		/* test number of arguments */
		if(num_args < 2)
			luaL_error(L, "incorrect argument to function 'gl.Vertex'");

		v = (GLdouble *)malloc(num_args * sizeof(GLdouble));

		/* get arguments */
		for(index = 0; index < num_args; index++)
		{
			/* test arguments type */
			if(!lua_isnumber(L, index + 1))
				luaL_error(L, "incorrect argument to function 'gl.Vertex'");

			/* get argument */
			v[index] = (GLdouble)lua_tonumber(L, index + 1);
		}
	}

	/* if more then 4 arguments, ignore the others */
	if(num_args > 4)
		num_args = 4;

	/* call openGL functions */
	switch(num_args)
	{
		case 2:  glVertex2dv((GLdouble *)v);  break;
		case 3:  glVertex3dv((GLdouble *)v);  break;
		case 4:  glVertex4dv((GLdouble *)v);  break;
	}

	free(v);

	return 0;
}

/* void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
 *
 * VertexPointer (vertexArray [,size [,stride]]) -> none
 * VertexPointer (bufferOffset, size [,stride]) -> none
 */
static int gl_vertex_pointer(lua_State *L)
{
	GLint size, offset;
	GLsizei stride = 0;
	static GLfloat *array = 0;

	if(array)
		free(array);

	if (lua_isnumber(L, 1)) {
		offset = lua_tointeger(L, 1);
		if (!lua_isnumber(L, 2)) {
			luaL_error(L, "incorrect argument to function 'gl.VertexPointer'");
		}
		size = lua_tointeger(L, 2);
		
		if (lua_isnumber(L, 3)) {
			stride = lua_tointeger(L, 3);
		}

		glVertexPointer(size, GL_FLOAT, stride * sizeof(GLfloat), (GLubyte*) NULL + offset * sizeof(GLfloat));
	}
	else if (lua_istable(L, 1)) {
		if(lua_isnumber(L, 2))
		{
		  size = (GLint)lua_tonumber(L, 2);
		  get_arrayf(L, 1, &array);
		}
		else if(get_array2f(L, 1, &array, &size) == -1)
		{
			luaL_error(L, "incorrect argument to function 'gl.VertexPointer'");
			return 0;
		}

		if (lua_isnumber(L, 3)) {
			stride = lua_tointeger(L, 3);
		}

		glVertexPointer(size, GL_FLOAT, stride * sizeof(GLfloat), array);
	}
	else {
		luaL_error(L, "incorrect argument to function 'gl.VertexPointer'");
	}

	return 0;
}

/*Viewport (x, y, width, height) -> none*/
static int gl_viewport(lua_State *L)
{
	/* test arguments type */
	if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
			lua_isnumber(L, 3) && lua_isnumber(L, 4) ))
		luaL_error(L, "incorrect argument to function 'gl.Viewport'");

	/* call openGL function */
	glViewport((GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 2),
				  (GLsizei)lua_tonumber(L, 3), (GLsizei)lua_tonumber(L, 4));

	return 0;
}

extern int gl_active_texture(lua_State *L);
extern int gl_attach_shader (lua_State *L);
extern int gl_bind_attrib_location (lua_State *L);
extern int gl_blend_equation_separate(lua_State *L);
extern int gl_blend_func_separate(lua_State *L);
extern int gl_client_active_texture(lua_State *L);
extern int gl_compile_shader(lua_State *L);
extern int gl_create_program (lua_State *L);
extern int gl_create_shader(lua_State *L);
extern int gl_delete_program (lua_State *L);
extern int gl_delete_shader(lua_State *L);
extern int gl_detach_shader (lua_State *L);
extern int gl_disable_vertex_attrib_array (lua_State *L);
extern int gl_enable_vertex_attrib_array (lua_State *L);
extern int gl_fog_coord(lua_State *L);
extern int gl_fog_coord_pointer(lua_State *L);
extern int gl_get_attrib_location (lua_State *L);
extern int gl_get_program (lua_State *L);
extern int gl_get_program_info_log (lua_State *L);
extern int gl_get_shader (lua_State *L);
extern int gl_get_shader_info_log (lua_State *L);
extern int gl_get_shader_source (lua_State *L);
extern int gl_get_uniform_location (lua_State *L);
extern int gl_is_buffer(lua_State *L);
extern int gl_is_program (lua_State *L);
extern int gl_is_shader (lua_State *L);
extern int gl_link_program (lua_State *L);
extern int gl_multi_tex_coord(lua_State *L);
extern int gl_point_parameter(lua_State *L);
extern int gl_secondary_color(lua_State *L);
extern int gl_secondary_color_pointer(lua_State *L);
extern int gl_shader_source(lua_State *L);
extern int gl_uniform_f (lua_State *L);
extern int gl_uniform_i (lua_State *L);
extern int gl_uniform_matrix (lua_State *L);
extern int gl_use_program (lua_State *L);
extern int gl_validate_program (lua_State *L);
extern int gl_vertex_attrib_f (lua_State *L);
extern int gl_vertex_attrib_pointer(lua_State *L);
extern int gl_window_pos (lua_State *L);

extern int gl_bind_buffer(lua_State *L);
extern int gl_blend_color (lua_State *L);
extern int gl_blend_equation (lua_State *L);
extern int gl_buffer_data(lua_State *L);
extern int gl_buffer_sub_data(lua_State *L);
extern int gl_copy_tex_image_1d (lua_State *L);
extern int gl_copy_tex_image_2d (lua_State *L);
extern int gl_copy_tex_sub_image_1d (lua_State *L);
extern int gl_copy_tex_sub_image_2d (lua_State *L);
extern int gl_copy_tex_sub_image_3d (lua_State *L);
extern int gl_delete_buffers(lua_State *L);
extern int gl_draw_pixels (lua_State *L);
extern int gl_gen_buffers(lua_State *L);
extern int gl_tex_image_1d (lua_State *L);
extern int gl_tex_image_2d (lua_State *L);
extern int gl_tex_image_3d (lua_State *L);
extern int gl_tex_sub_image_1d (lua_State *L);
extern int gl_tex_sub_image_2d (lua_State *L);
extern int gl_tex_sub_image_3d (lua_State *L);

extern int gl_test (lua_State *L);

const luaL_reg gllib[] = {
  {"Accum", gl_accum},
  {"ActiveTexture", gl_active_texture},
  {"AlphaFunc", gl_alpha_func},
  {"AreTexturesResident", gl_are_textures_resident},
  {"ArrayElement", gl_array_element},
  {"AttachShader", gl_attach_shader},
  {"Begin", gl_begin},
  {"BindAttribLocation", gl_bind_attrib_location},
  {"BindBuffer", gl_bind_buffer},
  {"BindTexture", gl_bind_texture},
  {"Bitmap", gl_bitmap},
  {"BlendColor", gl_blend_color},
  {"BlendEquation", gl_blend_equation},
  {"BlendEquationSeparate", gl_blend_equation_separate},
  {"BlendFunc", gl_blend_func},
  {"BlendFuncSeparate", gl_blend_func_separate},
  {"BufferData", gl_buffer_data},
  {"BufferSubData", gl_buffer_sub_data},
  {"CallList", gl_call_list},
  {"CallLists", gl_call_lists},
  {"Clear", gl_clear},
  {"ClearAccum", gl_clear_accum},
  {"ClearColor", gl_clear_color},
  {"ClearDepth", gl_clear_depth},
  {"ClearIndex", gl_clear_index},
  {"ClearStencil", gl_clear_stencil},
  {"ClientActiveTexture", gl_client_active_texture},
  {"ClipPlane", gl_clip_plane},
  {"Color", gl_color},
  {"ColorMask", gl_color_mask},
  {"ColorMaterial", gl_color_material},
  {"ColorPointer", gl_color_pointer},
  {"CompileShader", gl_compile_shader},
  {"CopyPixels", gl_copy_pixels},
  {"CopyTexImage1D", gl_copy_tex_image_1d},
  {"CopyTexImage2D", gl_copy_tex_image_2d},
  {"CopyTexSubImage1D", gl_copy_tex_sub_image_1d},
  {"CopyTexSubImage2D", gl_copy_tex_sub_image_2d},
  {"CopyTexSubImage3D", gl_copy_tex_sub_image_3d},
  {"CreateProgram", gl_create_program},
  {"CreateShader", gl_create_shader},
  {"CullFace",gl_cull_face},
  {"DeleteBuffers", gl_delete_buffers},
  {"DeleteLists",gl_delete_lists},
  {"DeleteProgram", gl_delete_program},
  {"DeleteShader", gl_delete_shader},
  {"DeleteTextures",gl_delete_textures},
  {"DepthFunc",gl_depth_func},
  {"DepthMask",gl_depth_mask},
  {"DepthRange",gl_depth_range},
  {"DetachShader", gl_detach_shader},
  {"Disable",gl_disable},
  {"DisableClientState",gl_disable_client_state},
  {"DisableVertexAttribArray", gl_disable_vertex_attrib_array},
  {"DrawArrays",gl_draw_arrays},
  {"DrawBuffer",gl_draw_buffer},
  {"DrawElements", gl_draw_elements},
  {"DrawPixels", gl_draw_pixels},
  {"EdgeFlag", gl_edge_flag},
  {"EdgeFlagPointer", gl_edge_flag_pointer},
  {"Enable", gl_enable},
  {"EnableClientState", gl_enable_client_state},
  {"EnableVertexAttribArray", gl_enable_vertex_attrib_array},
  {"End", gl_end},
  {"EndList", gl_end_list},
  {"EvalCoord", gl_eval_coord},
  {"EvalMesh", gl_eval_mesh},
  {"EvalPoint", gl_eval_point},
  {"FeedbackBuffer", gl_feedback_buffer},
  {"Finish", gl_finish},
  {"Flush", gl_flush},
  {"Fog", gl_fog},
  {"FogCoord", gl_fog_coord},
  {"FogCoordPointer", gl_fog_coord_pointer},
  {"FrontFace", gl_front_face},
  {"Frustum", gl_frustum},
  {"GenLists", gl_gen_lists},
  {"GenBuffers", gl_gen_buffers},
  {"GenTextures", gl_gen_textures},
  {"Get", gl_get},
  {"GetArray", gl_get_array},
  {"GetAttribLocation", gl_get_attrib_location},
  {"GetConst", gl_get_const},
  {"GetClipPlane", gl_get_clip_plane},
  {"GetError", gl_get_error},
  {"GetLight", gl_get_light},
  {"GetMap", gl_get_map},
  {"GetMaterial", gl_get_material},
  {"GetPixelMap", gl_get_pixel_map},
  {"GetPointer", gl_get_pointer},
  {"GetPolygonStipple", gl_get_polygon_stipple},
  {"GetProgram", gl_get_program},
  {"GetProgramInfoLog", gl_get_program_info_log},
  {"GetShader", gl_get_shader},
  {"GetShaderInfoLog", gl_get_shader_info_log},
  {"GetShaderSource", gl_get_shader_source},
  {"GetString", gl_get_string},
  {"GetUniformLocation", gl_get_uniform_location},
  {"GetTexEnv", gl_get_tex_env},
  {"GetTexGen", gl_get_tex_gen},
  {"GetTexImage", gl_get_tex_image},
  {"GetTexLevelParameter", gl_get_tex_level_parameter},
  {"GetTexParameter", gl_get_tex_parameter},
  {"Hint", gl_hint},
  {"Index", gl_index},
  {"IndexMask", gl_index_mask},
  {"IndexPointer", gl_index_pointer},
  {"InitNames", gl_init_names},
  {"IsEnabled", gl_is_enabled},
  {"IsBuffer", gl_is_buffer},
  {"IsList", gl_is_list},
  {"IsProgram", gl_is_program},
  {"IsShader", gl_is_shader},
  {"IsTexture", gl_is_texture},
  {"Light", gl_light},
  {"LightModel", gl_light_model},
  {"LineStipple", gl_line_stipple},
  {"LineWidth", gl_line_width},
  {"LinkProgram", gl_link_program},
  {"ListBase", gl_list_base},
  {"LoadIdentity", gl_load_identity},
  {"LoadMatrix", gl_load_matrix},
  {"LoadName", gl_load_name},
  {"LogicOp", gl_logic_op},
  {"Map", gl_map},
  {"MapGrid", gl_map_grid},
  {"Material", gl_material},
  {"MatrixMode", gl_matrix_mode},
  {"MultMatrix", gl_mult_matrix},
  {"MultiTexCoord", gl_multi_tex_coord},
  {"NewList", gl_new_list},
  {"Normal", gl_normal},
  {"NormalPointer", gl_normal_pointer},
  {"Ortho", gl_ortho},
  {"PassThrough", gl_pass_through},
  {"PixelMap", gl_pixel_map},
  {"PixelStore", gl_pixel_store},
  {"PixelTransfer", gl_pixel_transfer},
  {"PixelZoom", gl_pixel_zoom},
  {"PointParameter", gl_point_parameter},
  {"PointSize", gl_point_size},
  {"PolygonMode", gl_polygon_mode},
  {"PolygonOffset", gl_polygon_offset},
  {"PolygonStipple", gl_polygon_stipple},
  {"PopAttrib", gl_pop_attrib},
  {"PopClientAttrib", gl_pop_client_attrib},
  {"PopMatrix", gl_pop_matrix},
  {"PopName", gl_pop_name},
  {"PrioritizeTextures", gl_prioritize_textures},
  {"PushAttrib", gl_push_attrib},
  {"PushClientAttrib", gl_push_client_attrib},
  {"PushMatrix", gl_push_matrix},
  {"PushName", gl_push_name},
  {"RasterPos", gl_raster_pos},
  {"ReadBuffer", gl_read_buffer},
  {"ReadPixels", gl_read_pixels},
  {"Rect", gl_rect},
  {"RenderMode", gl_render_mode},
  {"Rotate", gl_rotate},
  {"Scale", gl_scale},
  {"Scissor", gl_scissor},
  {"SecondaryColor", gl_secondary_color},
  {"SecondaryColorPointer", gl_secondary_color_pointer},
  {"SelectBuffer", gl_select_buffer},
  {"ShadeModel", gl_shade_model},
  {"ShaderSource", gl_shader_source},
  {"StencilFunc", gl_stencil_func},
  {"StencilMask", gl_stencil_mask},
  {"StencilOp", gl_stencil_op},
  {"TexCoord", gl_tex_coord},
  {"TexCoordPointer", gl_tex_coord_pointer},
  {"TexEnv", gl_tex_env},
  {"TexGen", gl_tex_gen},
  {"TexImage1D", gl_tex_image_1d},
  {"TexImage2D", gl_tex_image_2d},
  {"TexImage3D", gl_tex_image_3d},
  {"TexSubImage1D", gl_tex_sub_image_1d},
  {"TexSubImage2D", gl_tex_sub_image_2d},
  {"TexSubImage3D", gl_tex_sub_image_3d},
  {"TexParameter", gl_tex_parameter},
  {"Translate", gl_translate},
  {"Uniformf", gl_uniform_f},
  {"Uniformi", gl_uniform_i},
  {"UniformMatrix", gl_uniform_matrix},
  {"UseProgram", gl_use_program},
  {"ValidateProgram", gl_validate_program},
  {"Vertex", gl_vertex},
  {"VertexAttribf", gl_vertex_attrib_f},
  {"VertexAttribPointer", gl_vertex_attrib_pointer},
  {"VertexPointer", gl_vertex_pointer},
  {"Viewport", gl_viewport},
  {"WindowPos", gl_window_pos},
  {"Test", gl_test},
  {NULL, NULL}
};

extern int glu_look_at (lua_State *L);
extern int glu_ortho_2d (lua_State *L);
extern int glu_perspective (lua_State *L);
extern int glu_pick_matrix (lua_State *L);
extern int glu_project (lua_State *L);
extern int glu_unproject (lua_State *L);

const luaL_reg glulib[] = {
	{"LookAt", glu_look_at},
	{"Ortho2D", glu_ortho_2d},
	{"Perspective", glu_perspective},
	{"PickMatrix", glu_pick_matrix},
	{"Project", glu_project},
	{"UnProject", glu_unproject},
	{NULL, NULL}
};

LUAGL_API int luaopen_opengl (lua_State *L) {
	const gl_str_value* v;

	luaL_openlib(L, "glu", glulib, 0);

	for (v = glu_str; v->str != 0; v++) {
		lua_pushinteger(L, v->value);
		lua_setfield(L, -2, v->str);
	}

	luaL_openlib(L, "gl", gllib, 0);

	for (v = gl_str; v->str != 0; v++) {
		lua_pushinteger(L, v->value);
		lua_setfield(L, -2, v->str);
	}

	return 1;
}
