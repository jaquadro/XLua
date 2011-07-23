#include <string.h>
#include <malloc.h>

#include "LuaUtil.h"

extern "C" {

	/* void glBlendColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	 *
	 * BlendColor (red, green, blue, alpha) -> none
	 */
	int gl_blend_color(lua_State *L)
	{
		if (!(	lua_isnumber(L, 1) &&		// red
				lua_isnumber(L, 2) &&		// green
				lua_isnumber(L, 3) &&		// blue
				lua_isnumber(L, 4)			// alpha
			)) {
			luaL_error(L, "incorrect argument to function 'gl.BlendColor'");
		}

		glBlendColor((GLclampf)lua_tonumber(L, 1), (GLclampf)lua_tonumber(L, 2),
			(GLclampf)lua_tonumber(L, 3), (GLclampf)lua_tonumber(L, 4));

		return 0;
	}


	/* void glBlendEquation (GLenum mode)
	 *
	 * BlendEquation (mode) -> none
	 */
	int gl_blend_equation(lua_State *L) {
	
		if (!lua_isnumber(L, 1)) {
			luaL_error(L, "incorrect argument to function 'gl.BlendEquation'");
		}

		glBlendEquation((GLenum)lua_tointeger(L, 1));

		return 0;
	}


	/* void glCopyTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset,
	 *							 GLint x, GLint y, GLsizei width, GLsizei height)
     *
	 * CopyTexSubImage3D (target, level, xoffset, yoffset, zoffset, x, y, width, height)
	 */
	int gl_copy_tex_sub_image_3d (lua_State* L) {
		
		if (!(	lua_isnumber(L, 1) &&				// target
				lua_isnumber(L, 2) &&				// level
				lua_isnumber(L, 3) &&				// xoffset
				lua_isnumber(L, 4) &&				// yoffset
				lua_isnumber(L, 5) &&				// zoffset
				lua_isnumber(L, 6) &&				// x
				lua_isnumber(L, 7) &&				// y
				lua_isnumber(L, 8) &&				// width
				lua_isnumber(L, 9)					// height
			)) {
			luaL_error(L, "Incorrect argument to function gl.CopyTexSubImage3D");
		}

		glCopyTexSubImage3D((GLenum)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLint)lua_tointeger(L, 3),
			(GLint)lua_tointeger(L, 4), (GLint)lua_tointeger(L, 5), (GLint)lua_tointeger(L, 6),
			(GLint)lua_tointeger(L, 7), (GLsizei)lua_tointeger(L, 8), (GLsizei)lua_tointeger(L, 9));

		return 0;
	}


	/* void glTexImage3D (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height,
	 *                    GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* data)
	 *
	 * TexImage3D (target, level, internalFormat, width, height, depth, border, format, type, data)
	 */
	int gl_tex_image_3d (lua_State* L) {

		if (!(	lua_isnumber(L, 1) &&				// target
				lua_isnumber(L, 2) &&				// level
				lua_isnumber(L, 3) &&				// internalFormat
				lua_isnumber(L, 4) &&				// width
				lua_isnumber(L, 5) &&				// height
				lua_isnumber(L, 6) &&				// depth
				lua_isnumber(L, 7) &&				// border
				lua_isnumber(L, 8) &&				// format
				lua_isnumber(L, 9) &&				// type
				(lua_istable(L, 10) || lua_isuserdata(L, 10) || lua_isnumber(L, 10))	// data
			)) {
			luaL_error(L, "Incorrect argument to function gl.TexImage3D");
		}

		GLint width = lua_tointeger(L, 4);
		GLint height = lua_tointeger(L, 5);
		GLint depth = lua_tointeger(L, 6);
		GLenum format = lua_tointeger(L, 8);
		GLenum type = lua_tointeger(L, 9);
		const void* data = 0;

		if (lua_isuserdata(L, 10)) {
			data = GLCommon::get3DPixelsFromSA(L, 10, width, height, depth, format, type, "gl.TexImage3D");
		}
		else if (lua_istable(L, 10)) {
			data = GLCommon::get3DPixelsFromTable(L, 10, width, height, depth, format, type, "gl.TexImage3D");
		}
		else {
			GLint pixbuf = 0;
			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &pixbuf);
			if (pixbuf)
				data = (const void*)lua_tointeger(L, 10);
			else if (lua_tointeger(L, 10) != 0)
				luaL_error(L, "Buffer offset used with unbound buffer in gl.TexImage3D");
		}

		glTexImage3D((GLint)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLint)lua_tointeger(L, 3),
			width, height, depth, (GLint)lua_tointeger(L, 6), format, type, data);
		
		if (lua_istable(L, 10))
			delete[] data;

		return 0;
	}


	/* void glTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width,
	 *						 GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* data)
	 *
	 * TexSubImage3D (target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data)
	 */
	int gl_tex_sub_image_3d (lua_State* L) {

		if (!(	lua_isnumber(L, 1) &&				// target
				lua_isnumber(L, 2) &&				// level
				lua_isnumber(L, 3) &&				// xoffset
				lua_isnumber(L, 4) &&				// yoffset
				lua_isnumber(L, 5) &&				// zoffset
				lua_isnumber(L, 6) &&				// width
				lua_isnumber(L, 7) &&				// height
				lua_isnumber(L, 8) &&				// depth
				lua_isnumber(L, 9) &&				// format
				lua_isnumber(L, 10) &&				// type
				(lua_istable(L, 11) || lua_isuserdata(L, 11) || lua_isnumber(L, 11))	// data
			)) {
			luaL_error(L, "Incorrect argument to function gl.TexSubImage3D");
		}

		GLint width = lua_tointeger(L, 6);
		GLint height = lua_tointeger(L, 7);
		GLint depth = lua_tointeger(L, 8);
		GLenum format = lua_tointeger(L, 9);
		GLenum type = lua_tointeger(L, 10);
		const void* data = 0;

		if (lua_isuserdata(L, 11)) {
			data = GLCommon::get3DPixelsFromSA(L, 11, width, height, depth, format, type, "gl.TexSubImage3D");
		}
		else if (lua_istable(L, 11)) {
			data = GLCommon::get3DPixelsFromTable(L, 11, width, height, depth, format, type, "gl.TexSubImage3D");
		}
		else {
			GLint pixbuf = 0;
			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &pixbuf);
			if (pixbuf)
				data = (const void*)lua_tointeger(L, 11);
			else
				luaL_error(L, "Buffer offset used with unbound buffer in gl.TexSubImage3D");
		}

		glTexSubImage3D((GLint)lua_tointeger(L, 1), (GLint)lua_tointeger(L, 2), (GLint)lua_tointeger(L, 3),
			(GLint)lua_tointeger(L, 4), (GLint)lua_tointeger(L, 5), width, height, depth, format, type, data);
		
		if (lua_istable(L, 11))
			delete[] data;

		return 0;
	}
}
