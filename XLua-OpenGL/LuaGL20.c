#include <string.h>
#include <malloc.h>

#include "lauxlib.h"
#include "LuaGL.h"

/* void glAttachShader (GLuint program, GLuint shader)
 *
 * AttachShader (program, shader) -> none
 */
int gl_attach_shader (lua_State *L)
{
	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.AttachShader'");

	/* call opengl function */
	glAttachShader(lua_tointeger(L, 1), lua_tointeger(L, 2));

	return 0;
}

/* void glBindAttribLocation (GLuint program, GLuint index, const GLchar *name)
 *
 * BindAttribLocation (program, index, name) -> none
 */
int gl_bind_attrib_location (lua_State *L)
{
	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isstring(L, 3)))
		luaL_error(L, "incorrect argument to function 'gl.BindAttribLocation'");

	/* call opengl function */
	glBindAttribLocation(lua_tointeger(L, 1), lua_tointeger(L, 2), lua_tostring(L, 3));

	return 0;
}

/* void glBlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha)
 * 
 * BlendEquationSeparate (mode) -> none
 */
int gl_blend_equation_separate(lua_State *L)
{
	//GLenum a, b;

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.BindAttribLocation'");

	//a = (GLenum)get_gl_enum(L, 1);
	//b = (GLenum)get_gl_enum(L, 2);

	//if (a == ENUM_ERROR || b == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.BlendEquationSeparate'");

	glBlendEquationSeparate((GLenum)lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2));

	return 0;
}

/* void glCompileShader (GLuint shader)
 *
 * CompileShader (shader) -> none
 */
int gl_compile_shader (lua_State *L)
{
	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.CompileShader'");

	/* call opengl function */
	glCompileShader(lua_tointeger(L, 1));

	return 0;
}

/* GLuint glCreateProgram (void)
 *
 * CreateProgram () -> number
 */
int gl_create_program (lua_State *L)
{
	/* call opengl function */
	lua_pushinteger(L, glCreateProgram());

	return 1;
}

/* GLuint glCreateShader (GLenum shaderType)
 *
 * CreateShader (type) -> number
 */
int gl_create_shader (lua_State *L)
{
	//GLenum e;

	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.CreateShader'");

	/* get string parameter */
	//e = get_gl_enum(L, 1);

	/* test argument */
	//if(e == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.CreateShader'");

	/* call opengl function */
	lua_pushinteger(L, glCreateShader((GLenum)lua_tointeger(L, 1)));

	return 1;
}

/* void glDeleteProgram (GLuint program)
 *
 * DeleteProgram (program) -> none
 */
int gl_delete_program (lua_State *L)
{
	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.DeleteProgram'");

	/* call opengl function */
	glDeleteProgram(lua_tointeger(L, 1));

	return 0;
}

/* void glDeleteShader (GLuint shader)
 *
 * DeleteShader (shader) -> none
 */
int gl_delete_shader (lua_State *L)
{
	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.DeleteShader'");

	/* call opengl function */
	glDeleteShader(lua_tointeger(L, 1));

	return 0;
}

/* void glDetachShader (GLuint program, GLuint shader)
 *
 * DetachShader (program, shader) -> none
 */
int gl_detach_shader (lua_State *L)
{
	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.DetachShader'");

	/* call opengl function */
	glDetachShader(lua_tointeger(L, 1), lua_tointeger(L, 2));

	return 0;
}

/* void glDisableVertexAttribArray (GLuint index)
 *
 * DisableVertexAttribArray (index) -> none
 */
int gl_disable_vertex_attrib_array (lua_State *L)
{
	/* test arguments type */
	if(!(lua_isnumber(L, 1)))
		luaL_error(L, "incorrect argument to function 'gl.DisableVertexAttribArray'");

	/* call opengl function */
	glDisableVertexAttribArray(lua_tointeger(L, 1));

	return 0;
}

/* void glEnableVertexAttribArray (GLuint index)
 *
 * EnableVertexAttribArray (index) -> none
 */
int gl_enable_vertex_attrib_array (lua_State *L)
{
	/* test arguments type */
	if(!(lua_isnumber(L, 1)))
		luaL_error(L, "incorrect argument to function 'gl.EnableVertexAttribArray'");

	/* call opengl function */
	glEnableVertexAttribArray(lua_tointeger(L, 1));

	return 0;
}

/* GLint glGetAttribLocation (GLuint program, const GLchar* name)
 *
 * GetAttribLocation (program, name) -> number
 */
int gl_get_attrib_location (lua_State *L)
{
	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isstring(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.GetAttribLocation'");

	/* call opengl function */
	lua_pushinteger(L, glGetAttribLocation(lua_tointeger(L, 1), lua_tostring(L, 2)));

	return 1;
}

/* void glGetProgram (GLuint program, GLenum pname, GLint* params)
 *
 * GetProgram (program, pname) -> number
 */
int gl_get_program (lua_State *L)
{
	//GLenum pname;
	GLint param;

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.GetProgram'");

	//pname = get_gl_enum(L, 2);

	//if(pname == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetProgram'");

	/* call opengl function */
	glGetProgramiv(lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2), &param);

	lua_pushinteger(L, param);
	return 1;
}

/* void glGetProgramInfoLog (GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog)
 *
 * GetProgramInfoLog (program) -> string
 */
int gl_get_program_info_log (lua_State *L)
{
	GLint program, param;
	GLsizei length;
	GLchar* log;

	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GetProgramInfoLog'");

	program = lua_tointeger(L, 1);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &param);

	log = (GLchar*) malloc((param + 1) * sizeof(GLchar));

	/* call opengl function */
	glGetProgramInfoLog(program, param + 1, &length, log);

	lua_pushstring(L, log);
	free(log);

	return 1;
}

/* void glGetShader (GLuint shader, GLenum pname, GLint* params)
 *
 * GetShader (shader, pname) -> number
 */
int gl_get_shader (lua_State *L)
{
	//GLenum pname;
	GLint param;

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.GetShader'");

	//pname = get_gl_enum(L, 2);

	//if(pname == ENUM_ERROR)
	//	luaL_error(L, "incorrect string argument to function 'gl.GetShader'");

	/* call opengl function */
	glGetShaderiv(lua_tointeger(L, 1), (GLenum)lua_tointeger(L, 2), &param);

	lua_pushinteger(L, param);
	return 1;
}

/* void glGetShaderInfoLog (GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog)
 *
 * GetShaderInfoLog (shader) -> string
 */
int gl_get_shader_info_log (lua_State *L)
{
	GLint shader, param;
	GLsizei length;
	GLchar* log;

	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GetShaderInfoLog'");

	shader = lua_tointeger(L, 1);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &param);

	log = (GLchar*) malloc((param + 1) * sizeof(GLchar));

	/* call opengl function */
	glGetShaderInfoLog(shader, param + 1, &length, log);

	lua_pushstring(L, log);
	free(log);

	return 1;
}

/* void glGetShaderSource (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* source)
 *
 * GetShaderSource (shader) -> string
 */
int gl_get_shader_source (lua_State *L)
{
	GLint shader, param;
	GLsizei length;
	GLchar* source;

	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.GetShaderSource'");

	shader = lua_tointeger(L, 1);
	glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &param);

	source = (GLchar*) malloc((param + 1) * sizeof(GLchar));

	/* call opengl function */
	glGetShaderSource(shader, param + 1, &length, source);

	lua_pushstring(L, source);
	free(source);

	return 1;
}

/* GLint glGetUniformLocation (GLuint program, const GLchar* name)
 *
 * GetUniformLocation (program, name) -> number
 */
int gl_get_uniform_location (lua_State *L)
{
	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isstring(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.GetUniformLocation'");

	/* call opengl function */
	lua_pushinteger(L, glGetUniformLocation(lua_tointeger(L, 1), lua_tostring(L, 2)));

	return 1;
}

/* GLboolean glIsProgram (GLuint program)
 *
 * IsProgram (program) -> number
 */
int gl_is_program (lua_State *L)
{
	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.IsProgram'");

	/* call opengl function */
	lua_pushboolean(L, glIsProgram(lua_tointeger(L, 1)));

	return 1;
}

/* GLboolean glIsShader (GLuint shader)
 *
 * IsShader (shader) -> number
 */
int gl_is_shader (lua_State *L)
{
	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.IsShader'");

	/* call opengl function */
	lua_pushboolean(L, glIsShader(lua_tointeger(L, 1)));

	return 1;
}

/* void glLinkProgram (GLuint program)
 *
 * LinkProgram (program) -> none
 */
int gl_link_program (lua_State *L)
{
	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.LinkProgram'");

	/* call opengl function */
	glLinkProgram(lua_tointeger(L, 1));

	return 0;
}

/* void glShaderSource (GLuint shader, GLsizei count, const GLchar** string, const GLint* length)
 *
 * ShaderSource (shader, source) -> none
 */
int gl_shader_source (lua_State *L)
{
	GLuint shader;
	const GLchar* str;

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isstring(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.ShaderSource'");

	/* get string parameter */
	shader = lua_tointeger(L, 1);
	str = lua_tostring(L, 2);

	/* call opengl function */
	glShaderSource(shader, 1, &str, NULL);

	return 0;
}

/* void glUniform4f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
 * void glUniform4fv (GLint location, GLsizei count, const GLfloat* value)
 *
 * Uniformf (location, v0 [, v1 [, v2 [, v3]]]) -> none
 */
int gl_uniform_f (lua_State *L)
{
	int i, count = 1, size = 1;
	GLfloat* fv;

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && (lua_isnumber(L, 2) || lua_istable(L, 2))))
		luaL_error(L, "incorrect argument to function 'gl.Uniformf'");

	if (lua_istable(L, 2)) {
		count = get_array2f(L, 2, &fv, &size);
		if (count == -1) {
			luaL_error(L, "incorrect argument to function 'gl.Uniformf'");
		}
	} else {
		if (lua_isnumber(L, 3))
			size = 2;
		if (lua_isnumber(L, 4))
			size = 3;
		if (lua_isnumber(L, 5))
			size = 4;

		fv = (GLfloat*) malloc(size * sizeof(GLfloat));
		for (i = 0; i < size; i++) {
			fv[i] = (GLfloat) lua_tonumber(L, i + 2);
		}
	}

	/* call opengl function */
	switch (size) {
		case 1: glUniform1fv(lua_tointeger(L, 1), count, fv); break;
		case 2: glUniform2fv(lua_tointeger(L, 1), count, fv); break;
		case 3: glUniform3fv(lua_tointeger(L, 1), count, fv); break;
		case 4: glUniform4fv(lua_tointeger(L, 1), count, fv); break;
	}

	return 0;
}

/* void glUniform4i (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
 * void glUniform4iv (GLint location, GLsizei count, const GLfloat* value)
 *
 * Uniformi (location, v0 [, v1 [, v2 [, v3]]]) -> none
 */
int gl_uniform_i (lua_State *L)
{
	int i, count = 1, size = 1;
	GLint* iv;

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && (lua_isnumber(L, 2) || lua_istable(L, 2))))
		luaL_error(L, "incorrect argument to function 'gl.Uniformi'");

	if (lua_istable(L, 2)) {
		count = get_array2i(L, 2, &iv, &size);
		if (count == -1) {
			luaL_error(L, "incorrect argument to function 'gl.Uniformi'");
		}
	} else {
		if (lua_isnumber(L, 3))
			size = 2;
		if (lua_isnumber(L, 4))
			size = 3;
		if (lua_isnumber(L, 5))
			size = 4;

		iv = (GLint*) malloc(size * sizeof(GLint));
		for (i = 0; i < size; i++) {
			iv[i] = (GLint) lua_tointeger(L, i + 2);
		}
	}

	/* call opengl function */
	switch (size) {
		case 1: glUniform1iv(lua_tointeger(L, 1), count, iv); break;
		case 2: glUniform2iv(lua_tointeger(L, 1), count, iv); break;
		case 3: glUniform3iv(lua_tointeger(L, 1), count, iv); break;
		case 4: glUniform4iv(lua_tointeger(L, 1), count, iv); break;
	}

	return 0;
}

/* void glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
 *
 * UniformMatrix (location, matTable) -> none
 */
int gl_uniform_matrix (lua_State *L)
{
	int count = 1, innerSize = 1, outerSize = 1;
	GLfloat* fv;

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_istable(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.UniformMatrix'");

	count = get_array3f(L, 2, &fv, &outerSize, &innerSize);
	if (count == -1) {
		count = get_array2f(L, 2, &fv, &innerSize);
		if (count == -1)
			luaL_error(L, "incorrect argument to function 'gl.UniformMatrix'");
	}

	/* call opengl function */
	switch (innerSize) {
		case 2: glUniformMatrix2fv(lua_tointeger(L, 1), count, GL_TRUE, fv); break;
		case 3: glUniformMatrix3fv(lua_tointeger(L, 1), count, GL_TRUE, fv); break;
		case 4: glUniformMatrix4fv(lua_tointeger(L, 1), count, GL_TRUE, fv); break;
	}

	return 0;
}

/* void glUseProgram (GLuint program)
 *
 * UseProgram (program) -> none
 */
int gl_use_program (lua_State *L)
{
	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.UseProgram'");

	/* call opengl function */
	glUseProgram(lua_tointeger(L, 1));

	return 0;
}

/* void glValidateProgram (GLuint program)
 *
 * ValidateProgram (program) -> none
 */
int gl_validate_program (lua_State *L)
{
	/* test arguments type */
	if(!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.ValidateProgram'");

	/* call opengl function */
	glValidateProgram(lua_tointeger(L, 1));

	return 0;
}

/* void glVertexAttrib4f (GLint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
 * void glVertexAttrib4fv (GLint index, const GLfloat* value)
 *
 * VertexAttribf (index, v0 [, v1 [, v2 [, v3]]]) -> none
 */
int gl_vertex_attrib_f (lua_State *L)
{
	int i, size = 1;
	GLfloat* fv;

	/* test arguments type */
	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2)))
		luaL_error(L, "incorrect argument to function 'gl.VertexAttribf'");

	if (lua_isnumber(L, 3))
		size = 2;
	if (lua_isnumber(L, 4))
		size = 3;
	if (lua_isnumber(L, 5))
		size = 4;

	fv = (GLfloat*) malloc(size * sizeof(GLfloat));
	for (i = 0; i < size; i++) {
		fv[i] = (GLfloat) lua_tonumber(L, i + 2);
	}

	/* call opengl function */
	switch (size) {
		case 1: glVertexAttrib1fv(lua_tointeger(L, 1), fv); break;
		case 2: glVertexAttrib2fv(lua_tointeger(L, 1), fv); break;
		case 3: glVertexAttrib3fv(lua_tointeger(L, 1), fv); break;
		case 4: glVertexAttrib4fv(lua_tointeger(L, 1), fv); break;
	}

	return 0;
}

/* void glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
 *
 * VertexAttribPointer (index, attribArray [,size [,stride]]) -> none
 * VertexAttribPointer (index, bufferOffset, size [,stride]) -> none
 */
int gl_vertex_attrib_pointer(lua_State *L)
{
	GLint size, offset;
	GLsizei stride = 0;
	static GLfloat *array = 0;

	if(array)
		free(array);

	if (!lua_isnumber(L, 1))
		luaL_error(L, "incorrect argument to function 'gl.VertexAttribPointer'");

	if (lua_isnumber(L, 2)) {
		offset = lua_tointeger(L, 2);
		if (!lua_isnumber(L, 3)) {
			luaL_error(L, "incorrect argument to function 'gl.VertexAttribPointer'");
		}
		size = lua_tointeger(L, 3);
		
		if (lua_isnumber(L, 4)) {
			stride = lua_tointeger(L, 4);
		}

		glVertexAttribPointer(lua_tointeger(L, 1), size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLubyte*) NULL + offset * sizeof(GLfloat));
	}
	else if (lua_istable(L, 2)) {
		if (get_array2f(L, 2, &array, &size) == -1)
			size = get_arrayf(L, 2, &array) / 3;

		if (lua_isnumber(L, 3)) {
			size = lua_tointeger(L, 3);
		}

		if (lua_isnumber(L, 4)) {
			stride = lua_tointeger(L, 4);
		}

		glVertexAttribPointer(lua_tointeger(L, 1), size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), array);
	}
	else {
		luaL_error(L, "incorrect argument to function 'gl.VertexAttribPointer'");
	}

	return 0;
}
