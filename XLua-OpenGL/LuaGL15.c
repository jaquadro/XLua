#include <string.h>
#include <malloc.h>

#include "lauxlib.h"
#include "LuaGL.h"

/* void glBindBuffer (GLenum target, GLuint buffer)
 *
 * BindBuffer (target, buffer) -> none
 */
int gl_bind_buffer(lua_State *L)
{
	//GLenum e;

	/* test arguments */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.BindBuffer'");

	/* get string value */
	//e = get_gl_enum(L, 1);

	/* test arguments */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.BindBuffer'");

	/* call opengl function */
	glBindBuffer((GLenum)lua_tointeger(L, 1), (GLuint)lua_tointeger(L, 2));

	return 0;
}

/* void glBufferData (Glenum target, GLsizeptr size, const GLvoid* data, GLenum usage)
 *
 * BufferData (target, data, usage [,size]) -> none
 */
int gl_buffer_data(lua_State *L)
{
	//GLenum target, usage;
	GLenum target;
	GLint size = 1, n, fullsize;
	static GLfloat *array = 0;
	static GLuint *uiarray = 0;

	if(array)
		free(array);
	if (uiarray)
		free(uiarray);

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_istable(L, 2) && lua_isnumber(L, 3)))
		luaL_error(L, "incorrect argument to function 'gl.BufferData'");

	/* get parameters */
	//target = get_gl_enum(L, 1);
	target = lua_tointeger(L, 1);
	//usage = get_gl_enum(L, 3);

	/* test arguments */
	//if(target == ENUM_ERROR || usage == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.BufferData'");

	/* Get data */
	if (target == GL_ARRAY_BUFFER) {
		n = get_array2f(L, 2, &array, &size);
		if (n == -1) {
			n = get_arrayf(L, 2, &array);
		}
	}
	else {
		n = get_array2ui(L, 2, &uiarray, &size);
		if (n == -1) {
			n = get_arrayui(L, 2, &uiarray);
		}
	}

	/* Optional size specification */
	if (lua_isnumber(L, 4)) {
		fullsize = lua_tointeger(L, 4);
		if (fullsize > n * size) {
			fullsize = n * size;
		}
	}
	else {
		fullsize = n * size;
	}

	/* call opengl function */
	if (target == GL_ARRAY_BUFFER) {
		glBufferData(target, (GLuint)(fullsize * sizeof(GLfloat)), (GLvoid*)array, (GLenum)lua_tointeger(L, 3));
	}
	else {
		glBufferData(target, (GLuint)(fullsize * sizeof(GLuint)), (GLvoid*)uiarray, (GLenum)lua_tointeger(L, 3));
	}

	return 0;
}

/* void glBufferSubData (Glenum target, GLintptr offset, GLsizeptr size, const GLvoid* data)
 *
 * BufferSubData (target, offset, data [,size]) -> none
 */
int gl_buffer_sub_data(lua_State *L)
{
	GLenum target;
	GLint offset, size = 1, n, fullsize;
	static GLfloat *array = 0;
	static GLuint *uiarray = 0;

	if(array)
		free(array);
	if (uiarray)
		free(uiarray);

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2), lua_istable(L, 3)))
		luaL_error(L, "incorrect argument to function 'gl.BufferSubData'");

	/* get parameters */
	//target = get_gl_enum(L, 1);
	target = lua_tointeger(L, 1);
	offset = lua_tointeger(L, 2);

	/* test arguments */
	//if(target == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.BufferSubData'");

	/* Get data */
	if (target == GL_ARRAY_BUFFER) {
		n = get_array2f(L, 3, &array, &size);
		if (n == -1) {
			n = get_arrayf(L, 3, &array);
		}
	}
	else {
		n = get_array2ui(L, 3, &uiarray, &size);
		if (n == -1) {
			n = get_arrayui(L, 3, &uiarray);
		}
	}

	/* Optional size specification */
	if (lua_isnumber(L, 4)) {
		fullsize = lua_tointeger(L, 4);
		if (fullsize > n * size) {
			fullsize = n * size;
		}
	}
	else {
		fullsize = n * size;
	}

	/* call opengl function */
	if (target == GL_ARRAY_BUFFER) {
		glBufferSubData(target, (GLintptr)offset, (GLsizeiptr)(fullsize * sizeof(GLfloat)), (GLvoid*)array);
	}
	else {
		glBufferSubData(target, (GLintptr)offset, (GLsizeiptr)(fullsize * sizeof(GLuint)), (GLvoid*)uiarray);
	}

	return 0;
}

/* void glDeleteBuffers (GLsizei n, const GLuint* buffers)
 *
 * DeleteBuffers (buffers) -> none
 */
int gl_delete_buffers(lua_State *L)
{
	int n;
	GLuint *buffers;

	/* test argument type */
	if(!lua_istable(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.DeleteBuffers'");

	/* get textures array */
	n = get_arrayui(L, 1, &buffers);

	/* call opengl function */
	glDeleteBuffers((GLsizei)n, (GLuint *)buffers);

	free(buffers);

	return 0;
}

/* void glGenBuffers (GLsizei n, GLuint* buffers)
 *
 * GenBuffers (n) -> buffersArray
 */
int gl_gen_buffers(lua_State *L)
{
	int i;
	GLsizei n;
	GLuint *buffers;

	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GenBuffers'");

	n = (GLsizei)lua_tonumber(L, 1);
	buffers = (GLuint *)malloc(n * sizeof(GLuint));

	/* call opengl function */
	glGenBuffers(n, (GLuint*)buffers);

	lua_newtable(L);

	for(i = 0; i < n; i++)
		set_field(L, i+1, buffers[i]);

	free(buffers);

	return 1;
}

/* GLboolean glIsBuffer (GLuint buffer)
 *
 * IsBuffer (buffer) -> true/false
 */
int gl_is_buffer(lua_State *L)
{
	/* test argument type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.IsBuffer'");

	/* call opengl function and push return value in the lua stack */
	lua_pushboolean(L, glIsBuffer((GLuint)lua_tonumber(L, 1)));

	return 1;
}