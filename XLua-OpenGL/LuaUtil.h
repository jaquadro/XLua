#ifndef LUA_UTIL_H_
#define LUA_UTIL_H_

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

extern "C" {
	#include "glext.h"
	#include "lauxlib.h"
}

#include "dll_static/dll_static.h"

int getDimensions (lua_State* L, int index);
int getDimLength (lua_State* L, int index, int dim);

void setField (lua_State *L, unsigned int index, lua_Number value);

bool getArray1bool (lua_State* L, int index, GLboolean** arr, int* n1);
bool getArray1b (lua_State* L, int index, GLbyte** arr, int* n1);
bool getArray1ub (lua_State* L, int index, GLubyte** arr, int* n1);
bool getArray1s (lua_State* L, int index, GLshort** arr, int* n1);
bool getArray1us (lua_State* L, int index, GLushort** arr, int* n1);
bool getArray1i (lua_State* L, int index, GLint** arr, int* n1);
bool getArray1ui (lua_State* L, int index, GLuint** arr, int* n1);
bool getArray1f (lua_State* L, int index, GLfloat** arr, int* n1);
bool getArray1d (lua_State* L, int index, GLdouble** arr, int* n1);
bool getArray2b (lua_State* L, int index, GLbyte** arr, int* n1, int* n2);
bool getArray2ub (lua_State* L, int index, GLubyte** arr, int* n1, int* n2);
bool getArray2s (lua_State* L, int index, GLshort** arr, int* n1, int* n2);
bool getArray2us (lua_State* L, int index, GLushort** arr, int* n1, int* n2);
bool getArray2i (lua_State* L, int index, GLint** arr, int* n1, int* n2);
bool getArray2ui (lua_State* L, int index, GLuint** arr, int* n1, int* n2);
bool getArray2f (lua_State* L, int index, GLfloat** arr, int* n1, int* n2);
bool getArray2d (lua_State* L, int index, GLdouble** arr, int* n1, int* n2);
bool getArray3b (lua_State* L, int index, GLbyte** arr, int* n1, int* n2, int* n3);
bool getArray3ub (lua_State* L, int index, GLubyte** arr, int* n1, int* n2, int* n3);
bool getArray3s (lua_State* L, int index, GLshort** arr, int* n1, int* n2, int* n3);
bool getArray3us (lua_State* L, int index, GLushort** arr, int* n1, int* n2, int* n3);
bool getArray3i (lua_State* L, int index, GLint** arr, int* n1, int* n2, int* n3);
bool getArray3ui (lua_State* L, int index, GLuint** arr, int* n1, int* n2, int* n3);
bool getArray3f (lua_State* L, int index, GLfloat** arr, int* n1, int* n2, int* n3);
bool getArray3d (lua_State* L, int index, GLdouble** arr, int* n1, int* n2, int* n3);
bool getArray4b (lua_State* L, int index, GLbyte** arr, int* n1, int* n2, int* n3, int* n4);
bool getArray4ub (lua_State* L, int index, GLubyte** arr, int* n1, int* n2, int* n3, int* n4);
bool getArray4s (lua_State* L, int index, GLshort** arr, int* n1, int* n2, int* n3, int* n4);
bool getArray4us (lua_State* L, int index, GLushort** arr, int* n1, int* n2, int* n3, int* n4);
bool getArray4i (lua_State* L, int index, GLint** arr, int* n1, int* n2, int* n3, int* n4);
bool getArray4ui (lua_State* L, int index, GLuint** arr, int* n1, int* n2, int* n3, int* n4);
bool getArray4f (lua_State* L, int index, GLfloat** arr, int* n1, int* n2, int* n3, int* n4);
bool getArray4d (lua_State* L, int index, GLdouble** arr, int* n1, int* n2, int* n3, int* n4);

class GLCommon {
public:

	static int formatToElemCount (lua_State* L, GLenum format, const char* func);
	static int typeToPixelSize (lua_State* L, GLenum type, int elemCount, const char* func);

	static const void* get1DPixelsFromSA (lua_State* L, int index, int width, int format, int type, const char* func);
	static const void* get2DPixelsFromSA (lua_State* L, int index, int width, int height, int format, int type, const char* func);
	static const void* get3DPixelsFromSA (lua_State* L, int index, int width, int height, int depth, int format, int type, const char* func);

	static const void* get1DPixelsFromTable (lua_State* L, int index, int width, int format, int type, const char* func);
	static const void* get2DPixelsFromTable (lua_State* L, int index, int width, int height, int format, int type, const char* func);
	static const void* get3DPixelsFromTable (lua_State* L, int index, int width, int height, int depth, int format, int type, const char* func);

	static const void* getDataFromTable (lua_State* L, int index, int type, int* size, const char* func);

public:

	static const int TABLE_NO_ERROR = 0;
	static const int TABLE_DATA_ERROR = 1;
	static const int TABLE_TYPE_ERROR = 2;

	static int _simpleArray1 (lua_State* L, int index, int type, int* n1, const void** data);
	static int _simpleArray2 (lua_State* L, int index, int type, int* n1, int* n2, const void** data);
	static int _simpleArray3 (lua_State* L, int index, int type, int* n1, int* n2, int* n3, const void** data);
	static int _simpleArray4 (lua_State* L, int index, int type, int* n1, int* n2, int* n3, int* n4, const void** data);

	static int _complexArray1 (lua_State* L, int index, int elemCount, int type, int* n1, const void** data);
	static int _complexArray2 (lua_State* L, int index, int elemCount, int type, int* n1, int* n2, const void** data);
	static int _complexArray3 (lua_State* L, int index, int elemCount, int type, int* n1, int* n2, int* n3, const void** data);
};

#endif
