#include <string.h>
#include <malloc.h>

#include "LuaUtil.h"

extern "C" {

	/* void glCopyTexImage1D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y,
	 *						  GLsizei width, GLint border)
     *
	 * CopyTexImage1D (target, level, internalFormat, x, y, width, border)
	 */
	int gl_copy_tex_image_1d (lua_State* L) {
		
		if (!(	lua_isnumber(L, 1) &&				// target
				lua_isnumber(L, 2) &&				// level
				lua_isnumber(L, 3) &&				// internalFormat
				lua_isnumber(L, 4) &&				// x
				lua_isnumber(L, 5) &&				// y
				lua_isnumber(L, 6) &&				// width
				lua_isnumber(L, 7)					// border
			)) {
			luaL_error(L, "Incorrect argument to function gl.CopyTexImage1D");
		}

		glCopyTexImage1D((GLenum)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLenum)lua_tointeger(L, 3),
			(GLint)lua_tointeger(L, 4), (GLint)lua_tointeger(L, 5), (GLsizei)lua_tointeger(L, 6),
			(GLint)lua_tointeger(L, 8));

		return 0;
	}


	/* void glCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y,
	 *						  GLsizei width, GLsizei height, GLint border)
     *
	 * CopyTexImage2D (target, level, internalFormat, x, y, width, height, border)
	 */
	int gl_copy_tex_image_2d (lua_State* L) {
		
		if (!(	lua_isnumber(L, 1) &&				// target
				lua_isnumber(L, 2) &&				// level
				lua_isnumber(L, 3) &&				// internalFormat
				lua_isnumber(L, 4) &&				// x
				lua_isnumber(L, 5) &&				// y
				lua_isnumber(L, 6) &&				// width
				lua_isnumber(L, 7) &&				// height
				lua_isnumber(L, 8)					// border
			)) {
			luaL_error(L, "Incorrect argument to function gl.CopyTexImage2D");
		}

		glCopyTexImage2D((GLenum)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLenum)lua_tointeger(L, 3),
			(GLint)lua_tointeger(L, 4), (GLint)lua_tointeger(L, 5), (GLsizei)lua_tointeger(L, 6),
			(GLsizei)lua_tointeger(L, 7), (GLint)lua_tointeger(L, 8));

		return 0;
	}


	/* void glCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y,
	 *							 GLsizei width)
     *
	 * CopyTexSubImage1D (target, level, xoffset, x, y, width)
	 */
	int gl_copy_tex_sub_image_1d (lua_State* L) {
		
		if (!(	lua_isnumber(L, 1) &&				// target
				lua_isnumber(L, 2) &&				// level
				lua_isnumber(L, 3) &&				// xoffset
				lua_isnumber(L, 4) &&				// x
				lua_isnumber(L, 5) &&				// y
				lua_isnumber(L, 6)					// width
			)) {
			luaL_error(L, "Incorrect argument to function gl.CopyTexSubImage1D");
		}

		glCopyTexSubImage1D((GLenum)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLint)lua_tointeger(L, 3),
			(GLint)lua_tointeger(L, 4), (GLint)lua_tointeger(L, 5), (GLsizei)lua_tointeger(L, 6));

		return 0;
	}


	/* void glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset,
	 *							 GLint x, GLint y, GLsizei width, GLsizei height)
     *
	 * CopyTexSubImage2D (target, level, xoffset, yoffset, x, y, width, height)
	 */
	int gl_copy_tex_sub_image_2d (lua_State* L) {
		
		if (!(	lua_isnumber(L, 1) &&				// target
				lua_isnumber(L, 2) &&				// level
				lua_isnumber(L, 3) &&				// xoffset
				lua_isnumber(L, 4) &&				// yoffset
				lua_isnumber(L, 5) &&				// x
				lua_isnumber(L, 6) &&				// y
				lua_isnumber(L, 7) &&				// width
				lua_isnumber(L, 8)					// height
			)) {
			luaL_error(L, "Incorrect argument to function gl.CopyTexSubImage2D");
		}

		glCopyTexSubImage2D((GLenum)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLint)lua_tointeger(L, 3),
			(GLint)lua_tointeger(L, 4), (GLint)lua_tointeger(L, 5), (GLint)lua_tointeger(L, 6),
			(GLsizei)lua_tointeger(L, 7), (GLsizei)lua_tointeger(L, 8));

		return 0;
	}


	/* void glTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width,
	 *						 GLenum format, GLenum type, const GLvoid* data)
	 *
	 * TexSubImage1D (target, level, xoffset, width, format, type, data)
	 */
	int gl_tex_sub_image_1d (lua_State* L) {

		if (!(	lua_isnumber(L, 1) &&				// target
				lua_isnumber(L, 2) &&				// level
				lua_isnumber(L, 3) &&				// xoffset
				lua_isnumber(L, 4) &&				// width
				lua_isnumber(L, 5) &&				// format
				lua_isnumber(L, 6) &&				// type
				(lua_istable(L, 7) || lua_isuserdata(L, 9) || lua_isnumber(L, 9))	// data
			)) {
			luaL_error(L, "Incorrect argument to function gl.TexSubImage1D");
		}

		GLint width = lua_tointeger(L, 4);
		GLenum format = lua_tointeger(L, 5);
		GLenum type = lua_tointeger(L, 6);
		const void* data = 0;

		if (lua_isuserdata(L, 7)) {
			data = GLCommon::get1DPixelsFromSA(L, 7, width, format, type, "gl.TexSubImage1D");
		}
		else if (lua_istable(L, 7)) {
			data = GLCommon::get1DPixelsFromTable(L, 7, width, format, type, "gl.TexSubImage1D");
		}
		else {
			GLint pixbuf = 0;
			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &pixbuf);
			if (pixbuf)
				data = (const void*)lua_tointeger(L, 7);
			else
				luaL_error(L, "Buffer offset used with unbound buffer in gl.TexSubImage1D");
		}

		glTexSubImage1D((GLint)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLint)lua_tointeger(L, 3),
			width, format, type, data);
		
		if (lua_istable(L, 7))
			delete[] data;

		return 0;
	}


	/* void glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
	 *						 GLint height, GLenum format, GLenum type, const GLvoid* data)
	 *
	 * TexSubImage2D (target, level, xoffset, yoffset, width, height, format, type, data)
	 */
	int gl_tex_sub_image_2d (lua_State* L) {

		if (!(	lua_isnumber(L, 1) &&				// target
				lua_isnumber(L, 2) &&				// level
				lua_isnumber(L, 3) &&				// xoffset
				lua_isnumber(L, 4) &&				// yoffset
				lua_isnumber(L, 5) &&				// width
				lua_isnumber(L, 6) &&				// height
				lua_isnumber(L, 7) &&				// format
				lua_isnumber(L, 8) &&				// type
				(lua_istable(L, 9) || lua_isuserdata(L, 9) || lua_isnumber(L, 9))	// data
			)) {
			luaL_error(L, "Incorrect argument to function gl.TexSubImage2D");
		}

		GLint width = lua_tointeger(L, 5);
		GLint height = lua_tointeger(L, 6);
		GLenum format = lua_tointeger(L, 7);
		GLenum type = lua_tointeger(L, 8);
		const void* data = 0;

		if (lua_isuserdata(L, 9)) {
			data = GLCommon::get2DPixelsFromSA(L, 9, width, height, format, type, "gl.TexSubImage2D");
		}
		else if (lua_istable(L, 9)) {
			data = GLCommon::get2DPixelsFromTable(L, 9, width, height, format, type, "gl.TexSubImage2D");
		}
		else {
			GLint pixbuf = 0;
			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &pixbuf);
			if (pixbuf)
				data = (const void*)lua_tointeger(L, 9);
			else
				luaL_error(L, "Buffer offset used with unbound buffer in gl.TexSubImage2D");
		}

		glTexSubImage2D((GLint)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLint)lua_tointeger(L, 3),
			(GLint)lua_tointeger(L, 4), width, height, format, type, data);
		
		if (lua_istable(L, 9))
			delete[] data;

		return 0;
	}

	/* void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
	 *
	 * VertexPointer (size, type, stride, data)
	 */
	int gl_vertex_pointer (lua_State* L) {

		if (!(	lua_isnumber(L, 1) &&		// size
				lua_isnumber(L, 2) &&		// type
				lua_isnumber(L, 3)			// stride
			)) {
			luaL_error(L, "Incorrect argument to function 'gl.VertexPointer'");
		}

		GLenum type = lua_tointeger(L, 2);
		const void* data = 0;
		static const void** dataTable = 0;

		if (!dataTable) {
			GLint msz = 0;
			glGetIntegerv(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, &msz);
			dataTable = new const void*[msz];

			for (int i = 0; i < msz; i++)
				dataTable[i] = 0;
		}

		GLint index = 0;
		glGetIntegerv(GL_CLIENT_ATTRIB_STACK_DEPTH, &index);

		if (dataTable[index]) {
			delete[] dataTable[index];
			dataTable[index] = 0;
		}

		if (lua_isuserdata(L, 4)) {
			ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, 4, "lib.array");
			if (!aud->arr)
				luaL_error(L, "Invalid static array passed to function 'gl.VertexPointer'");

			data = aud->arr->raw();
		}
		else if (lua_istable(L, 4)) {
			int len = 0;
			dataTable[index] = GLCommon::getDataFromTable(L, 3, type, &len, "gl.VertexPointer");
			data = dataTable[index];
		}
		else if (lua_isnumber(L, 4)) {
			GLint arrbuf = 0;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrbuf);
			if (arrbuf)
				data = (const void*)lua_tointeger(L, 4);
			else
				luaL_error(L, "Buffer offset used with unbound buffer in gl.VertexPointer");
		}
		else
			luaL_error(L, "Incorrect argument to function 'gl.VertexPointer'");

		glVertexPointer(lua_tointeger(L, 1), type, lua_tointeger(L, 3), data);

		return 0;
	}
}
