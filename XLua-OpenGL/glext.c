#include <windows.h>
#include <GL/gl.h>

#include "glext.h"

#define STATICFN(fntype, fnname)			static PFN ## fntype ## PROC p ## fnname = NULL; \
											if (p ## fnname == NULL) \
												p ## fnname = (PFN ## fntype ## PROC) wglGetProcAddress(#fnname); \

#define STATICFN_R(fntype, fnname)			STATICFN(fntype, fnname) \
											if (p ## fnname == NULL) return 0;

#define STATICFN_F(fntype, fnname)			STATICFN(fntype, fnname) \
											if (p ## fnname == NULL) return;

/* *** GL 1.2 ********************* */

/* BlendColor */

GLAPI void APIENTRY glBlendColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
	STATICFN(GLBLENDCOLOR, glBlendColor)
	if (pglBlendColor == NULL)
		glBlendColorEXT(red, green, blue, alpha);
	pglBlendColor(red, green, blue, alpha);
}

GLAPI void APIENTRY glBlendColorEXT (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
	STATICFN_F(GLBLENDCOLOREXT, glBlendColorEXT)
	pglBlendColorEXT(red, green, blue, alpha);
}

/* BlendEquation */

GLAPI void APIENTRY glBlendEquation (GLenum mode) {
	STATICFN(GLBLENDEQUATION, glBlendEquation)
	if (pglBlendEquation == NULL)
		glBlendEquationEXT(mode);
	pglBlendEquation(mode);
}

GLAPI void APIENTRY glBlendEquationEXT (GLenum mode) {
	STATICFN_F(GLBLENDEQUATIONEXT, glBlendEquationEXT)
	pglBlendEquationEXT(mode);
}

/* *** GL 1.3 ********************* */

/* ActiveTexture */

GLAPI void APIENTRY glActiveTexture (GLenum texture) {
	STATICFN(GLACTIVETEXTURE, glActiveTexture)
	if (pglActiveTexture == NULL)
		glActiveTextureARB(texture);
	pglActiveTexture(texture);
}

GLAPI void APIENTRY glActiveTextureARB (GLenum texture) {
	STATICFN_F(GLACTIVETEXTUREARB, glActiveTextureARB)
	pglActiveTextureARB(texture);
}

/* ClientActiveTexture */

GLAPI void APIENTRY glClientActiveTexture (GLenum texture) {
	STATICFN(GLCLIENTACTIVETEXTURE, glClientActiveTexture)
	if (pglClientActiveTexture == NULL)
		glClientActiveTextureARB(texture);
	pglClientActiveTexture(texture);
}

GLAPI void APIENTRY glClientActiveTextureARB (GLenum texture) {
	STATICFN_F(GLCLIENTACTIVETEXTUREARB, glClientActiveTextureARB)
	pglClientActiveTextureARB(texture);
}

/* MultiTexCoord */

GLAPI void APIENTRY glMultiTexCoord1f (GLenum target, GLfloat s) {
	STATICFN(GLMULTITEXCOORD1F, glMultiTexCoord1f)
	if (pglMultiTexCoord1f == NULL)
		glMultiTexCoord1fARB(target, s);
	pglMultiTexCoord1f(target, s);
}

GLAPI void APIENTRY glMultiTexCoord1fARB (GLenum target, GLfloat s) {
	STATICFN_F(GLMULTITEXCOORD1FARB, glMultiTexCoord1fARB)
	pglMultiTexCoord1fARB(target, s);
}

GLAPI void APIENTRY glMultiTexCoord2f (GLenum target, GLfloat s, GLfloat t) {
	STATICFN(GLMULTITEXCOORD2F, glMultiTexCoord2f)
	if (pglMultiTexCoord2f == NULL)
		glMultiTexCoord2fARB(target, s, t);
	pglMultiTexCoord2f(target, s, t);
}

GLAPI void APIENTRY glMultiTexCoord2fARB (GLenum target, GLfloat s, GLfloat t) {
	STATICFN_F(GLMULTITEXCOORD2FARB, glMultiTexCoord2fARB)
	pglMultiTexCoord2fARB(target, s, t);
}

GLAPI void APIENTRY glMultiTexCoord3f (GLenum target, GLfloat s, GLfloat t, GLfloat r) {
	STATICFN(GLMULTITEXCOORD3F, glMultiTexCoord3f)
	if (pglMultiTexCoord3f == NULL)
		glMultiTexCoord3fARB(target, s, t, r);
	pglMultiTexCoord3f(target, s, t, r);
}

GLAPI void APIENTRY glMultiTexCoord3fARB (GLenum target, GLfloat s, GLfloat t, GLfloat r) {
	STATICFN_F(GLMULTITEXCOORD3FARB, glMultiTexCoord3fARB)
	pglMultiTexCoord3fARB(target, s, t, r);
}

GLAPI void APIENTRY glMultiTexCoord4f (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
	STATICFN(GLMULTITEXCOORD4F, glMultiTexCoord4f)
	if (pglMultiTexCoord4f == NULL)
		glMultiTexCoord4fARB(target, s, t, r, q);
	pglMultiTexCoord4f(target, s, t, r, q);
}

GLAPI void APIENTRY glMultiTexCoord4fARB (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
	STATICFN_F(GLMULTITEXCOORD4FARB, glMultiTexCoord4fARB)
	pglMultiTexCoord4fARB(target, s, t, r, q);
}

/* *** GL 1.4 ********************* */

/* BlendFuncSeparate */

GLAPI void APIENTRY glBlendFuncSeparate (GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha) {
	STATICFN(GLBLENDFUNCSEPARATE, glBlendFuncSeparate)
	if (pglBlendFuncSeparate == NULL)
		glBlendFuncSeparateEXT(srcRGB, destRGB, srcAlpha, destAlpha);
	pglBlendFuncSeparate(srcRGB, destRGB, srcAlpha, destAlpha);
}

GLAPI void APIENTRY glBlendFuncSeparateEXT (GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha) {
	STATICFN_F(GLBLENDFUNCSEPARATEEXT, glBlendFuncSeparateEXT)
	pglBlendFuncSeparateEXT(srcRGB, destRGB, srcAlpha, destAlpha);
}

/* FogCoord */

GLAPI void APIENTRY glFogCoordd (GLdouble coord) {
	STATICFN(GLFOGCOORDD, glFogCoordd)
	if (pglFogCoordd == NULL)
		glFogCoorddEXT(coord);
	pglFogCoordd(coord);
}

GLAPI void APIENTRY glFogCoorddEXT (GLdouble coord) {
	STATICFN_F(GLFOGCOORDDEXT, glFogCoorddEXT)
	pglFogCoorddEXT(coord);
}

GLAPI void APIENTRY glFogCoordf (GLfloat coord) {
	STATICFN(GLFOGCOORDF, glFogCoordf)
	if (pglFogCoordf == NULL)
		glFogCoordfEXT(coord);
	pglFogCoordf(coord);
}

GLAPI void APIENTRY glFogCoordfEXT (GLfloat coord) {
	STATICFN_F(GLFOGCOORDFEXT, glFogCoordfEXT)
	pglFogCoordfEXT(coord);
}

GLAPI void APIENTRY glFogCoorddv (const GLdouble* coord) {
	STATICFN(GLFOGCOORDDV, glFogCoorddv)
	if (pglFogCoorddv == NULL)
		glFogCoorddvEXT(coord);
	pglFogCoorddv(coord);
}

GLAPI void APIENTRY glFogCoorddvEXT (const GLdouble* coord) {
	STATICFN_F(GLFOGCOORDDVEXT, glFogCoorddvEXT)
	pglFogCoorddvEXT(coord);
}

GLAPI void APIENTRY glFogCoordfv (const GLfloat* coord) {
	STATICFN(GLFOGCOORDFV, glFogCoordfv)
	if (pglFogCoordfv == NULL)
		glFogCoordfvEXT(coord);
	pglFogCoordfv(coord);
}

GLAPI void APIENTRY glFogCoordfvEXT (const GLfloat* coord) {
	STATICFN_F(GLFOGCOORDFVEXT, glFogCoordfvEXT)
	pglFogCoordfvEXT(coord);
}

/* FogCoordPointer */

GLAPI void APIENTRY glFogCoordPointer (GLenum type, GLsizei stride, GLvoid* pointer) {
	STATICFN(GLFOGCOORDPOINTER, glFogCoordPointer)
	if (pglFogCoordPointer == NULL)
		glFogCoordPointerEXT(type, stride, pointer);
	pglFogCoordPointer(type, stride, pointer);
}

GLAPI void APIENTRY glFogCoordPointerEXT (GLenum type, GLsizei stride, GLvoid* pointer) {
	STATICFN_F(GLFOGCOORDPOINTEREXT, glFogCoordPointerEXT)
	pglFogCoordPointerEXT(type, stride, pointer);
}

/* PointParameter */

GLAPI void APIENTRY glPointParameterf (GLenum pname, GLfloat param) {
	STATICFN(GLPOINTPARAMETERF, glPointParameterf)
	if (pglPointParameterf == NULL)
		glPointParameterfARB(pname, param);
	pglPointParameterf(pname, param);
}

GLAPI void APIENTRY glPointParameterfARB (GLenum pname, GLfloat param) {
	STATICFN_F(GLPOINTPARAMETERFARB, glPointParameterfARB)
	pglPointParameterfARB(pname, param);
}

GLAPI void APIENTRY glPointParameterfv (GLenum pname, const GLfloat* params) {
	STATICFN(GLPOINTPARAMETERFV, glPointParameterfv)
	if (pglPointParameterfv == NULL)
		glPointParameterfvARB(pname, params);
	pglPointParameterfv(pname, params);
}

GLAPI void APIENTRY glPointParameterfvARB (GLenum pname, const GLfloat* params) {
	STATICFN_F(GLPOINTPARAMETERFVARB, glPointParameterfvARB)
	pglPointParameterfvARB(pname, params);
}

GLAPI void APIENTRY glPointParameteri (GLenum pname, GLint param) {
	STATICFN_F(GLPOINTPARAMETERI, glPointParameteri)
	pglPointParameteri(pname, param);
}

GLAPI void APIENTRY glPointParameteriv (GLenum pname, const GLint* params) {
	STATICFN_F(GLPOINTPARAMETERIV, glPointParameteriv)
	pglPointParameteriv(pname, params);
}

/* SecondaryColor3f */

GLAPI void APIENTRY glSecondaryColor3f (GLfloat red, GLfloat blue, GLfloat green) {
	STATICFN(GLSECONDARYCOLOR3F, glSecondaryColor3f)
	if (pglSecondaryColor3f == NULL)
		glSecondaryColor3fEXT(red, blue, green);
	pglSecondaryColor3f(red, blue, green);
}

GLAPI void APIENTRY glSecondaryColor3fEXT (GLfloat red, GLfloat blue, GLfloat green) {
	STATICFN_F(GLSECONDARYCOLOR3FEXT, glSecondaryColor3fEXT)
	pglSecondaryColor3fEXT(red, blue, green);
}

GLAPI void APIENTRY glSecondaryColor3fv (const GLfloat* v) {
	STATICFN(GLSECONDARYCOLOR3FV, glSecondaryColor3fv)
	if (pglSecondaryColor3fv == NULL)
		glSecondaryColor3fvEXT(v);
	pglSecondaryColor3fv(v);
}

GLAPI void APIENTRY glSecondaryColor3fvEXT (const GLfloat* v) {
	STATICFN_F(GLSECONDARYCOLOR3FVEXT, glSecondaryColor3fvEXT)
	pglSecondaryColor3fvEXT(v);
}

/* SecondaryColorPointer */

GLAPI void APIENTRY glSecondaryColorPointer (GLint size, GLenum type, GLsizei stride, GLvoid* pointer) {
	STATICFN(GLSECONDARYCOLORPOINTER, glSecondaryColorPointer)
	if (pglSecondaryColorPointer == NULL)
		glSecondaryColorPointerEXT(size, type, stride, pointer);
	pglSecondaryColorPointer(size, type, stride, pointer);
}

GLAPI void APIENTRY glSecondaryColorPointerEXT (GLint size, GLenum type, GLsizei stride, GLvoid* pointer) {
	STATICFN_F(GLSECONDARYCOLORPOINTEREXT, glSecondaryColorPointerEXT)
	pglSecondaryColorPointerEXT(size, type, stride, pointer);
}

/* WindowsPos */

GLAPI void APIENTRY glWindowPos2f (GLfloat x, GLfloat y) {
	STATICFN(GLWINDOWPOS2F, glWindowPos2f)
	if (pglWindowPos2f == NULL)
		glWindowPos2fARB(x, y);
	pglWindowPos2f(x, y);
}

GLAPI void APIENTRY glWindowPos2fARB (GLfloat x, GLfloat y) {
	STATICFN_F(GLWINDOWPOS2FARB, glWindowPos2fARB)
	pglWindowPos2fARB(x, y);
}

GLAPI void APIENTRY glWindowPos3f (GLfloat x, GLfloat y, GLfloat z) {
	STATICFN(GLWINDOWPOS3F, glWindowPos3f)
	if (pglWindowPos3f == NULL)
		glWindowPos3fARB(x, y, z);
	pglWindowPos3f(x, y, z);
}

GLAPI void APIENTRY glWindowPos3fARB (GLfloat x, GLfloat y, GLfloat z) {
	STATICFN_F(GLWINDOWPOS3FARB, glWindowPos3fARB)
	pglWindowPos3fARB(x, y, z);
}

GLAPI void APIENTRY glWindowPos2fv (const GLfloat* v) {
	STATICFN(GLWINDOWPOS2FV, glWindowPos2fv)
	if (pglWindowPos2fv == NULL)
		glWindowPos2fvARB(v);
	pglWindowPos2fv(v);
}

GLAPI void APIENTRY glWindowPos2fvARB (const GLfloat* v) {
	STATICFN_F(GLWINDOWPOS2FVARB, glWindowPos2fvARB)
	pglWindowPos2fvARB(v);
}

GLAPI void APIENTRY glWindowPos3fv (const GLfloat* v) {
	STATICFN(GLWINDOWPOS3FV, glWindowPos3fv)
	if (pglWindowPos3fv == NULL)
		glWindowPos3fvARB(v);
	pglWindowPos3fv(v);
}

GLAPI void APIENTRY glWindowPos3fvARB (const GLfloat* v) {
	STATICFN_F(GLWINDOWPOS3FVARB, glWindowPos3fvARB)
	pglWindowPos3fvARB(v);
}

/* *** GL 1.5 ********************* */

/* BindBuffer */

GLAPI void APIENTRY glBindBuffer (GLenum target, GLuint buffer) {
	STATICFN(GLBINDBUFFER, glBindBuffer)
	if (pglBindBuffer == NULL)
		glBindBufferARB(target, buffer);
	pglBindBuffer(target, buffer);
}

GLAPI void APIENTRY glBindBufferARB (GLenum target, GLuint buffer) {
	STATICFN_F(GLBINDBUFFERARB, glBindBufferARB)
	pglBindBufferARB(target, buffer);
}

/* BufferData */

GLAPI void APIENTRY glBufferData (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {
	STATICFN(GLBUFFERDATA, glBufferData)
	if (pglBufferData == NULL)
		glBufferDataARB(target, size, data, usage);
	pglBufferData(target, size, data, usage);
}

GLAPI void APIENTRY glBufferDataARB (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {
	STATICFN_F(GLBUFFERDATAARB, glBufferDataARB)
	pglBufferDataARB(target, size, data, usage);
}

/* BufferSubData */

GLAPI void APIENTRY glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data) {
	STATICFN(GLBUFFERSUBDATA, glBufferSubData)
	if (pglBufferSubData == NULL)
		glBufferSubDataARB(target, offset, size, data);
	pglBufferSubData(target, offset, size, data);
}

GLAPI void APIENTRY glBufferSubDataARB (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data) {
	STATICFN_F(GLBUFFERSUBDATAARB, glBufferSubDataARB)
	pglBufferSubDataARB(target, offset, size, data);
}

/* DeleteBuffers */

GLAPI void APIENTRY glDeleteBuffers (GLsizei n, const GLuint* buffers) {
	STATICFN(GLDELETEBUFFERS, glDeleteBuffers)
	if (pglDeleteBuffers == NULL)
		glDeleteBuffersARB(n, buffers);
	pglDeleteBuffers(n, buffers);
}

GLAPI void APIENTRY glDeleteBuffersARB (GLsizei n, const GLuint* buffers) {
	STATICFN_F(GLDELETEBUFFERSARB, glDeleteBuffersARB)
	pglDeleteBuffersARB(n, buffers);
}

/* GenBuffers */

GLAPI void APIENTRY glGenBuffers (GLsizei n, GLuint* buffers) {
	STATICFN(GLGENBUFFERS, glGenBuffers)
	if (pglGenBuffers == NULL)
		glGenBuffersARB(n, buffers);
	pglGenBuffers(n, buffers);
}

GLAPI void APIENTRY glGenBuffersARB (GLsizei n, GLuint* buffers) {
	STATICFN_F(GLGENBUFFERSARB, glGenBuffersARB)
	pglGenBuffersARB(n, buffers);
}

/* GetBufferParameter */

GLAPI void APIENTRY glGetBufferParameteriv (GLenum target, GLenum value, GLint* data) {
	STATICFN(GLGETBUFFERPARAMETERIV, glGetBufferParameteriv)
	if (pglGetBufferParameteriv == NULL)
		glGetBufferParameterivARB(target, value, data);
	pglGetBufferParameteriv(target, value, data);
}

GLAPI void APIENTRY glGetBufferParameterivARB (GLenum target, GLenum value, GLint* data) {
	STATICFN_F(GLGETBUFFERPARAMETERIVARB, glGetBufferParameterivARB)
	pglGetBufferParameterivARB(target, value, data);
}

/* GetBufferPointer */

GLAPI void APIENTRY glGetBufferPointerv (GLenum target, GLenum pname, GLvoid** params) {
	STATICFN(GLGETBUFFERPOINTERV, glGetBufferPointerv)
	if (pglGetBufferPointerv == NULL)
		glGetBufferPointervARB(target, pname, params);
	pglGetBufferPointerv(target, pname, params);
}

GLAPI void APIENTRY glGetBufferPointervARB (GLenum target, GLenum pname, GLvoid** params) {
	STATICFN_F(GLGETBUFFERPOINTERVARB, glGetBufferPointervARB)
	pglGetBufferPointervARB(target, pname, params);
}

/* GetBufferSubData */

GLAPI void APIENTRY glGetBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid* data) {
	STATICFN(GLGETBUFFERSUBDATA, glGetBufferSubData)
	if (pglGetBufferSubData == NULL)
		glGetBufferSubDataARB(target, offset, size, data);
	pglGetBufferSubData(target, offset, size, data);
}

GLAPI void APIENTRY glGetBufferSubDataARB (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid* data) {
	STATICFN_F(GLGETBUFFERSUBDATAARB, glGetBufferSubDataARB)
	pglGetBufferSubDataARB(target, offset, size, data);
}

/* IsBuffer */

GLAPI GLboolean APIENTRY glIsBuffer (GLuint buffer) {
	STATICFN(GLISBUFFER, glIsBuffer)
	if (pglIsBuffer == NULL)
		return glIsBufferARB(buffer);
	return pglIsBuffer(buffer);
}

GLAPI GLboolean APIENTRY glIsBufferARB (GLuint buffer) {
	STATICFN_R(GLISBUFFERARB, glIsBufferARB)
	return pglIsBufferARB(buffer);
}

/* MapBuffer */

GLAPI void* APIENTRY glMapBuffer (GLenum target, GLenum access) {
	STATICFN(GLMAPBUFFER, glMapBuffer)
	if (pglMapBuffer == NULL)
		return glMapBufferARB(target, access);
	return pglMapBuffer(target, access);
}

GLAPI void* APIENTRY glMapBufferARB (GLenum target, GLenum access) {
	STATICFN_R(GLMAPBUFFERARB, glMapBufferARB)
	return pglMapBufferARB(target, access);
}

/* UnmapBuffer */

GLAPI GLboolean APIENTRY glUnmapBuffer (GLenum target) {
	STATICFN(GLUNMAPBUFFER, glUnmapBuffer)
	if (pglUnmapBuffer == NULL)
		return glUnmapBufferARB(target);
	return pglUnmapBuffer(target);
}

GLAPI GLboolean APIENTRY glUnmapBufferARB (GLenum target) {
	STATICFN_R(GLUNMAPBUFFERARB, glUnmapBufferARB)
	return pglUnmapBufferARB(target);
}

/* *** GL 2.0 ********************* */

/* AttachShader */

GLAPI void APIENTRY glAttachShader (GLuint program, GLuint shader) {
	STATICFN(GLATTACHSHADER, glAttachShader)
	if (pglAttachShader == NULL)
		glAttachObjectARB(program, shader);
	pglAttachShader(program, shader);
}

GLAPI void APIENTRY glAttachObjectARB (GLhandleARB program, GLhandleARB shader) {
	STATICFN_F(GLATTACHOBJECTARB, glAttachObjectARB)
	pglAttachObjectARB(program, shader);
}

/* BindAttribLocation */

GLAPI void APIENTRY glBindAttribLocation (GLuint program, GLuint index, const GLchar *name) {
	STATICFN(GLBINDATTRIBLOCATION, glBindAttribLocation)
	if (pglBindAttribLocation == NULL)
		glBindAttribLocationARB(program, index, name);
	pglBindAttribLocation(program, index, name);
}

GLAPI void APIENTRY glBindAttribLocationARB (GLhandleARB program, GLuint index, const GLcharARB *name) {
	STATICFN_F(GLBINDATTRIBLOCATIONARB, glBindAttribLocationARB)
	pglBindAttribLocationARB(program, index, name);
}

/* BlendEquationSeparate */

GLAPI void APIENTRY glBlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha) {
	STATICFN(GLBLENDEQUATIONSEPARATE, glBlendEquationSeparate)
	if (pglBlendEquationSeparate == NULL)
		glBlendEquationSeparateEXT(modeRGB, modeAlpha);
	pglBlendEquationSeparate(modeRGB, modeAlpha);
}

GLAPI void APIENTRY glBlendEquationSeparateEXT (GLenum modeRGB, GLenum modeAlpha) {
	STATICFN_F(GLBLENDEQUATIONSEPARATEEXT, glBlendEquationSeparateEXT)
	pglBlendEquationSeparateEXT(modeRGB, modeAlpha);
}

/* CompileShader */

GLAPI void APIENTRY glCompileShader (GLuint shader) {
	STATICFN(GLCOMPILESHADER, glCompileShader)
	if (pglCompileShader == NULL)
		glCompileShaderARB(shader);
	pglCompileShader(shader);
}

GLAPI void APIENTRY glCompileShaderARB (GLuint shader) {
	STATICFN_F(GLCOMPILESHADERARB, glCompileShaderARB)
	pglCompileShaderARB(shader);
}

/* CreateProgram */

GLAPI GLuint APIENTRY glCreateProgram () {
	STATICFN(GLCREATEPROGRAM, glCreateProgram)
	if (pglCreateProgram == NULL)
		return glCreateProgramObjectARB();
	return pglCreateProgram();
}

GLAPI GLhandleARB APIENTRY glCreateProgramObjectARB () {
	STATICFN_R(GLCREATEPROGRAMOBJECTARB, glCreateProgramObjectARB)
	return pglCreateProgramObjectARB();
}

/* CreateShader */

GLAPI GLuint APIENTRY glCreateShader (GLenum type) {
	STATICFN(GLCREATESHADER, glCreateShader)
	if (pglCreateShader == NULL)
		return glCreateShaderObjectARB(type);
	return pglCreateShader(type);
}

GLAPI GLhandleARB APIENTRY glCreateShaderObjectARB (GLenum type) {
	STATICFN_R(GLCREATESHADEROBJECTARB, glCreateShaderObjectARB)
	return pglCreateShaderObjectARB(type);
}

/* DeleteProgram */

GLAPI void APIENTRY glDeleteProgram (GLuint program) {
	STATICFN_F(GLDELETEPROGRAM, glDeleteProgram)
	pglDeleteProgram(program);
}

/* DeleteShader */

GLAPI void APIENTRY glDeleteShader (GLuint shader) {
	STATICFN(GLDELETESHADER, glDeleteShader)
	if (pglDeleteShader == NULL)
		glDeleteObjectARB(shader);
	pglDeleteShader(shader);
}

GLAPI void APIENTRY glDeleteObjectARB (GLhandleARB shader) {
	STATICFN_F(GLDELETEOBJECTARB, glDeleteObjectARB)
	pglDeleteObjectARB(shader);
}

/* DetachShader */

GLAPI void APIENTRY glDetachShader (GLuint program, GLuint shader) {
	STATICFN(GLDETACHSHADER, glDetachShader)
	if (pglDetachShader == NULL)
		glDetachObjectARB(program, shader);
	pglDetachShader(program, shader);
}

GLAPI void APIENTRY glDetachObjectARB (GLhandleARB program, GLhandleARB shader) {
	STATICFN_F(GLDETACHOBJECTARB, glDetachObjectARB)
	pglDetachObjectARB(program, shader);
}

/* DisableVertexAttribArray */

GLAPI void APIENTRY glDisableVertexAttribArray (GLuint index) {
	STATICFN(GLDISABLEVERTEXATTRIBARRAY, glDisableVertexAttribArray)
	if (pglDisableVertexAttribArray == NULL)
		glDisableVertexAttribArrayARB(index);
	pglDisableVertexAttribArray(index);
}

GLAPI void APIENTRY glDisableVertexAttribArrayARB (GLuint index) {
	STATICFN_F(GLDISABLEVERTEXATTRIBARRAYARB, glDisableVertexAttribArrayARB)
	pglDisableVertexAttribArrayARB(index);
}

/* DrawBuffers */

GLAPI void APIENTRY glDrawBuffers (GLsizei n, const GLenum *bufs) {
	STATICFN(GLDRAWBUFFERS, glDrawBuffers)
	if (pglDrawBuffers == NULL)
		glDrawBuffersARB(n, bufs);
	pglDrawBuffers(n, bufs);
}

GLAPI void APIENTRY glDrawBuffersARB (GLsizei n, const GLenum *bufs) {
	STATICFN_F(GLDRAWBUFFERSARB, glDrawBuffersARB)
	pglDrawBuffersARB(n, bufs);
}

/* EnableVertexAttribArray */

GLAPI void APIENTRY glEnableVertexAttribArray (GLuint index) {
	STATICFN(GLENABLEVERTEXATTRIBARRAY, glEnableVertexAttribArray)
	if (pglEnableVertexAttribArray == NULL)
		glEnableVertexAttribArrayARB(index);
	pglEnableVertexAttribArray(index);
}

GLAPI void APIENTRY glEnableVertexAttribArrayARB (GLuint index) {
	STATICFN_F(GLENABLEVERTEXATTRIBARRAYARB, glEnableVertexAttribArrayARB)
	pglEnableVertexAttribArrayARB(index);
}

/* GetActiveAttrib */

GLAPI void APIENTRY glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) {
	STATICFN(GLGETACTIVEATTRIB, glGetActiveAttrib)
	if (pglGetActiveAttrib == NULL)
		glGetActiveAttribARB(program, index, bufSize, length, size, type, name);
	pglGetActiveAttrib(program, index, bufSize, length, size, type, name);
}

GLAPI void APIENTRY glGetActiveAttribARB (GLhandleARB program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name) {
	STATICFN_F(GLGETACTIVEATTRIBARB, glGetActiveAttribARB)
	pglGetActiveAttribARB(program, index, bufSize, length, size, type, name);
}

/* GetActiveUniform */

GLAPI void APIENTRY glGetActiveUniform (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) {
	STATICFN(GLGETACTIVEUNIFORM, glGetActiveUniform)
	if (pglGetActiveUniform == NULL)
		glGetActiveUniformARB(program, index, bufSize, length, size, type, name);
	pglGetActiveUniform(program, index, bufSize, length, size, type, name);
}

GLAPI void APIENTRY glGetActiveUniformARB (GLhandleARB program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name) {
	STATICFN_F(GLGETACTIVEUNIFORMARB, glGetActiveUniformARB)
	pglGetActiveUniformARB(program, index, bufSize, length, size, type, name);
}

/* GetAttachedShaders */

GLAPI void APIENTRY glGetAttachedShaders (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj) {
	STATICFN(GLGETATTACHEDSHADERS, glGetAttachedShaders)
	if (pglGetAttachedShaders == NULL)
		glGetAttachedObjectsARB(program, maxCount, count, obj);
	pglGetAttachedShaders(program, maxCount, count, obj);
}

GLAPI void APIENTRY glGetAttachedObjectsARB (GLhandleARB program, GLsizei maxCount, GLsizei *count, GLuint *obj) {
	STATICFN_F(GLGETATTACHEDOBJECTSARB, glGetAttachedObjectsARB)
	pglGetAttachedObjectsARB(program, maxCount, count, obj);
}

/* GetAttribLocation */

GLAPI GLint APIENTRY glGetAttribLocation (GLuint program, const GLchar* name) {
	STATICFN(GLGETATTRIBLOCATION, glGetAttribLocation)
	if (pglGetAttribLocation == NULL)
		return glGetAttribLocationARB(program, name);
	return pglGetAttribLocation(program, name);
}

GLAPI GLint APIENTRY glGetAttribLocationARB (GLhandleARB program, const GLcharARB* name) {
	STATICFN_R(GLGETATTRIBLOCATIONARB, glGetAttribLocationARB)
	return pglGetAttribLocationARB(program, name);
}

/* GetProgram */

GLAPI void APIENTRY glGetProgramiv (GLenum program, GLenum pname, GLint* params) {
	STATICFN(GLGETPROGRAMIV, glGetProgramiv)
	if (pglGetProgramiv == NULL)
		glGetProgramivARB(program, pname, params);
	pglGetProgramiv(program, pname, params);
}

GLAPI void APIENTRY glGetProgramivARB (GLenum program, GLenum pname, GLint* params) {
	STATICFN_F(GLGETPROGRAMIVARB, glGetProgramivARB)
	pglGetProgramivARB(program, pname, params);
}

/* GetProgramInfoLog */

GLAPI void APIENTRY glGetProgramInfoLog (GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog) {
	STATICFN_F(GLGETPROGRAMINFOLOG, glGetProgramInfoLog)
	pglGetProgramInfoLog(program, maxLength, length, infoLog);
}

/* GetShader */

GLAPI void APIENTRY glGetShaderiv (GLenum shader, GLenum pname, GLint* params) {
	STATICFN(GLGETSHADERIV, glGetShaderiv)
	if (pglGetShaderiv == NULL)
		glGetObjectParameterivARB(shader, pname, params);
	pglGetShaderiv(shader, pname, params);
}

GLAPI void APIENTRY glGetObjectParameterivARB (GLenum shader, GLenum pname, GLint* params) {
	STATICFN_F(GLGETOBJECTPARAMETERIVARB, glGetObjectParameterivARB)
	pglGetObjectParameterivARB(shader, pname, params);
}

/* GetShaderInfoLog */

GLAPI void APIENTRY glGetShaderInfoLog (GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog) {
	STATICFN_F(GLGETSHADERINFOLOG, glGetShaderInfoLog)
	pglGetShaderInfoLog(shader, maxLength, length, infoLog);
}

/* GetShaderSource */

GLAPI void APIENTRY glGetShaderSource (GLuint shader, GLsizei bufSize, GLsizei* length , GLchar* source) {
	STATICFN(GLGETSHADERSOURCE, glGetShaderSource)
	if (pglGetShaderSource == NULL)
		glGetShaderSourceARB(shader, bufSize, length, source);
	pglGetShaderSource(shader, bufSize, length, source);
}

GLAPI void APIENTRY glGetShaderSourceARB (GLhandleARB shader, GLsizei bufSize, GLsizei* length , GLchar* source) {
	STATICFN_F(GLGETSHADERSOURCEARB, glGetShaderSourceARB)
	pglGetShaderSourceARB(shader, bufSize, length, source);
}

/* GetUniform */

GLAPI void APIENTRY glGetUniformfv (GLuint program, GLint location, GLfloat *params) {
	STATICFN(GLGETUNIFORMFV, glGetUniformfv)
	if (pglGetUniformfv == NULL)
		glGetUniformfvARB(program, location, params);
	pglGetUniformfv(program, location, params);
}

GLAPI void APIENTRY glGetUniformfvARB (GLhandleARB program, GLint location, GLfloat *params) {
	STATICFN_F(GLGETUNIFORMFVARB, glGetUniformfvARB)
	pglGetUniformfvARB(program, location, params);
}

GLAPI void APIENTRY glGetUniformiv (GLuint program, GLint location, GLint *params) {
	STATICFN(GLGETUNIFORMIV, glGetUniformiv)
	if (pglGetUniformiv == NULL)
		glGetUniformivARB(program, location, params);
	pglGetUniformiv(program, location, params);
}

GLAPI void APIENTRY glGetUniformivARB (GLhandleARB program, GLint location, GLint *params) {
	STATICFN_F(GLGETUNIFORMIVARB, glGetUniformivARB)
	pglGetUniformivARB(program, location, params);
}

/* GetUniformLocation */

GLAPI GLint APIENTRY glGetUniformLocation (GLuint program, const GLchar* name) {
	STATICFN(GLGETUNIFORMLOCATION, glGetUniformLocation)
	if (pglGetUniformLocation == NULL)
		return glGetUniformLocationARB(program, name);
	return pglGetUniformLocation(program, name);
}

GLAPI GLint APIENTRY glGetUniformLocationARB (GLhandleARB program, const GLchar* name) {
	STATICFN_R(GLGETUNIFORMLOCATIONARB, glGetUniformLocationARB)
	return pglGetUniformLocationARB(program, name);
}

/* GetVertexAttrib */

GLAPI void APIENTRY glGetVertexAttribdv (GLuint index, GLenum pname, GLdouble *params) {
	STATICFN(GLGETVERTEXATTRIBDV, glGetVertexAttribdv)
	if (pglGetVertexAttribdv == NULL)
		glGetVertexAttribdvARB(index, pname, params);
	pglGetVertexAttribdv(index, pname, params);
}

GLAPI void APIENTRY glGetVertexAttribdvARB (GLuint index, GLenum pname, GLdouble *params) {
	STATICFN_F(GLGETVERTEXATTRIBDVARB, glGetVertexAttribdvARB)
	pglGetVertexAttribdvARB(index, pname, params);
}

GLAPI void APIENTRY glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat *params) {
	STATICFN(GLGETVERTEXATTRIBFV, glGetVertexAttribfv)
	if (pglGetVertexAttribfv == NULL)
		glGetVertexAttribfvARB(index, pname, params);
	pglGetVertexAttribfv(index, pname, params);
}

GLAPI void APIENTRY glGetVertexAttribfvARB (GLuint index, GLenum pname, GLfloat *params) {
	STATICFN_F(GLGETVERTEXATTRIBFVARB, glGetVertexAttribfvARB)
	pglGetVertexAttribfvARB(index, pname, params);
}

GLAPI void APIENTRY glGetVertexAttribiv (GLuint index, GLenum pname, GLint *params) {
	STATICFN(GLGETVERTEXATTRIBIV, glGetVertexAttribiv)
	if (pglGetVertexAttribiv == NULL)
		glGetVertexAttribivARB(index, pname, params);
	pglGetVertexAttribiv(index, pname, params);
}

GLAPI void APIENTRY glGetVertexAttribivARB (GLuint index, GLenum pname, GLint *params) {
	STATICFN_F(GLGETVERTEXATTRIBIVARB, glGetVertexAttribivARB)
	pglGetVertexAttribivARB(index, pname, params);
}

/* IsProgram */

GLAPI GLboolean APIENTRY glIsProgram (GLuint program) {
	STATICFN_R(GLISPROGRAM, glIsProgram)
	return pglIsProgram(program);
}

/* IsShader */

GLAPI GLboolean APIENTRY glIsShader (GLuint shader) {
	STATICFN_R(GLISSHADER, glIsShader)
	return pglIsShader(shader);
}

/* LinkProgram */

GLAPI void APIENTRY glLinkProgram (GLuint program) {
	STATICFN(GLLINKPROGRAM, glLinkProgram)
	if (pglLinkProgram == NULL)
		glLinkProgramARB(program);
	pglLinkProgram(program);
}

GLAPI void APIENTRY glLinkProgramARB (GLhandleARB program) {
	STATICFN_F(GLLINKPROGRAMARB, glLinkProgramARB)
	pglLinkProgramARB(program);
}

/* ShaderSource */

GLAPI void APIENTRY glShaderSource (GLuint shader, GLsizei count, const GLchar** string, const GLint* length) {
	STATICFN(GLSHADERSOURCE, glShaderSource)
	if (pglShaderSource == NULL)
		glShaderSourceARB(shader, count, string, length);
	pglShaderSource(shader, count, string, length);
}

GLAPI void APIENTRY glShaderSourceARB (GLhandleARB shader, GLsizei count, const GLcharARB** string, const GLint* length) {
	STATICFN_F(GLSHADERSOURCEARB, glShaderSourceARB)
	pglShaderSourceARB(shader, count, string, length);
}

/* StencilFuncSeparate */

GLAPI void APIENTRY glStencilFuncSeparate (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask) {
	STATICFN_F(GLSTENCILFUNCSEPARATE, glStencilFuncSeparate)
	pglStencilFuncSeparate(frontfunc, backfunc, ref, mask);
}

/* StencilMaskSeparate */

GLAPI void APIENTRY glStencilMaskSeparate (GLenum face, GLuint mask) {
	STATICFN_F(GLSTENCILMASKSEPARATE, glStencilMaskSeparate)
	pglStencilMaskSeparate(face, mask);
}

/* StencilOpSeparate */

GLAPI void APIENTRY glStencilOpSeparate (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) {
	STATICFN_F(GLSTENCILOPSEPARATE, glStencilOpSeparate)
	pglStencilOpSeparate(face, sfail, dpfail, dppass);
}

/* Uniform */

GLAPI void APIENTRY glUniform1fv (GLint location, GLsizei count, const GLfloat* value) {
	STATICFN(GLUNIFORM1FV, glUniform1fv)
	if (pglUniform1fv == NULL)
		glUniform1fvARB(location, count, value);
	pglUniform1fv(location, count, value);
}

GLAPI void APIENTRY glUniform1fvARB (GLint location, GLsizei count, const GLfloat* value) {
	STATICFN_F(GLUNIFORM1FVARB, glUniform1fvARB)
	pglUniform1fvARB(location, count, value);
}

GLAPI void APIENTRY glUniform1iv (GLint location, GLsizei count, const GLint* value) {
	STATICFN(GLUNIFORM1IV, glUniform1iv)
	if (pglUniform1iv == NULL)
		glUniform1ivARB(location, count, value);
	pglUniform1iv(location, count, value);
}

GLAPI void APIENTRY glUniform1ivARB (GLint location, GLsizei count, const GLint* value) {
	STATICFN_F(GLUNIFORM1IVARB, glUniform1ivARB)
	pglUniform1ivARB(location, count, value);
}

GLAPI void APIENTRY glUniform2fv (GLint location, GLsizei count, const GLfloat* value) {
	STATICFN(GLUNIFORM2FV, glUniform2fv)
	if (pglUniform2fv == NULL)
		glUniform2fvARB(location, count, value);
	pglUniform2fv(location, count, value);
}

GLAPI void APIENTRY glUniform2fvARB (GLint location, GLsizei count, const GLfloat* value) {
	STATICFN_F(GLUNIFORM2FVARB, glUniform2fvARB)
	pglUniform2fvARB(location, count, value);
}

GLAPI void APIENTRY glUniform2iv (GLint location, GLsizei count, const GLint* value) {
	STATICFN(GLUNIFORM2IV, glUniform2iv)
	if (pglUniform2iv == NULL)
		glUniform2ivARB(location, count, value);
	pglUniform2iv(location, count, value);
}

GLAPI void APIENTRY glUniform2ivARB (GLint location, GLsizei count, const GLint* value) {
	STATICFN_F(GLUNIFORM2IVARB, glUniform2ivARB)
	pglUniform2ivARB(location, count, value);
}

GLAPI void APIENTRY glUniform3fv (GLint location, GLsizei count, const GLfloat* value) {
	STATICFN(GLUNIFORM3FV, glUniform3fv)
	if (pglUniform3fv == NULL)
		glUniform3fvARB(location, count, value);
	pglUniform3fv(location, count, value);
}

GLAPI void APIENTRY glUniform3fvARB (GLint location, GLsizei count, const GLfloat* value) {
	STATICFN_F(GLUNIFORM3FVARB, glUniform3fvARB)
	pglUniform3fvARB(location, count, value);
}

GLAPI void APIENTRY glUniform3iv (GLint location, GLsizei count, const GLint* value) {
	STATICFN(GLUNIFORM3IV, glUniform3iv)
	if (pglUniform3iv == NULL)
		glUniform3ivARB(location, count, value);
	pglUniform3iv(location, count, value);
}

GLAPI void APIENTRY glUniform3ivARB (GLint location, GLsizei count, const GLint* value) {
	STATICFN_F(GLUNIFORM3IVARB, glUniform3ivARB)
	pglUniform3ivARB(location, count, value);
}

GLAPI void APIENTRY glUniform4fv (GLint location, GLsizei count, const GLfloat* value) {
	STATICFN(GLUNIFORM4FV, glUniform4fv)
	if (pglUniform4fv == NULL)
		glUniform4fvARB(location, count, value);
	pglUniform4fv(location, count, value);
}

GLAPI void APIENTRY glUniform4fvARB (GLint location, GLsizei count, const GLfloat* value) {
	STATICFN_F(GLUNIFORM4FVARB, glUniform4fvARB)
	pglUniform4fvARB(location, count, value);
}

GLAPI void APIENTRY glUniform4iv (GLint location, GLsizei count, const GLint* value) {
	STATICFN(GLUNIFORM4IV, glUniform4iv)
	if (pglUniform4iv == NULL)
		glUniform4ivARB(location, count, value);
	pglUniform4iv(location, count, value);
}

GLAPI void APIENTRY glUniform4ivARB (GLint location, GLsizei count, const GLint* value) {
	STATICFN_F(GLUNIFORM4IVARB, glUniform4ivARB)
	pglUniform4ivARB(location, count, value);
}

/* UniformMatrix */

GLAPI void APIENTRY glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	STATICFN(GLUNIFORMMATRIX2FV, glUniformMatrix2fv)
	if (pglUniformMatrix2fv == NULL)
		glUniformMatrix2fvARB(location, count, transpose, value);
	pglUniformMatrix2fv(location, count, transpose, value);
}

GLAPI void APIENTRY glUniformMatrix2fvARB (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	STATICFN_F(GLUNIFORMMATRIX2FVARB, glUniformMatrix2fvARB)
	pglUniformMatrix2fvARB(location, count, transpose, value);
}

GLAPI void APIENTRY glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	STATICFN(GLUNIFORMMATRIX3FV, glUniformMatrix3fv)
	if (pglUniformMatrix3fv == NULL)
		glUniformMatrix3fvARB(location, count, transpose, value);
	pglUniformMatrix3fv(location, count, transpose, value);
}

GLAPI void APIENTRY glUniformMatrix3fvARB (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	STATICFN_F(GLUNIFORMMATRIX3FVARB, glUniformMatrix3fvARB)
	pglUniformMatrix3fvARB(location, count, transpose, value);
}

GLAPI void APIENTRY glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	STATICFN(GLUNIFORMMATRIX4FV, glUniformMatrix4fv)
	if (pglUniformMatrix4fv == NULL)
		glUniformMatrix4fvARB(location, count, transpose, value);
	pglUniformMatrix4fv(location, count, transpose, value);
}

GLAPI void APIENTRY glUniformMatrix4fvARB (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	STATICFN_F(GLUNIFORMMATRIX4FVARB, glUniformMatrix4fvARB)
	pglUniformMatrix4fvARB(location, count, transpose, value);
}

/* UseProgram */

GLAPI void APIENTRY glUseProgram (GLuint program) {
	STATICFN(GLUSEPROGRAM, glUseProgram)
	if (pglUseProgram == NULL)
		glUseProgramObjectARB(program);
	pglUseProgram(program);
}

GLAPI void APIENTRY glUseProgramObjectARB (GLhandleARB program) {
	STATICFN_F(GLUSEPROGRAMOBJECTARB, glUseProgramObjectARB)
	pglUseProgramObjectARB(program);
}

/* ValidateProgram */

GLAPI void APIENTRY glValidateProgram (GLuint program) {
	STATICFN(GLVALIDATEPROGRAM, glValidateProgram)
	if (pglValidateProgram == NULL)
		glValidateProgramARB(program);
	pglValidateProgram(program);
}

GLAPI void APIENTRY glValidateProgramARB (GLhandleARB program) {
	STATICFN_F(GLVALIDATEPROGRAMARB, glValidateProgramARB)
	pglValidateProgramARB(program);
}

/* VertexAttrib */

GLAPI void APIENTRY glVertexAttrib1f (GLuint index, GLfloat v0) {
	STATICFN(GLVERTEXATTRIB1F, glVertexAttrib1f)
	if (pglVertexAttrib1f == NULL)
		glVertexAttrib1fARB(index, v0);
	pglVertexAttrib1f(index, v0);
}

GLAPI void APIENTRY glVertexAttrib1fARB (GLuint index, GLfloat v0) {
	STATICFN_F(GLVERTEXATTRIB1FARB, glVertexAttrib1fARB)
	pglVertexAttrib1fARB(index, v0);
}

GLAPI void APIENTRY glVertexAttrib1fv (GLuint index, const GLfloat* v) {
	STATICFN(GLVERTEXATTRIB1FV, glVertexAttrib1fv)
	if (pglVertexAttrib1fv == NULL)
		glVertexAttrib1fvARB(index, v);
	pglVertexAttrib1fv(index, v);
}

GLAPI void APIENTRY glVertexAttrib1fvARB (GLuint index, const GLfloat* v) {
	STATICFN_F(GLVERTEXATTRIB1FVARB, glVertexAttrib1fvARB)
	pglVertexAttrib1fvARB(index, v);
}

GLAPI void APIENTRY glVertexAttrib2f (GLuint index, GLfloat v0, GLfloat v1) {
	STATICFN(GLVERTEXATTRIB2F, glVertexAttrib2f)
	if (pglVertexAttrib2f == NULL)
		glVertexAttrib2fARB(index, v0, v1);
	pglVertexAttrib2f(index, v0, v1);
}

GLAPI void APIENTRY glVertexAttrib2fARB (GLuint index, GLfloat v0, GLfloat v1) {
	STATICFN_F(GLVERTEXATTRIB2FARB, glVertexAttrib2fARB)
	pglVertexAttrib2fARB(index, v0, v1);
}

GLAPI void APIENTRY glVertexAttrib2fv (GLuint index, const GLfloat* v) {
	STATICFN(GLVERTEXATTRIB2FV, glVertexAttrib2fv)
	if (pglVertexAttrib2fv == NULL)
		glVertexAttrib2fvARB(index, v);
	pglVertexAttrib2fv(index, v);
}

GLAPI void APIENTRY glVertexAttrib2fvARB (GLuint index, const GLfloat* v) {
	STATICFN_F(GLVERTEXATTRIB2FVARB, glVertexAttrib2fvARB)
	pglVertexAttrib2fvARB(index, v);
}

GLAPI void APIENTRY glVertexAttrib3f (GLuint index, GLfloat v0, GLfloat v1, GLfloat v2) {
	STATICFN(GLVERTEXATTRIB3F, glVertexAttrib3f)
	if (pglVertexAttrib3f == NULL)
		glVertexAttrib3fARB(index, v0, v1, v2);
	pglVertexAttrib3f(index, v0, v1, v2);
}

GLAPI void APIENTRY glVertexAttrib3fARB (GLuint index, GLfloat v0, GLfloat v1, GLfloat v2) {
	STATICFN_F(GLVERTEXATTRIB3FARB, glVertexAttrib3fARB)
	pglVertexAttrib3fARB(index, v0, v1, v2);
}

GLAPI void APIENTRY glVertexAttrib3fv (GLuint index, const GLfloat* v) {
	STATICFN(GLVERTEXATTRIB3FV, glVertexAttrib3fv)
	if (pglVertexAttrib3fv == NULL)
		glVertexAttrib3fvARB(index, v);
	pglVertexAttrib3fv(index, v);
}

GLAPI void APIENTRY glVertexAttrib3fvARB (GLuint index, const GLfloat* v) {
	STATICFN_F(GLVERTEXATTRIB3FVARB, glVertexAttrib3fvARB)
	pglVertexAttrib3fvARB(index, v);
}

GLAPI void APIENTRY glVertexAttrib4f (GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
	STATICFN(GLVERTEXATTRIB4F, glVertexAttrib4f)
	if (pglVertexAttrib4f == NULL)
		glVertexAttrib4fARB(index, v0, v1, v2, v3);
	pglVertexAttrib4f(index, v0, v1, v2, v3);
}

GLAPI void APIENTRY glVertexAttrib4fARB (GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
	STATICFN_F(GLVERTEXATTRIB4FARB, glVertexAttrib4fARB)
	pglVertexAttrib4fARB(index, v0, v1, v2, v3);
}

GLAPI void APIENTRY glVertexAttrib4fv (GLuint index, const GLfloat* v) {
	STATICFN(GLVERTEXATTRIB4FV, glVertexAttrib4fv)
	if (pglVertexAttrib4fv == NULL)
		glVertexAttrib4fvARB(index, v);
	pglVertexAttrib4fv(index, v);
}

GLAPI void APIENTRY glVertexAttrib4fvARB (GLuint index, const GLfloat* v) {
	STATICFN_F(GLVERTEXATTRIB4FVARB, glVertexAttrib4fvARB)
	pglVertexAttrib4fvARB(index, v);
}

/* VertexAttribPointer */

GLAPI void APIENTRY glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) {
	STATICFN(GLVERTEXATTRIBPOINTER, glVertexAttribPointer)
	if (pglVertexAttribPointer == NULL)
		glVertexAttribPointerARB(index, size, type, normalized, stride, pointer);
	pglVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

GLAPI void APIENTRY glVertexAttribPointerARB (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) {
	STATICFN_F(GLVERTEXATTRIBPOINTERARB, glVertexAttribPointerARB)
	pglVertexAttribPointerARB(index, size, type, normalized, stride, pointer);
}