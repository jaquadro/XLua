#include <string.h>
#include <malloc.h>

#include "LuaUtil.h"

extern "C" {

	int gl_test (lua_State* L) {
		return 0;
	}

	/* void glDrawPixels (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data)
	 *
	 * DrawPixels (width, height, format, type, data) -> None
	 */
	int gl_draw_pixels (lua_State* L) {
		
		if (!(	lua_isnumber(L, 1) &&				// width
				lua_isnumber(L, 2) &&				// height
				lua_isnumber(L, 3) &&				// format
				lua_isnumber(L, 4) &&				// type
				(lua_istable(L, 5) || lua_isuserdata(L, 5) || lua_isnumber(L, 5))	// data
			)) {
			luaL_error(L, "Incorrect argument to function gl.DrawPixels");
		}

		GLsizei width = lua_tointeger(L, 1);
		GLsizei height = lua_tointeger(L, 2);
		GLint format = lua_tointeger(L, 3);
		GLint type = lua_tointeger(L, 4);
		const void* data = 0;

		if (lua_isuserdata(L, 5)) {
			data = GLCommon::get2DPixelsFromSA(L, 5, width, height, format, type, "gl.DrawPixels");
		}
		else if (lua_istable(L, 5)) {
			data = GLCommon::get2DPixelsFromTable(L, 5, width, height, format, type, "gl.DrawPixels");
		}
		else {
			GLint pixbuf = 0;
			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &pixbuf);
			if (pixbuf)
				data = (const void*)lua_tointeger(L, 5);
			else
				luaL_error(L, "Buffer offset used with unbound buffer in gl.DrawPixels");
		}

		glDrawPixels(width, height, format, type, data);

		if (lua_istable(L, 8))
			delete[] data;

		return 0;
	}


	/* void glTexImage1D (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border,
	 *                    GLenum format, GLenum type, const GLvoid* data)
	 *
	 * TexImage1D (target, level, internalFormat, width, border, format, type, data) -> None
	 */
	int gl_tex_image_1d (lua_State* L) {

		if (!(	lua_isnumber(L, 1) &&				// target
				lua_isnumber(L, 2) &&				// level
				lua_isnumber(L, 3) &&				// internalFormat
				lua_isnumber(L, 4) &&				// width
				lua_isnumber(L, 5) &&				// border
				lua_isnumber(L, 6) &&				// format
				lua_isnumber(L, 7) &&				// type
				(lua_istable(L, 8) || lua_isuserdata(L, 8) || lua_isnumber(L, 8))	// data
			)) {
			luaL_error(L, "Incorrect argument to function gl.TexImage1D");
		}

		GLint width = lua_tointeger(L, 4);
		GLenum format = lua_tointeger(L, 6);
		GLenum type = lua_tointeger(L, 7);
		const void* data = 0;

		if (lua_isuserdata(L, 8)) {
			data = GLCommon::get1DPixelsFromSA(L, 8, width, format, type, "gl.TexImage1D");
		}
		else if (lua_istable(L, 8)) {
			data = GLCommon::get1DPixelsFromTable(L, 8, width, format, type, "gl.TexImage1D");
		}
		else {
			GLint pixbuf = 0;
			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &pixbuf);
			if (pixbuf)
				data = (const void*)lua_tointeger(L, 8);
			else if (lua_tointeger(L, 8) != 0)
				luaL_error(L, "Buffer offset used with unbound buffer in gl.TexImage1D");
		}

		glTexImage1D((GLint)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLint)lua_tointeger(L, 3),
			width, (GLint)lua_tointeger(L, 5), format, type, data);
		
		if (lua_istable(L, 8))
			delete[] data;

		return 0;
	}


	/* void glTexImage2D (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height,
	 *                    GLint border, GLenum format, GLenum type, const GLvoid* data)
	 *
	 * TexImage2D (target, level, internalFormat, width, height, border, format, type, data)
	 */
	int gl_tex_image_2d (lua_State* L) {

		if (!(	lua_isnumber(L, 1) &&				// target
				lua_isnumber(L, 2) &&				// level
				lua_isnumber(L, 3) &&				// internalFormat
				lua_isnumber(L, 4) &&				// width
				lua_isnumber(L, 5) &&				// height
				lua_isnumber(L, 6) &&				// border
				lua_isnumber(L, 7) &&				// format
				lua_isnumber(L, 8) &&				// type
				(lua_istable(L, 9) || lua_isuserdata(L, 9) || lua_isnumber(L, 9))	// data
			)) {
			luaL_error(L, "Incorrect argument to function gl.TexImage2D");
		}

		GLint width = lua_tointeger(L, 4);
		GLint height = lua_tointeger(L, 5);
		GLenum format = lua_tointeger(L, 7);
		GLenum type = lua_tointeger(L, 8);
		const void* data = 0;

		if (lua_isuserdata(L, 9)) {
			data = GLCommon::get2DPixelsFromSA(L, 9, width, height, format, type, "gl.TexImage2D");
		}
		else if (lua_istable(L, 9)) {
			data = GLCommon::get2DPixelsFromTable(L, 9, width, height, format, type, "gl.TexImage2D");
		}
		else {
			GLint pixbuf = 0;
			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &pixbuf);
			if (pixbuf)
				data = (const void*)lua_tointeger(L, 9);
			else if (lua_tointeger(L, 9) != 0)
				luaL_error(L, "Buffer offset used with unbound buffer in gl.TexImage2D");
		}

		glTexImage2D((GLint)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLint)lua_tointeger(L, 3),
			width, height, (GLint)lua_tointeger(L, 6), format, type, data);
		
		if (lua_istable(L, 9))
			delete[] data;

		return 0;
	}

}
