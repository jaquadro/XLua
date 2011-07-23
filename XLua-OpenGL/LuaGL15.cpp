#include <string.h>
#include <malloc.h>

#include "LuaUtil.h"

extern "C" {

	/* void glBindBuffer (GLenum target, GLuint buffer)
	 *
	 * BindBuffer (target, buffer) -> none
	 */
	int gl_bind_buffer(lua_State *L) {

		if (!(	lua_isnumber(L, 1) &&		// target
				lua_isnumber(L, 2)			// buffer
			)) {
			luaL_error(L, "incorrect argument to function 'gl.BindBuffer'");
		}

		glBindBuffer((GLenum)lua_tointeger(L, 1), (GLuint)lua_tointeger(L, 2));

		return 0;
	}


	/* void glBufferData (Glenum target, GLsizeptr size, const GLvoid* data, GLenum usage)
	 *
	 * BufferData (target, size, data, usage, [type]) -> none
	 */
	int gl_buffer_data(lua_State *L) {

		if (!(	lua_isnumber(L, 1) &&							// target
				(lua_isnumber(L, 2) || lua_isnil(L, 2)) &&		// size
				(lua_istable(L, 3) || lua_isuserdata(L, 3) || lua_isnil(L, 3)) &&	// data
				lua_isnumber(L, 4)								// usage
			)) {
			luaL_error(L, "Incorrect argument to function 'gl.BufferData'");
		}

		GLsizeiptr size = lua_tointeger(L, 2);
		const void* data = 0;

		if (lua_isuserdata(L, 3)) {
			ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 3, "lib.array");
			if (!aud->arr)
				luaL_error(L, "Invalid static array passed to function 'gl.BufferData'");

			if (lua_isnil(L, 2))
				size = aud->arr->size() * aud->arr->count();
			else if (size > aud->arr->size() * aud->arr->count())
				luaL_error(L, "Requested size larger than static array in function 'gl.BufferData'");

			data = aud->arr->raw();
		}
		else if (lua_istable(L, 3)) {
			if (!lua_isnumber(L, 5))
				luaL_error(L, "Passing a data table requires a type value in function 'gl.BufferData'");

			GLenum type = lua_tointeger(L, 5);
			data = GLCommon::getDataFromTable(L, 3, type, &size, "gl.BufferData");
		}

		glBufferData((GLenum)lua_tointeger(L, 1), size, data, (GLenum)lua_tointeger(L, 4));

		if (lua_istable(L, 3))
			delete[] data;

		return 0;
	}


	/* void glBufferSubData (Glenum target, GLintptr offset, GLsizeptr size, const GLvoid* data)
	 *
	 * BufferSubData (target, offset, size, data, [type]) -> none
	 */
	int gl_buffer_sub_data(lua_State *L) {

		if (!(	lua_isnumber(L, 1) &&							// target
				lua_isnumber(L, 2) &&							// offset
				(lua_isnumber(L, 3) || lua_isnil(L, 3)) &&		// size
				(lua_istable(L, 4) || lua_isuserdata(L, 4))		// data
			)) {
			luaL_error(L, "Incorrect argument to function 'gl.BufferSubData'");
		}

		GLsizeiptr size = lua_tointeger(L, 3);
		const void* data = 0;

		if (lua_isuserdata(L, 3)) {
			ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 4, "lib.array");
			if (!aud->arr)
				luaL_error(L, "Invalid static array passed to function 'gl.BufferSubData'");

			if (lua_isnil(L, 3))
				size = aud->arr->size() * aud->arr->count();
			else if (size > aud->arr->size() * aud->arr->count())
				luaL_error(L, "Requested size larger than static array in function 'gl.BufferSubData'");

			data = aud->arr->raw();
		}
		else if (lua_istable(L, 3)) {
			if (!lua_isnumber(L, 5))
				luaL_error(L, "Passing a data table requires a type value in function 'gl.BufferSubData'");

			GLenum type = lua_tointeger(L, 5);
			data = GLCommon::getDataFromTable(L, 4, type, &size, "gl.BufferSubData");
		}

		glBufferSubData((GLenum)lua_tointeger(L, 1), (GLintptr)lua_tointeger(L, 2), size, data);

		if (lua_istable(L, 4))
			delete[] data;

		return 0;
	}


	/* void glDeleteBuffers (GLsizei n, const GLuint* buffers)
	 *
	 * DeleteBuffers (buffers) -> none
	 */
	int gl_delete_buffers (lua_State *L) {

		if (!(lua_istable(L, 1))) {
			luaL_error(L, "Incorrect argument to function 'gl.DeleteBuffers'");
		}

		GLsizei size = 0;
		GLuint* buffers = 0;

		getArray1ui(L, 1, &buffers, &size);

		glDeleteBuffers(size, buffers);

		delete[] buffers;

		return 0;
	}


	/* void glGenBuffers (GLsizei n, GLuint* buffers)
	 *
	 * GenBuffers (n) -> buffersArray
	 */
	int gl_gen_buffers (lua_State *L) {

		if(!lua_isnumber(L, 1)) {
			luaL_error(L, "Incorrect argument to function 'gl.GenBuffers'");
		}

		GLsizei size = lua_tointeger(L, 1);
		GLuint* buffers = new GLuint[size];

		glGenBuffers(size, buffers);

		lua_newtable(L);
		for(int i = 0; i < size; i++)
			setField(L, i + 1, buffers[i]);

		delete[] buffers;

		return 1;
	}


	/* GLboolean glIsBuffer (GLuint buffer)
	 *
	 * IsBuffer (buffer) -> true/false
	 */
	int gl_is_buffer (lua_State *L) {

		if(!lua_isnumber(L, 1)) {
			luaL_error(L, "incorrect argument to function 'gl.IsBuffer'");
		}

		lua_pushboolean(L, glIsBuffer((GLuint)lua_tointeger(L, 1)));

		return 1;
	}

}
