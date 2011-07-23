#include "LuaUtil.h"

void raiseError (lua_State* L, const char* error, const char* func) {
	char buf[256];
	sprintf_s(buf, error, func);
	luaL_error(L, buf);
}

int GLCommon::formatToElemCount (lua_State* L, GLenum format, const char* func) {
	int elemCount = 0;
	switch (format) {
		case GL_RED:
		case GL_GREEN:
		case GL_BLUE:
		case GL_ALPHA:
		case GL_LUMINANCE:
		case GL_INTENSITY:
			elemCount = 1;
			break;
		case GL_LUMINANCE_ALPHA:
			elemCount = 2;
			break;
		case GL_RGB:
		case GL_BGR:
			elemCount = 3;
			break;
		case GL_RGBA:
		case GL_BGRA:
			elemCount = 4;
			break;
		default:
			raiseError(L, "Unsupported format requested in %s", func);
	}

	return elemCount;
}

int GLCommon::typeToPixelSize (lua_State* L, GLenum type, int elemCount, const char* func) {
	int cellSize = 0;
	switch (type) {
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			cellSize = elemCount * 1; break;
		case GL_SHORT: 
		case GL_UNSIGNED_SHORT:
			cellSize = elemCount * 2; break;
		case GL_INT:
		case GL_UNSIGNED_INT: 
		case GL_FLOAT: 
			cellSize = elemCount * 4; break;
		case GL_UNSIGNED_BYTE_3_3_2:
		case GL_UNSIGNED_BYTE_2_3_3_REV:
			cellSize = 1; break;
		case GL_UNSIGNED_SHORT_5_6_5:
		case GL_UNSIGNED_SHORT_5_6_5_REV:
		case GL_UNSIGNED_SHORT_4_4_4_4:
		case GL_UNSIGNED_SHORT_4_4_4_4_REV:
		case GL_UNSIGNED_SHORT_5_5_5_1:
		case GL_UNSIGNED_SHORT_1_5_5_5_REV:
			cellSize = 2; break;
		case GL_UNSIGNED_INT_8_8_8_8:
		case GL_UNSIGNED_INT_8_8_8_8_REV:
		case GL_UNSIGNED_INT_10_10_10_2:
		case GL_UNSIGNED_INT_2_10_10_10_REV:
			cellSize = 4; break;
		default:
			raiseError(L, "Unsupported type requested in %s", func);
	}

	return cellSize;
}

const void* GLCommon::get1DPixelsFromSA (lua_State* L, int index, int width, int format, int type, const char* func) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, index, "lib.array");
	if (!aud->arr)
		raiseError(L, "Invalid static array passed to function %s", func);

	// Discover the number of elements in the requested format
	int elemCount = formatToElemCount(L, format, func);

	// Get the bytes per cell
	int cellSize = typeToPixelSize(L, type, elemCount, func);

	if (aud->arr->size() * aud->arr->count() != cellSize * width)
		raiseError(L, "Size of static array does not match dimensions in %s", func);

	return aud->arr->raw();
}

const void* GLCommon::get2DPixelsFromSA (lua_State* L, int index, int width, int height, int format, int type, const char* func) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, index, "lib.array");
	if (!aud->arr)
		raiseError(L, "Invalid static array passed to function %s", func);

	// Discover the number of elements in the requested format
	int elemCount = formatToElemCount(L, format, func);

	// Get the bytes per cell
	int cellSize = typeToPixelSize(L, type, elemCount, func);

	if (aud->arr->size() * aud->arr->count() != cellSize * width * height)
		raiseError(L, "Size of static array does not match dimensions in %s", func);

	return aud->arr->raw();
}

const void* GLCommon::get3DPixelsFromSA (lua_State* L, int index, int width, int height, int depth, int format, int type, const char* func) {
	ArrayUD* aud = (ArrayUD*)luaL_checkudata(L, index, "lib.array");
	if (!aud->arr)
		raiseError(L, "Invalid static array passed to function %s", func);

	// Discover the number of elements in the requested format
	int elemCount = formatToElemCount(L, format, func);

	// Get the bytes per cell
	int cellSize = typeToPixelSize(L, type, elemCount, func);

	if (aud->arr->size() * aud->arr->count() != cellSize * width * height * depth)
		raiseError(L, "Size of static array does not match dimensions in %s", func);

	return aud->arr->raw();
}

const void* GLCommon::get1DPixelsFromTable (lua_State* L, int index, int width, int format, int type, const char* func) {
	int dimCount = getDimensions(L, index);
	bool ok = false;

	// Discover the number of elements in the requested format
	int elemCount = formatToElemCount(L, format, func);

	int len1 = getDimLength(L, index, 1);

	const void* data = 0;

	// Fetch Data given a 1D organization with flattened vector data
	if (dimCount == 1 && len1 == width * elemCount && elemCount > 1) {
		int n1 = len1;

		switch (GLCommon::_simpleArray1(L, index, type, &n1, &data)) {
			case GLCommon::TABLE_DATA_ERROR: 
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: 
				raiseError(L, "Unsupported type requested using 1D flattened vector data table in %s", func); break;
		}
	}

	// Fetch Data given a 1D organization with scaler data
	else if (dimCount == 1 && len1 == width) {
		int n1 = len1;

		switch (GLCommon::_complexArray1(L, index, elemCount, type, &n1, &data)) {
			case GLCommon::TABLE_DATA_ERROR:
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: {
				char buf[256];
				sprintf_s(buf, "Unsupported type requested for %d-element format using 1D scaler data table in %%s", elemCount);
				raiseError(L, buf, func);
				} break;
		}
	}
	
	// Fetch Data given a 1D organization with vector data
	else if (dimCount == 2 && len1 == width) {
		int n1 = len1;
		int n2 = elemCount;

		switch (GLCommon::_simpleArray2(L, index, type, &n1, &n2, &data)) {
			case GLCommon::TABLE_DATA_ERROR: 
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: 
				raiseError(L, "Unsupported type requested using 1D vector data table in %s", func); break;
		}
	}

	// Our parameters aren't right, oh no!
	else {
		raiseError(L, "Invalid dimensions on data table in %s", func);
	}

	return data;
}

const void* GLCommon::get2DPixelsFromTable (lua_State* L, int index, int width, int height, int format, int type, const char* func) {
	int dimCount = getDimensions(L, index);
	bool ok = false;

	// Discover the number of elements in the requested format
	int elemCount = formatToElemCount(L, format, func);

	int len1 = getDimLength(L, index, 1);
	int len2 = 0;
	if (dimCount >= 2)
		len2 = getDimLength(L, index, 2);

	const void* data = 0;

	// Fetch Data given a 1D organization with flattened vector data
	if (dimCount == 1 && len1 == width * height * elemCount && elemCount > 1) {
		int n1 = len1;

		switch (GLCommon::_simpleArray1(L, index, type, &n1, &data)) {
			case GLCommon::TABLE_DATA_ERROR: 
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: 
				raiseError(L, "Unsupported type requested using 1D flattened vector data table in %s", func); break;
		}
	}

	// Fetch Data given a 1D organization with scaler data
	else if (dimCount == 1 && len1 == width * height) {
		int n1 = len1;

		switch (GLCommon::_complexArray1(L, index, elemCount, type, &n1, &data)) {
			case GLCommon::TABLE_DATA_ERROR:
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: {
				char buf[256];
				sprintf_s(buf, "Unsupported type requested for %d-element format using 1D scaler data table in %%s", elemCount);
				raiseError(L, buf, func);
				} break;
		}
	}
	
	// Fetch Data given a 1D organization with vector data
	else if (dimCount == 2 && len1 == width * height) {
		int n1 = len1;
		int n2 = elemCount;

		switch (GLCommon::_simpleArray2(L, index, type, &n1, &n2, &data)) {
			case GLCommon::TABLE_DATA_ERROR: 
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: 
				raiseError(L, "Unsupported type requested using 1D vector data table in %s", func); break;
		}
	}

	// Fetch Data given a 2D organization with scaler data
	else if (dimCount == 2 && len1 == width && len2 == height) {
		int n1 = len1;
		int n2 = len2;

		switch (GLCommon::_complexArray2(L, index, elemCount, type, &n1, &n2, &data)) {
			case GLCommon::TABLE_DATA_ERROR:
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: {
				char buf[256];
				sprintf_s(buf, "Unsupported type requested for %d-element format using 2D scaler data table in %%s", elemCount);
				raiseError(L, buf, func);
				} break;
		}
	}

	// Fetch data given a 3D data table
	else if (dimCount == 3 && len1 == width && len2 == width) {
		int n1 = len1;
		int n2 = len2;
		int n3 = elemCount;

		switch (GLCommon::_simpleArray3(L, index, type, &n1, &n2, &n3, &data)) {
			case GLCommon::TABLE_DATA_ERROR: 
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: 
				raiseError(L, "Unsupported type requested using 2D vector data table in %s", func); break;
		}
	}

	// Our parameters aren't right, oh no!
	else {
		raiseError(L, "Invalid dimensions on data table in %s", func);
	}

	return data;
}

const void* GLCommon::get3DPixelsFromTable (lua_State* L, int index, int width, int height, int depth, int format, int type, const char* func) {
	int dimCount = getDimensions(L, index);
	bool ok = false;

	// Discover the number of elements in the requested format
	int elemCount = formatToElemCount(L, format, func);

	int len1 = getDimLength(L, index, 1);
	int len2 = 0;
	if (dimCount >= 2)
		len2 = getDimLength(L, index, 2);
	int len3 = 0;
	if (dimCount >= 3)
		len3 = getDimLength(L, index, 3);

	const void* data = 0;

	// Fetch Data given a 1D organization with flattened vector data
	if (dimCount == 1 && len1 == width * height * depth * elemCount && elemCount > 1) {
		int n1 = len1;

		switch (GLCommon::_simpleArray1(L, index, type, &n1, &data)) {
			case GLCommon::TABLE_DATA_ERROR: 
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: 
				raiseError(L, "Unsupported type requested using 1D flattened vector data table in %s", func); break;
		}
	}

	// Fetch Data given a 1D organization with scaler data
	else if (dimCount == 1 && len1 == width * height * depth) {
		int n1 = len1;

		switch (GLCommon::_complexArray1(L, index, elemCount, type, &n1, &data)) {
			case GLCommon::TABLE_DATA_ERROR:
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: {
				char buf[256];
				sprintf_s(buf, "Unsupported type requested for %d-element format using 1D scaler data table in %%s", elemCount);
				raiseError(L, buf, func);
				} break;
		}
	}
	
	// Fetch Data given a 1D organization with vector data
	else if (dimCount == 2 && len1 == width * height * depth) {
		int n1 = len1;
		int n2 = elemCount;

		switch (GLCommon::_simpleArray2(L, index, type, &n1, &n2, &data)) {
			case GLCommon::TABLE_DATA_ERROR: 
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: 
				raiseError(L, "Unsupported type requested using 1D vector data table in %s", func); break;
		}
	}

	// Fetch Data given a 3D organization with scaler data
	else if (dimCount == 3 && len1 == width && len2 == height && len3 == depth) {
		int n1 = len1;
		int n2 = len2;
		int n3 = len3;

		switch (GLCommon::_complexArray3(L, index, elemCount, type, &n1, &n2, &n3, &data)) {
			case GLCommon::TABLE_DATA_ERROR:
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: {
				char buf[256];
				sprintf_s(buf, "Unsupported type requested for %d-element format using 2D scaler data table in %%s", elemCount);
				raiseError(L, buf, func);
				} break;
		}
	}

	// Fetch data given a 4D data table
	else if (dimCount == 4 && len1 == width && len2 == width && len3 == depth) {
		int n1 = len1;
		int n2 = len2;
		int n3 = len3;
		int n4 = elemCount;

		switch (GLCommon::_simpleArray4(L, index, type, &n1, &n2, &n3, &n4, &data)) {
			case GLCommon::TABLE_DATA_ERROR: 
				raiseError(L, "Invalid data table passed to %s", func); break;
			case GLCommon::TABLE_TYPE_ERROR: 
				raiseError(L, "Unsupported type requested using 2D vector data table in %s", func); break;
		}
	}

	// Our parameters aren't right, oh no!
	else {
		raiseError(L, "Invalid dimensions on data table in %s", func);
	}

	return data;
}

const void* GLCommon::getDataFromTable (lua_State* L, int index, int type, int* size, const char* func) {
	int dimCount = getDimensions(L, index);
	bool ok = false;

	int len1 = getDimLength(L, index, 1);
	int szCmp = len1;

	int len2 = 0;
	if (dimCount >= 2) {
		len2 = getDimLength(L, index, 2);
		szCmp *= len2;
	}

	int len3 = 0;
	if (dimCount >= 3) {
		len3 = getDimLength(L, index, 3);
		szCmp *= len3;
	}

	int len4 = 0;
	if (dimCount >= 4) {
		len4 = getDimLength(L, index, 4);
		szCmp *= len4;
	}

	if (dimCount >= 5)
		raiseError(L, "Data tables nested greater than 4D not supported in '%s'", func);

	szCmp *= typeToPixelSize(L, type, 1, func);
	if (*size && szCmp != *size)
		raiseError(L, "Data table does not match requested size in '%s'", func);
	else if (*size == 0)
		*size = szCmp;

	const void* data = 0;

	int code = 0;
	switch (dimCount) {
		case 1: code = _simpleArray1(L, index, type, &len1, &data); break;
		case 2: code = _simpleArray2(L, index, type, &len1, &len2, &data); break;
		case 3: code = _simpleArray3(L, index, type, &len1, &len2, &len3, &data); break;
		case 4: code = _simpleArray4(L, index, type, &len1, &len2, &len3, &len4, &data); break;
	}

	switch (code) {
		case GLCommon::TABLE_DATA_ERROR:
			raiseError(L, "Invalid data table passed to %s", func); break;
		case GLCommon::TABLE_TYPE_ERROR:
			raiseError(L, "Unsupported type requested for data table in %s", func); break;
	}

	return data;
}


// -----


int GLCommon::_simpleArray1 (lua_State* L, int index, int type, int* n1, const void** data) {
	bool ok = false;
	switch (type) {
		case GL_BYTE:
			ok = getArray1b(L, index, (GLbyte**)data, n1); break;
		case GL_UNSIGNED_BYTE:
			ok = getArray1ub(L, index, (GLubyte**)data, n1); break;
		case GL_SHORT: 
			ok = getArray1s(L, index, (GLshort**)data, n1); break;
		case GL_UNSIGNED_SHORT:
			ok = getArray1us(L, index, (GLushort**)data, n1); break;
		case GL_INT:
			ok = getArray1i(L, index, (GLint**)data, n1); break;
		case GL_UNSIGNED_INT: 
			ok = getArray1ui(L, index, (GLuint**)data, n1); break;
		case GL_FLOAT: 
			ok = getArray1f(L, index, (GLfloat**)data, n1); break;
		default:
			return TABLE_TYPE_ERROR;
	}

	if (!ok)
		return TABLE_DATA_ERROR;

	return TABLE_NO_ERROR;
}

int GLCommon::_simpleArray2 (lua_State* L, int index, int type, int* n1, int* n2, const void** data) {
	bool ok = false;
	switch (type) {
		case GL_BYTE:
			ok = getArray2b(L, index, (GLbyte**)data, n1, n2); break;
		case GL_UNSIGNED_BYTE:
			ok = getArray2ub(L, index, (GLubyte**)data, n1, n2); break;
		case GL_SHORT: 
			ok = getArray2s(L, index, (GLshort**)data, n1, n2); break;
		case GL_UNSIGNED_SHORT:
			ok = getArray2us(L, index, (GLushort**)data, n1, n2); break;
		case GL_INT:
			ok = getArray2i(L, index, (GLint**)data, n1, n2); break;
		case GL_UNSIGNED_INT: 
			ok = getArray2ui(L, index, (GLuint**)data, n1, n2); break;
		case GL_FLOAT: 
			ok = getArray2f(L, index, (GLfloat**)data, n1, n2); break;
		default:
			return TABLE_TYPE_ERROR;
	}

	if (!ok)
		return TABLE_DATA_ERROR;

	return TABLE_NO_ERROR;
}

int GLCommon::_simpleArray3 (lua_State* L, int index, int type, int* n1, int* n2, int* n3, const void** data) {
	bool ok = false;
	switch (type) {
		case GL_BYTE:
			ok = getArray3b(L, index, (GLbyte**)data, n1, n2, n3); break;
		case GL_UNSIGNED_BYTE:
			ok = getArray3ub(L, index, (GLubyte**)data, n1, n2, n3); break;
		case GL_SHORT: 
			ok = getArray3s(L, index, (GLshort**)data, n1, n2, n3); break;
		case GL_UNSIGNED_SHORT:
			ok = getArray3us(L, index, (GLushort**)data, n1, n2, n3); break;
		case GL_INT:
			ok = getArray3i(L, index, (GLint**)data, n1, n2, n3); break;
		case GL_UNSIGNED_INT: 
			ok = getArray3ui(L, index, (GLuint**)data, n1, n2, n3); break;
		case GL_FLOAT: 
			ok = getArray3f(L, index, (GLfloat**)data, n1, n2, n3); break;
		default:
			return TABLE_TYPE_ERROR;
	}

	if (!ok)
		return TABLE_DATA_ERROR;

	return TABLE_NO_ERROR;
}

int GLCommon::_simpleArray4 (lua_State* L, int index, int type, int* n1, int* n2, int* n3, int* n4, const void** data) {
	bool ok = false;
	switch (type) {
		case GL_BYTE:
			ok = getArray4b(L, index, (GLbyte**)data, n1, n2, n3, n4); break;
		case GL_UNSIGNED_BYTE:
			ok = getArray4ub(L, index, (GLubyte**)data, n1, n2, n3, n4); break;
		case GL_SHORT: 
			ok = getArray4s(L, index, (GLshort**)data, n1, n2, n3, n4); break;
		case GL_UNSIGNED_SHORT:
			ok = getArray4us(L, index, (GLushort**)data, n1, n2, n3, n4); break;
		case GL_INT:
			ok = getArray4i(L, index, (GLint**)data, n1, n2, n3, n4); break;
		case GL_UNSIGNED_INT: 
			ok = getArray4ui(L, index, (GLuint**)data, n1, n2, n3, n4); break;
		case GL_FLOAT: 
			ok = getArray4f(L, index, (GLfloat**)data, n1, n2, n3, n4); break;
		default:
			return TABLE_TYPE_ERROR;
	}

	if (!ok)
		return TABLE_DATA_ERROR;

	return TABLE_NO_ERROR;
}

int GLCommon::_complexArray1 (lua_State* L, int index, int elemCount, int type, int* n1, const void** data) {
	bool ok = false;

	if (elemCount == 1) {
		return _simpleArray1(L, index, type, n1, data);
	}
	else if (elemCount == 2) {
		return TABLE_TYPE_ERROR;
	}
	else if (elemCount == 3) {
		switch (type) {
			case GL_UNSIGNED_BYTE_3_3_2:
			case GL_UNSIGNED_BYTE_2_3_3_REV:
				ok = getArray1ub(L, index, (GLubyte**)data, n1); break;
			case GL_UNSIGNED_SHORT_5_6_5:
			case GL_UNSIGNED_SHORT_5_6_5_REV:
				ok = getArray1us(L, index, (GLushort**)data, n1); break;
			default:
				return TABLE_TYPE_ERROR;
		}
	}
	else if (elemCount == 4) {
		switch (type) {
			case GL_UNSIGNED_SHORT_4_4_4_4:
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
			case GL_UNSIGNED_SHORT_5_5_5_1:
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
				ok = getArray1us(L, index, (GLushort**)data, n1); break;
			case GL_UNSIGNED_INT_8_8_8_8:
			case GL_UNSIGNED_INT_8_8_8_8_REV:
			case GL_UNSIGNED_INT_10_10_10_2:
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				ok = getArray1ui(L, index, (GLuint**)data, n1); break;
			default:
				return TABLE_TYPE_ERROR;
		}
	}

	if (!ok)
		return TABLE_DATA_ERROR;

	return TABLE_NO_ERROR;
}

int GLCommon::_complexArray2 (lua_State* L, int index, int elemCount, int type, int* n1, int* n2, const void** data) {
	bool ok = false;

	if (elemCount == 1) {
		return _simpleArray2(L, index, type, n1, n2, data);
	}
	else if (elemCount == 2) {
		return TABLE_TYPE_ERROR;
	}
	else if (elemCount == 3) {
		switch (type) {
			case GL_UNSIGNED_BYTE_3_3_2:
			case GL_UNSIGNED_BYTE_2_3_3_REV:
				ok = getArray2ub(L, index, (GLubyte**)data, n1, n2); break;
			case GL_UNSIGNED_SHORT_5_6_5:
			case GL_UNSIGNED_SHORT_5_6_5_REV:
				ok = getArray2us(L, index, (GLushort**)data, n1, n2); break;
			default:
				return TABLE_TYPE_ERROR;
		}
	}
	else if (elemCount == 4) {
		switch (type) {
			case GL_UNSIGNED_SHORT_4_4_4_4:
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
			case GL_UNSIGNED_SHORT_5_5_5_1:
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
				ok = getArray2us(L, index, (GLushort**)data, n1, n2); break;
			case GL_UNSIGNED_INT_8_8_8_8:
			case GL_UNSIGNED_INT_8_8_8_8_REV:
			case GL_UNSIGNED_INT_10_10_10_2:
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				ok = getArray2ui(L, index, (GLuint**)data, n1, n2); break;
			default:
				return TABLE_TYPE_ERROR;
		}
	}

	if (!ok)
		return TABLE_DATA_ERROR;

	return TABLE_NO_ERROR;
}

int GLCommon::_complexArray3 (lua_State* L, int index, int elemCount, int type, int* n1, int* n2, int* n3, const void** data) {
	bool ok = false;

	if (elemCount == 1) {
		return _simpleArray3(L, index, type, n1, n2, n3, data);
	}
	else if (elemCount == 2) {
		return TABLE_TYPE_ERROR;
	}
	else if (elemCount == 3) {
		switch (type) {
			case GL_UNSIGNED_BYTE_3_3_2:
			case GL_UNSIGNED_BYTE_2_3_3_REV:
				ok = getArray3ub(L, index, (GLubyte**)data, n1, n2, n3); break;
			case GL_UNSIGNED_SHORT_5_6_5:
			case GL_UNSIGNED_SHORT_5_6_5_REV:
				ok = getArray3us(L, index, (GLushort**)data, n1, n2, n3); break;
			default:
				return TABLE_TYPE_ERROR;
		}
	}
	else if (elemCount == 4) {
		switch (type) {
			case GL_UNSIGNED_SHORT_4_4_4_4:
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
			case GL_UNSIGNED_SHORT_5_5_5_1:
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
				ok = getArray3us(L, index, (GLushort**)data, n1, n2, n3); break;
			case GL_UNSIGNED_INT_8_8_8_8:
			case GL_UNSIGNED_INT_8_8_8_8_REV:
			case GL_UNSIGNED_INT_10_10_10_2:
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				ok = getArray3ui(L, index, (GLuint**)data, n1, n2, n3); break;
			default:
				return TABLE_TYPE_ERROR;
		}
	}

	if (!ok)
		return TABLE_DATA_ERROR;

	return TABLE_NO_ERROR;
}
